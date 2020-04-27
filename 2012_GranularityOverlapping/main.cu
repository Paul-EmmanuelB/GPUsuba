
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
    int     warm_up_device      = 1;    // GPU kernel warm up
	int     warm_up_host        = 0;    // Host warm up (with GPU kernel inside)
    int     threadNum           = 512;  // Threads per block. This is a recommanded number.
    int     blockNum            = 0;    // Number of blocks in the grid
    int     mode                = 1;    // Encryption mode, 1 to encrypt or 0 to decrypt.
    char *  filename;
    char *  keyfilename; 

    std::cout << std::endl << "********************************************************************" ;
    std::cout << std::endl << "******                   AES-128 CUDA                         ******" ;
    std::cout << std::endl << "******                Updated project of                      ******" ;
    std::cout << std::endl << "******  Implementation and Analysis of AES Encryption on GPU  ******" ;
    std::cout << std::endl << "******                      FROM                              ******" ;
    std::cout << std::endl << "******                 Qinjian Li and Al                      ******" ;
    std::cout << std::endl << "********************************************************************" << std::endl << std::endl; 

    if (argc > 1){
        for( int n=1 ; n<argc ; n=n+2 ) {
            if((strcmp(argv[n],"-wuDevice") == 0) && (n+1<argc)) {
                warm_up_device = atoi(argv[n+1]);
            }
            else if((strcmp(argv[n],"-wuHost") == 0) && (n+1<argc)) {
                warm_up_host = atoi(argv[n+1]);
            }
            else if((strcmp(argv[n],"-threadNum") == 0) && (n+1<argc)) {
                threadNum = atoi(argv[n+1]);
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
                std::cout << "   -wuHost number of main Host code launch for several timing results (around 10 should be enough)" << std::endl;
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
    std::cout << "    wuHost		= " << warm_up_host     << std::endl;
    std::cout << "    wuDevice		= " << warm_up_device   << std::endl;

    
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
    printf("TEST\n"); //PRINTTTTTTTTTTTTT
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
    printf("Filesize %d \n",filesize); //PRINTTTTTTTTTTTTT

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
    printf("Blocksize =  %d \n",blockNum);

    //Device vectors declarations and allocations
    uint32_t * devInput, * devOutput, * dev_sm_te1, * dev_sm_te2, * dev_sm_te3, * dev_sm_te4;
    uint8_t  * dev_sm_sbox;
    cudaMalloc( (void **) &devInput         , filesize*sizeof(uint8_t));
    cudaMalloc( (void **) &devOutput        , filesize*sizeof(uint8_t));
    cudaMalloc( (void **) &dev_sm_te1       , 256*sizeof(uint32_t));
    cudaMalloc( (void **) &dev_sm_te2       , 256*sizeof(uint32_t));
    cudaMalloc( (void **) &dev_sm_te3       , 256*sizeof(uint32_t));
    cudaMalloc( (void **) &dev_sm_te4       , 256*sizeof(uint32_t));
    cudaMalloc( (void **) &dev_sm_sbox      , 256*sizeof(uint8_t));

    //Copy vectors from host memory to device memory
    if(mode) {
        cudaMemcpy(dev_sm_te1       , TBox0         , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te2       , TBox1         , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te3       , TBox2         , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te4       , TBox3         , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_sbox      , sbox          , 256*sizeof(uint8_t ), cudaMemcpyHostToDevice);
    }
    else {
        cudaMemcpy(dev_sm_te1       , TBoxi0            , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te2       , TBoxi1            , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te3       , TBoxi2            , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te4       , TBoxi3            , 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_sbox      , sbox_inv          , 256*sizeof(uint8_t ), cudaMemcpyHostToDevice);
    }
    cudaMemcpy(devInput, inputData, filesize*sizeof(uint8_t), cudaMemcpyHostToDevice);
    
    //To record time for device execution
    cudaEvent_t startDevice, stopDevice;
	cudaEventCreate(&startDevice);
	cudaEventCreate(&stopDevice);

    if(mode) {
        for(int i=0; i < warm_up_device ; i++) {
            encrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_te1,
        dev_sm_te2, dev_sm_te3, dev_sm_te4, dev_sm_sbox);
	    }
        cudaEventRecord(startDevice);
        encrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_te1,
        dev_sm_te2, dev_sm_te3, dev_sm_te4, dev_sm_sbox);				
        cudaEventRecord(stopDevice);
        cudaEventSynchronize(stopDevice);
        cudaDeviceSynchronize();
    }
    else {
        for(int i=0; i < warm_up_device ; i++) {
            decrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_te1,
        dev_sm_te2, dev_sm_te3, dev_sm_te4, dev_sm_sbox);
	    }
        cudaEventRecord(startDevice);						
        decrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_te1,
        dev_sm_te2, dev_sm_te3, dev_sm_te4, dev_sm_sbox);
        cudaEventRecord(stopDevice);
        cudaEventSynchronize(stopDevice);
        cudaDeviceSynchronize();
    }

    //Time calculation
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, startDevice, stopDevice);
    printf("\n	GPU processing time: %f (ms)\n", milliseconds);

    //Copy results from host memory to device memory
	cudaMemcpy(outputData, devOutput, filesize*sizeof(uint8_t), cudaMemcpyDeviceToHost);

    //Writing results inside a file
    FILE * outputFile;
    outputFile = fopen("result.dat","wb");

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
        padElmt = outputData[filesize-padElmt-1] + padElmt;
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
    cudaFree(dev_sm_te1);
    cudaFree(dev_sm_te2);
    cudaFree(dev_sm_te3);
    cudaFree(dev_sm_te4);
    cudaFree(dev_sm_sbox);
    
	return 0;

}
