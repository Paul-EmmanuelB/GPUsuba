
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
#include <cstdlib>
#include <stdio.h>

#include "utils.h"
#include "typedef.h"
#include "sboxE.h"
#include "sboxD.h"
#include "encryptKernelECB.h"
#include "decryptKernelECB.h"

int main(int argc, char * argv[]) {


    ///////////////////////////////////////////////////////////////
    // command line arguments
    ///////////////////////////////////////////////////////////////
    int     warm_up_device      = 0;    // GPU kernel warm up
    int     threadNum           = 512;  // Threads per block. This is a recommanded number.
    int     blockNum            = 0;    // Number of blocks in the grid
    int     mode                = 1;    // Encryption mode, 1 to encrypt or 0 to decrypt.
    char *  filename;
    char *  keyfilename; 

    std::cout << std::endl << "********************************************************************" ;
    std::cout << std::endl << "******                   AES-128 CUDA                         ******" ;
    std::cout << std::endl << "******                   Texture Memory                       ******" ;
    std::cout << std::endl << "********************************************************************" << std::endl << std::endl; 

    if (argc > 1){
        for( int n=1 ; n<argc ; n=n+2 ) {
            if((strcmp(argv[n],"-wuDevice") == 0) && (n+1<argc)) {
                warm_up_device = atoi(argv[n+1]);
            }
            else if((strcmp(argv[n],"-threadNum") == 0) && (n+1<argc)) {
                threadNum = atoi(argv[n+1]);
                if(threadNum <256) {
                    printf("\n threadNum must be superior to 256.\n");
                    exit(1);
                }
            }
            else if((strcmp(argv[n],"-blockNum") == 0) && (n+1<argc)) {
                blockNum = atoi(argv[n+1]);
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
                std::cout << "   -blockNum to set the number of blocks in the grid. Default value is 0 and will create enough blocks taking into account the input file size and the threadNum argument." << std::endl;
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
    cudaMemcpyToSymbol(const_IK0,  IK0, 256*sizeof(uint32));

    // ***Inputdata file to encrypt/decrypt***
    //Checking for the size of the file
    int filesize;
    filesize = fsize(filename);

    //CMS padding to have 16 bytes blocks of data
    uint8_t padElmt;
    int mod16 = filesize%16;

    padElmt = 16 - mod16; // We always add bytes for later padding detection

    //Creating required arrays
    uint8_t *inputData;
    uint8_t *outputData;
    inputData = (uint8_t*)malloc((filesize+padElmt)*sizeof(uint8_t));
    outputData = (uint8_t*)malloc((filesize+padElmt)*sizeof(uint8_t));

    //Opening the file
    FILE * inputFile;
    int result;
    inputFile = fopen(filename,"rb");

    if (inputFile == NULL) {
        perror ("Error opening file");
        exit(1);
    }
    result = fread (inputData, sizeof(uint8_t), filesize, inputFile);
    if(result != filesize) {
        perror("Reading error from the input file");
        exit(1);
    }
    fclose(inputFile);

    //Padding
    for (int i = 0; i < padElmt; i++) {
		inputData[filesize + i] = padElmt;
    }
	filesize += padElmt;
    std::cout << "    Data to treat with padding elements: " << filesize  << " bytes."  << std::endl;

    //Determining grid size if not given
    if(!blockNum) {
        blockNum = 1+filesize/(threadNum*16);
    }
    else {
        if(blockNum*threadNum* 16 < filesize) {
            std::cerr << std::endl <<  std::endl << "BlockNum and ThreadNum don't fit the data file ton encrypt/decrypt. ";
            exit(1);
        }
    }
    std::cout << "    Gridsize in term of block: " << blockNum  << std::endl;

    //Device vectors declarations and allocations
    uint32 * devInput, * devOutput;
    uint8_t  * dev_sm_sbox;
    checkCudaErrors(cudaMalloc( (void **) &devInput         , filesize*sizeof(uint8_t)));
    checkCudaErrors(cudaMalloc( (void **) &devOutput        , filesize*sizeof(uint8_t)));
    checkCudaErrors(cudaMalloc( (void **) &dev_sm_sbox      , 256*sizeof(uint8_t)));

    /// Allocate CUDA Texture objects in device memory
    uint32 *buffer0, *buffer1, *buffer2, *buffer3;
    checkCudaErrors(cudaMalloc(&buffer0, 256*sizeof(uint32)));
    checkCudaErrors(cudaMalloc(&buffer1, 256*sizeof(uint32)));
    checkCudaErrors(cudaMalloc(&buffer2, 256*sizeof(uint32)));
    checkCudaErrors(cudaMalloc(&buffer3, 256*sizeof(uint32)));
    
    // Copy to device memory Tbox data
    if(mode){
        checkCudaErrors(cudaMemcpy(buffer0,  TBox0, 256*sizeof(uint32), cudaMemcpyHostToDevice));
        checkCudaErrors(cudaMemcpy(buffer1,  TBox1, 256*sizeof(uint32), cudaMemcpyHostToDevice));
        checkCudaErrors(cudaMemcpy(buffer2,  TBox2, 256*sizeof(uint32), cudaMemcpyHostToDevice));
        checkCudaErrors(cudaMemcpy(buffer3,  TBox3, 256*sizeof(uint32), cudaMemcpyHostToDevice));
    }
    else{
        checkCudaErrors(cudaMemcpy(buffer0,  TBoxi0, 256*sizeof(uint32), cudaMemcpyHostToDevice));
        checkCudaErrors(cudaMemcpy(buffer1,  TBoxi1, 256*sizeof(uint32), cudaMemcpyHostToDevice));
        checkCudaErrors(cudaMemcpy(buffer2,  TBoxi2, 256*sizeof(uint32), cudaMemcpyHostToDevice));
        checkCudaErrors(cudaMemcpy(buffer3,  TBoxi3, 256*sizeof(uint32), cudaMemcpyHostToDevice));
    }

    // create texture objects
    cudaResourceDesc resDesc0, resDesc1, resDesc2, resDesc3;
    memset(&resDesc0, 0, sizeof(resDesc0));
    memset(&resDesc1, 0, sizeof(resDesc1));
    memset(&resDesc2, 0, sizeof(resDesc2));
    memset(&resDesc3, 0, sizeof(resDesc3));
    resDesc0.resType = cudaResourceTypeLinear;
    resDesc1.resType = cudaResourceTypeLinear;
    resDesc2.resType = cudaResourceTypeLinear;
    resDesc3.resType = cudaResourceTypeLinear;
    resDesc0.res.linear.devPtr = buffer0;
    resDesc1.res.linear.devPtr = buffer1;
    resDesc2.res.linear.devPtr = buffer2;
    resDesc3.res.linear.devPtr = buffer3;
    resDesc0.res.linear.desc.f = cudaChannelFormatKindUnsigned;
    resDesc1.res.linear.desc.f = cudaChannelFormatKindUnsigned;
    resDesc2.res.linear.desc.f = cudaChannelFormatKindUnsigned;
    resDesc3.res.linear.desc.f = cudaChannelFormatKindUnsigned;
    resDesc0.res.linear.desc.x = 32; // bits per channel
    resDesc1.res.linear.desc.x = 32; // bits per channel
    resDesc2.res.linear.desc.x = 32; // bits per channel
    resDesc3.res.linear.desc.x = 32; // bits per channel
    resDesc0.res.linear.sizeInBytes = 256 * sizeof(uint32);
    resDesc1.res.linear.sizeInBytes = 256 * sizeof(uint32);
    resDesc2.res.linear.sizeInBytes = 256 * sizeof(uint32);
    resDesc3.res.linear.sizeInBytes = 256 * sizeof(uint32);
    

    cudaTextureDesc texDesc;
    memset(&texDesc, 0, sizeof(texDesc));
    texDesc.readMode = cudaReadModeElementType;

    // create texture object: we only have to do this once!
    cudaTextureObject_t tex0 = 0, tex1 = 0, tex2 = 0, tex3 = 0;
    checkCudaErrors(cudaCreateTextureObject(&tex0, &resDesc0, &texDesc, NULL));
    checkCudaErrors(cudaCreateTextureObject(&tex1, &resDesc1, &texDesc, NULL));
    checkCudaErrors(cudaCreateTextureObject(&tex2, &resDesc2, &texDesc, NULL));
    checkCudaErrors(cudaCreateTextureObject(&tex3, &resDesc3, &texDesc, NULL));


    //GPU + memory transfers time
    cudaEvent_t startHost, stopHost;
	checkCudaErrors(cudaEventCreate(&startHost));
	checkCudaErrors(cudaEventCreate(&stopHost));

    //To record device time execution
    cudaEvent_t startDevice, stopDevice;
	checkCudaErrors(cudaEventCreate(&startDevice));
	checkCudaErrors(cudaEventCreate(&stopDevice));


    //Copy vectors from host memory to device memory
    if(mode) {
        checkCudaErrors(cudaMemcpy(dev_sm_sbox, sbox    ,   256*sizeof(uint8_t ), cudaMemcpyHostToDevice));
    }
    else {
        checkCudaErrors(cudaMemcpy(dev_sm_sbox, sbox_inv,   256*sizeof(uint8_t ), cudaMemcpyHostToDevice));
    }
    checkCudaErrors(cudaMemcpy(devInput, inputData, filesize*sizeof(uint8_t), cudaMemcpyHostToDevice));

    //Warm Up
    if(mode) {
        for(int i=0; i < warm_up_device ; i++) { 
            encrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_sbox, 
            tex0, tex1, tex2, tex3);
        }
    }
    else {
        for(int i=0; i < warm_up_device ; i++) {
            decrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_sbox,
            tex0, tex1, tex2, tex3);
        }
    }



    #ifdef BENCH_ON
        printf("\nBENCH_ON\n");
        if(mode) {
            checkCudaErrors(cudaEventRecord(startHost, NULL));
            for(int j=0; j<1000; j++){ //for benchmarking
                //cudaMemcpy(devInput, inputData, filesize*sizeof(uint8_t), cudaMemcpyHostToDevice);
                //checkCudaErrors(cudaEventRecord(startDevice, NULL));
                encrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_sbox,
                tex0, tex1, tex2, tex3);
                //checkCudaErrors(cudaEventRecord(stopDevice, NULL));	
                //cudaMemcpy(outputData, devOutput, filesize*sizeof(uint8_t), cudaMemcpyDeviceToHost);	
            }
            checkCudaErrors(cudaEventRecord(stopHost, NULL));
        } 
        else {
            checkCudaErrors(cudaEventRecord(startHost, NULL));
            for(int j=0; j<1000; j++){ //for benchmarking
                //cudaMemcpy(devInput, inputData, filesize*sizeof(uint8_t), cudaMemcpyHostToDevice);
                //checkCudaErrors(cudaEventRecord(startDevice, NULL));						
                decrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_sbox,
                tex0, tex1, tex2, tex3);
                //checkCudaErrors(cudaEventRecord(stopDevice, NULL));
                //cudaMemcpy(outputData, devOutput, filesize*sizeof(uint8_t), cudaMemcpyDeviceToHost);
            }
            checkCudaErrors(cudaEventRecord(stopHost, NULL));
        }
    #else
        if(mode) {
            checkCudaErrors(cudaEventRecord(startHost, NULL));
            cudaMemcpy(devInput, inputData, filesize*sizeof(uint8_t), cudaMemcpyHostToDevice);
            //checkCudaErrors(cudaEventRecord(startDevice, NULL));
            encrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_sbox,
            tex0, tex1, tex2, tex3);
            //checkCudaErrors(cudaEventRecord(stopDevice, NULL));		
        }
        else {
            checkCudaErrors(cudaEventRecord(startHost, NULL));
            cudaMemcpy(devInput, inputData, filesize*sizeof(uint8_t), cudaMemcpyHostToDevice);
            //checkCudaErrors(cudaEventRecord(startDevice, NULL));						
            decrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_sbox,
            tex0, tex1, tex2, tex3);
            //checkCudaErrors(cudaEventRecord(stopDevice, NULL));
        }
    #endif
    cudaMemcpy(outputData, devOutput, filesize*sizeof(uint8_t), cudaMemcpyDeviceToHost);
    checkCudaErrors(cudaEventRecord(stopHost, NULL));
    

    //Make sure to end following events before continuing 
    //checkCudaErrors(cudaEventSynchronize(stopDevice));	
    checkCudaErrors(cudaEventSynchronize(stopHost));

    //Time calculation
    //float Devmsec   = 0.0f;
    float Hostmsec  = 0.0f;
    double throughput;
    
    /*checkCudaErrors(cudaEventElapsedTime(&Devmsec, startDevice, stopDevice));
    throughput = 1.0e-9f*8*filesize/(Devmsec*1.0e-3f);
    printf("\n	GPU processing time: %f (ms)", Devmsec);
    printf("\n	GPU throughput: %f (Gbps)\n", throughput);*/
    
    checkCudaErrors(cudaEventElapsedTime(&Hostmsec, startHost, stopHost));
    #ifdef BENCH_ON
        Hostmsec /= 1000;
    #endif
    throughput = 1.0e-9f*8*filesize/(Hostmsec*1.0e-3f);
    printf("\n	Total processing time: %f (ms)", Hostmsec);
    printf("\n	Total throughput: %f (Gbps)\n", throughput);

    //Writing results inside a file
    FILE * outputFile;
    outputFile = fopen("Result/result.dat","wb");

    if (outputFile == NULL) {
        perror ("Error opening file");
        exit(1);
    }
    if(mode){
        result = fwrite (outputData, sizeof(uint8_t), filesize, outputFile);
        if(result != filesize) {
            perror("Writting error to the output file");
            exit(1);
        }
    }
    else {
        padElmt += outputData[filesize-padElmt-1];
        result = fwrite (outputData, sizeof(uint8_t), filesize-padElmt, outputFile);
        if(result != filesize-padElmt) {
            perror("Writting error to the output file");
            exit(1);
        }
    }
    fclose(outputFile);

    //Free host memory
    free(inputData);
    free(outputData);
    // Free device memory
    cudaFree(devInput);
    cudaFree(devOutput);
    cudaFree(dev_sm_sbox);
    cudaFree(buffer0);
    cudaFree(buffer1);
    cudaFree(buffer2);
    cudaFree(buffer3);

    // destroy texture object
    cudaDestroyTextureObject(tex0);
    cudaDestroyTextureObject(tex1);
    cudaDestroyTextureObject(tex2);
    cudaDestroyTextureObject(tex3);
    
	return 0;

}
