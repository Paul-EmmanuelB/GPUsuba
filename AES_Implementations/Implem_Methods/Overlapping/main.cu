
/***************************************************************************
 *   Copyright (C) 2020                                                    *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


/**
	@author Broux Paul-Emmanuel <paulemmanuelb@gmail.com>
 */

#define BENCH_ON

#include <stdio.h>
#include <cstdlib>

#include "utils.h"
#include "typedef.h"
#include "encryptKernelECB.h"
#include "decryptKernelECB.h"
#include "sboxE.h"
#include "sboxD.h"


int main(int argc, char * argv[]) {


    ///////////////////////////////////////////////////////////////
    // command line arguments
    ///////////////////////////////////////////////////////////////
    int     warm_up_device      = 0;    // GPU kernel warm up
    int     threadNum           = 512;  // Threads per block. This is a recommanded number.
    int     blockNum            = 0;    // Number of blocks in the grid
    int     streamNum           = 2;    // Number of streams to create for overlapping 
    int     mode                = 1;    // Encryption mode, 1 to encrypt or 0 to decrypt.
    char *  filename;
    char *  keyfilename; 

    std::cout << std::endl << "********************************************************************" ;
    std::cout << std::endl << "******                   AES-128 CUDA                         ******" ;
    std::cout << std::endl << "******                   Overlapping                          ******" ;
    std::cout << std::endl << "********************************************************************" << std::endl << std::endl; 

    if (argc > 1){
        for( int n=1 ; n<argc ; n=n+2 ) {
            if((strcmp(argv[n],"-wuDevice") == 0) && (n+1<argc)) {
                warm_up_device = atoi(argv[n+1]);
            }
            else if((strcmp(argv[n],"-threadNum") == 0) && (n+1<argc)) {
                threadNum = atoi(argv[n+1]);
                if(threadNum ==0) {
                    printf("\n threadNum must be a non-null value.\n");
                    exit(1);
                }
            }
            else if((strcmp(argv[n],"-blockNum") == 0) && (n+1<argc)) {
                blockNum = atoi(argv[n+1]);
            }
            else if((strcmp(argv[n],"-streamNum") == 0) && (n+1<argc)) {
                streamNum = atoi(argv[n+1]);
                if(streamNum ==0) {
                    printf("\n streamNum must be a non-null value.\n");
                    exit(1);
                }
                if(streamNum >= 16) {
                    printf("\n streamNum can't exceed 15.\n");
                    exit(1);
                }
            }
            else if((strcmp(argv[n],"-mode") == 0) && (n+1<argc)) {
                mode = atoi(argv[n+1]);
            }
            else if((strcmp(argv[n],"-filename") == 0) && (n+1<argc)) {
                filename = argv[n+1];
            }
            else if((strcmp(argv[n],"-keyfilename") == 0) && (n+1<argc)) {
                keyfilename = argv[n+1];
            }
            else if((strcmp(argv[n],"-help") == 0)) {
                std::cout << "   This is a AES-128 implementation." << std::endl;
				std::cout << "   \"-options value\" availables are:" << std::endl;
                std::cout << "   -mode, 1 to encrypt and 0 to decrypt. Default value is 1." << std::endl;
                std::cout << "   -filename, the file path to encrypt or decrypt." << std::endl;
                std::cout << "   -keyfilename, the 128 key file's path to use for encyption or decryption." << std::endl;
                std::cout << "   -threadNum to set the number of threads per block. Default recommended value is 512." << std::endl;
                std::cout << "   -blockNum to set the number of blocks in the grid. Default value is 0 and will create" << std::endl << "   enough blocks taking into account the input file size and the threadNum argument." << std::endl;
                std::cout << "   -streamNum will set the number of streams and the number of chunk created from the plaintext. Default value is 2 and max is 15." << std::endl;
                std::cout << "   -wuDevice number of device kernel launch before timing (around 1000 or less is adviced) " << std::endl;
                std::cout << "   The order between options is not important." << std::endl << std::endl;
                exit(0);
            }
            else {
                std::cout << std::endl << "Argument " << argv[n] << " does not correspond to any valid arguments. Type -help for details about valid command line arguments." <<std::endl;
		        exit(1);
            }
        }


    }
	else {
		std::cout << std::endl << std::endl << "Not enough arguments. Type -help option in the command line for further explanations." << std::endl;
		exit(1);
	}

    std::cout << "    mode		 = " 	<< mode             << std::endl;
    std::cout << "    threadNum		= " << threadNum        << std::endl;
    std::cout << "    blockNum		= " << blockNum         << std::endl;
    std::cout << "    streamNum		= " << streamNum        << std::endl;
    std::cout << "    wuDevice		= " << warm_up_device   << std::endl << std::endl;

    
    //Copying the key file
    unsigned char key[16];
    FILE * keyFile;
    keyFile = fopen(keyfilename,"rb");
    if (keyFile == NULL) {
        perror ("Error opening file");
        exit(1);
    }
    else {
        for(int i=0 ; i<16 ; i+=4) {
            if(fscanf(keyFile, "%x", (unsigned int *)&key[i]) != 1 ) {
                perror ("Error reading keyfile. Make sure the key is hexadecimal words like \"0x01234567 0x89abcdef ...\" .\n");
                exit(1);
            }
        }
    }
    fclose(keyFile);


    // ***Key scheduling***
    uint8 expkey[176];
    ExpandKey (key, expkey);
    cudaMemcpyToSymbol(const_expkey,  expkey, 176*sizeof(uint8)); //Moving the expanding key to constant memory
    cudaMemcpyToSymbol(const_IK0,  IK0, 256*sizeof(uint32_t));

    // ***Inputdata file to encrypt/decrypt***
    //Checking for the size of the file
    int filesize;
    filesize = fsize(filename);

    //CMS padding to have 16 bytes blocks of data
    uint8 padElmt = 0;
    uint8 streamPad = 0; 
    int mod16 = filesize%16;

    if(mode){
        padElmt = 16 - mod16; // We always add bytes for later padding detection
        mod16 = ((filesize+padElmt)/streamNum)%16; //padding for making each futur chunk multiple of 16
        streamPad = streamNum*(16 - mod16);
    }

    //Creating required arrays with page-locked memory
    uint8 *hostInData;
    checkCudaErrors(cudaHostAlloc((void**)&hostInData, (filesize+padElmt+streamPad)*sizeof(uint8), cudaHostAllocDefault));
    
    //Opening the file
    FILE * inputFile;
    int result;
    inputFile = fopen(filename,"rb");

    if (inputFile == NULL) {
        perror ("Error opening file");
        exit(1);
    }
    result = fread (hostInData, sizeof(uint8), filesize, inputFile);
    if(result != filesize) {
        perror("Reading error from the input file");
        exit(1);
    }
    fclose(inputFile);

    //Padding
    for (int i = 0; i < padElmt; i++) {
		hostInData[filesize + i] = padElmt;
    }
    filesize += padElmt;
    for (int i = 0; i < streamPad; i++) {
		hostInData[filesize + i] = streamPad;
    }
    filesize += streamPad;
    std::cout << "    Data to treat with padding elements: " << filesize  << " bytes."  << std::endl;

    //Determining grid size if not given
    int size = filesize/streamNum; // To treat by each kernel launch
    if(!blockNum) {
        blockNum = 1+size/(threadNum*16);
    }
    else {
        if(blockNum*threadNum* 16 < size) {
            std::cerr << std::endl <<  std::endl << "BlockNum and ThreadNum don't fit the data file ton encrypt/decrypt. ";
            exit(1);
        }
    }
    std::cout << "    Gridsize in term of block: " << blockNum  << std::endl;

    //Streams creation
    cudaStream_t stream[streamNum];
    for (int i = 0; i < streamNum; ++i)
        cudaStreamCreate(&stream[i]);

    //Device vectors declarations and allocations
    uint32_t * devInput, * devOutput, * dev_sm_te1, * dev_sm_te2, * dev_sm_te3, * dev_sm_te4;
    uint8  * dev_sm_sbox;
    cudaMalloc( (void **) &devInput         , filesize*sizeof(uint8));
    cudaMalloc( (void **) &devOutput        , filesize*sizeof(uint8));
    cudaMalloc( (void **) &dev_sm_te1       , 256*sizeof(uint32_t));
    cudaMalloc( (void **) &dev_sm_te2       , 256*sizeof(uint32_t));
    cudaMalloc( (void **) &dev_sm_te3       , 256*sizeof(uint32_t));
    cudaMalloc( (void **) &dev_sm_te4       , 256*sizeof(uint32_t));
    cudaMalloc( (void **) &dev_sm_sbox      , 256*sizeof(uint8));


    //GPU + memory transfers time
    cudaEvent_t startHost, stopHost, delayHost;
	checkCudaErrors(cudaEventCreate(&startHost));
	checkCudaErrors(cudaEventCreate(&stopHost));
    checkCudaErrors(cudaEventCreate(&delayHost));

    //To record device time execution
    cudaEvent_t startDevice, stopDevice;
	checkCudaErrors(cudaEventCreate(&startDevice));
	checkCudaErrors(cudaEventCreate(&stopDevice));

    
    //Copy vectors from host memory to device memory
    if(mode) {
        cudaMemcpy(dev_sm_te1       , TBox0         , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te2       , TBox1         , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te3       , TBox2         , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te4       , TBox3         , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_sbox      , sbox          , 256*sizeof(uint8 ), cudaMemcpyHostToDevice);
    }
    else {
        cudaMemcpy(dev_sm_te1       , TBoxi0            , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te2       , TBoxi1            , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te3       , TBoxi2            , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te4       , TBoxi3            , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_sbox      , sbox_inv          , 256*sizeof(uint8 ), cudaMemcpyHostToDevice);
    }

    uint32_t *hostData = (uint32_t*)hostInData;
    int word = size/4;

    //Warm Up
    cudaMemcpy(devInput, hostInData, filesize*sizeof(uint8_t), cudaMemcpyHostToDevice);
    if(mode) {
        for(int i=0; i < warm_up_device ; i++) { 
            encrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_te1,
        dev_sm_te2, dev_sm_te3, dev_sm_te4, dev_sm_sbox);
        }
    }
    else {
        for(int i=0; i < warm_up_device ; i++) {
                decrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_te1,
            dev_sm_te2, dev_sm_te3, dev_sm_te4, dev_sm_sbox);
        }
    }


    #ifdef BENCH_ON
        if(mode) {
            printf("\nBENCH_ON\n");
            checkCudaErrors(cudaEventRecord(startDevice, NULL));
            for(int j=0; j<1000; j++){
                for(int i=0; i < streamNum ; ++i) {
                    cudaMemcpyAsync(devInput+i*word, hostData+i*word, size, cudaMemcpyHostToDevice, stream[i]);
                    encrypt_Kernel<<<blockNum,threadNum, 0, stream[i]>>>(devInput+i*word, devOutput+i*word, size, dev_sm_te1,
                    dev_sm_te2, dev_sm_te3, dev_sm_te4, dev_sm_sbox);
                    cudaMemcpyAsync(hostData+i*word,  devOutput+i*word, size, cudaMemcpyDeviceToHost, stream[i]);
                }
            }
            checkCudaErrors(cudaEventRecord(stopDevice, NULL));		
        }
        else {
            checkCudaErrors(cudaEventRecord(startDevice, NULL));
            for(int j=0; j<1000; j++){					
                for(int i=0; i < streamNum ; ++i) {
                    cudaMemcpyAsync(devInput+i*word, hostData+i*word, size, cudaMemcpyHostToDevice, stream[i]);
                    decrypt_Kernel<<<blockNum,threadNum, 0, stream[i]>>>(devInput+i*word, devOutput+i*word, size, dev_sm_te1,
                    dev_sm_te2, dev_sm_te3, dev_sm_te4, dev_sm_sbox);
                    cudaMemcpyAsync(hostData+i*word,  devOutput+i*word, size, cudaMemcpyDeviceToHost, stream[i]);
                }
            }
            checkCudaErrors(cudaEventRecord(stopDevice, NULL));
        }
    #else
        if(mode) {
            checkCudaErrors(cudaEventRecord(startDevice, NULL));
            for(int i=0; i < streamNum ; ++i) {
                cudaMemcpyAsync(devInput+i*word, hostData+i*word, size, cudaMemcpyHostToDevice, stream[i]);
                encrypt_Kernel<<<blockNum,threadNum, 0, stream[i]>>>(devInput+i*word, devOutput+i*word, size, dev_sm_te1,
                dev_sm_te2, dev_sm_te3, dev_sm_te4, dev_sm_sbox);
                cudaMemcpyAsync(hostData+i*word,  devOutput+i*word, size, cudaMemcpyDeviceToHost, stream[i]);
            }
            checkCudaErrors(cudaEventRecord(stopDevice, NULL));		
        }
        else {
            checkCudaErrors(cudaEventRecord(startDevice, NULL));						
            for(int i=0; i < streamNum ; ++i) {
                cudaMemcpyAsync(devInput+i*word, hostData+i*word, size, cudaMemcpyHostToDevice, stream[i]);
                decrypt_Kernel<<<blockNum,threadNum, 0, stream[i]>>>(devInput+i*word, devOutput+i*word, size, dev_sm_te1,
                dev_sm_te2, dev_sm_te3, dev_sm_te4, dev_sm_sbox);
                cudaMemcpyAsync(hostData+i*word,  devOutput+i*word, size, cudaMemcpyDeviceToHost, stream[i]);
            }
            checkCudaErrors(cudaEventRecord(stopDevice, NULL));
        }
    #endif
    
    checkCudaErrors(cudaEventSynchronize(stopDevice));	
    checkCudaErrors(cudaDeviceSynchronize());

    //Time calculation
    float Devmsec   = 0.0f;
    
    checkCudaErrors(cudaEventElapsedTime(&Devmsec, startDevice, stopDevice));
    #ifdef BENCH_ON
        Devmsec/= 1000;
    #endif
    double throughput = 1.0e-9f*8*filesize/(Devmsec*1.0e-3f);
    printf("\n	GPU processing time: %f (ms)", Devmsec);
    printf("\n	GPU throughput: %f (Gbps)\n", throughput);

    //Writing results inside a file
    FILE * outputFile;
    outputFile = fopen("Result/result.dat","wb");

    if (outputFile == NULL) {
        perror ("Error opening file");
        exit(1);
    }
    if(mode){
        result = fwrite (hostInData, sizeof(uint8), filesize, outputFile);
        if(result != filesize) {
            perror("Writting error to the output file");
            exit(1);
        }
    }
    else {
        uint8 padTotal = hostInData[filesize - 1];
        padTotal += hostInData[filesize - padTotal - 1];
        result = fwrite (hostInData, sizeof(uint8), filesize-padTotal, outputFile);
        if(result != filesize-padTotal) {
            perror("Writting error to the output file");
            exit(1);
        }
    }
    fclose(outputFile);

    //free streams
    for (int i = 0; i < streamNum; ++i)
        cudaStreamDestroy(stream[i]);

    // Free device memory 
    checkCudaErrors( cudaFreeHost(hostInData) );  
    cudaFree(devInput);
    cudaFree(devOutput);
    cudaFree(dev_sm_te1);
    cudaFree(dev_sm_te2);
    cudaFree(dev_sm_te3);
    cudaFree(dev_sm_te4);
    cudaFree(dev_sm_sbox);
    
	return 0;
}
