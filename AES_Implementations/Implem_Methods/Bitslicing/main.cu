
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

#define BENCH_ON //Deactivate print messages and introduce warm up and average timing
#define ADV_BTS

#include <stdio.h>
#include <cstdlib>

#include "BtsUtils.h"
#include "transpose.h"

#ifdef ADV_BTS
#include "encryptKernelECB_Advanced.h"
#else
#include "encryptKernelECB.h"
#endif //ADV_BTS

int main(int argc, char * argv[]) {


    ///////////////////////////////////////////////////////////////
    // command line arguments
    ///////////////////////////////////////////////////////////////
    int     warm_up_device      = 0;    // GPU kernel warm up
    int     threadNum           = 64;  // Threads per block. This is a recommanded number.
    int     blockNum            = 0;    // Number of blocks in the grid
    char *  filename;
    char *  keyfilename; 

    std::cout << std::endl << "********************************************************************" ;
    std::cout << std::endl << "******                   AES-128 CUDA                         ******" ;
    std::cout << std::endl << "******                    Bitslicing                          ******" ;
    std::cout << std::endl << "********************************************************************" << std::endl << std::endl; 

    if (argc > 1){
        for( int n=1 ; n<argc ; n=n+2 ) {
            if((strcmp(argv[n],"-wuDevice") == 0) && (n+1<argc)) {
                warm_up_device = atoi(argv[n+1]);
            }
            else if((strcmp(argv[n],"-threadNum") == 0) && (n+1<argc)) {
                threadNum = atoi(argv[n+1]);
                if(threadNum < 0) {
                    printf("\n threadNum must be greater than 0.\n");
                    exit(1);
                }
                #ifdef ADV_BTS
                if(threadNum > 64) {
                    printf("\n threadNum must be lesser than 65.\n");
                    exit(1);
                }
                #endif
            }
            else if((strcmp(argv[n],"-blockNum") == 0) && (n+1<argc)) {
                blockNum = atoi(argv[n+1]);
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
                //exit(1);
            }
        }
    }
    fclose(keyFile);

    // ***Key scheduling***
    uint8 expkey[176];
    ExpandKey (key, expkey);

    //Key transposition
    uint32_t transposed_key[1408] = {0};
    transposeKey((uint32_t*)expkey, transposed_key);

    //PRINT KEY AND TRANSPOSED KEY
    printf("\nEncryption key : ");
    for(int i=0; i<16; i++){
        printf("%2x ",key[i]);
    }

    printf("\nRoundKeys\n");
    for(int i=0; i<11; i++){
        printf("Round %2d   ",i);
        print_state_128(&transposed_key[128*i],0);
    }


    //Attach to constant memory
    cudaMemcpyToSymbol(const_expkey,  transposed_key, 1408*sizeof(uint32_t)); //Moving the expanding key to constant memory



    // ***Inputdata file to encrypt/decrypt***
    //Checking for the size of the file
    int filesize;
    filesize = fsize(filename);

    //CMS padding to have 512 bytes blocks of data
    uint32_t padElmt;
/*     uint32_t mod512 = filesize%512;
    padElmt = 512 - mod512; */
    uint32_t mod512 = filesize%16384;
    padElmt = 16384 - mod512;

    filesize += padElmt;

    //Creating required arrays
    uint8_t inputData[filesize] = {0};
    uint8_t outputData[filesize] = {0};

    //Opening the file
    FILE * inputFile;
    int result;
    inputFile = fopen(filename,"rb");

    if (inputFile == NULL) {
        perror ("Error opening file");
        exit(1);
    }
    result = fread (inputData, sizeof(uint8_t), filesize-padElmt, inputFile);
    if(result != filesize-padElmt) {
        perror("Reading error from the input file");
        exit(1);
    }
    fclose(inputFile);

#ifndef BENCH_ON
    //PRINT PLAIN 
    /*printf("\nPlaintext : \n");
    for(int i=0; i<filesize; i++){
        if(i%16==0 && i)
            printf("| ");
        if(i%32==0 && i)
            printf("\n");
        printf("%2x ",inputData[i]);
    }*/
#endif //BENCH_ON

    std::cout << std::endl << "Data to treat with padding elements: " << filesize  << " bytes."  << std::endl;

    //Transposition for bitslicing
    uint32_t *transposition  = (uint32_t*)inputData;
    for(int i=0; i<filesize/512; i++){
        transpose(transposition);
        transposition += 128;
    }

#ifdef BENCH_ON
/*    //PRINT PLAIN TRANSPOSED
    printf("\n Plain through transposed state : \n");
    for(int i=0; i<filesize/512; i++){
        if(i%16==0 && i)
            printf("| ");
        if(i%32==0 && i)
            printf("\n");
        for(int state=0; state<32; state++)
            print_state_128(&transposition[i*128], state);
    }
*/
#endif //BENCH_ON

    //Determining grid size if not given
    if(!blockNum) {
        blockNum = 1+filesize/(threadNum*512);
    }
    std::cout << "Gridsize in term of block: " << blockNum  << std::endl;

    //Device vectors declarations and allocations
    uint32_t * devInput, * devOutput;
    cudaMalloc( (void **) &devInput  , filesize*sizeof(uint8_t));
    cudaMalloc( (void **) &devOutput , filesize*sizeof(uint8_t));

    //GPU + memory transfers time
    cudaEvent_t startHost, stopHost;
	checkCudaErrors(cudaEventCreate(&startHost));
	checkCudaErrors(cudaEventCreate(&stopHost));

    //To record device time execution
    cudaEvent_t startDevice, stopDevice;
	checkCudaErrors(cudaEventCreate(&startDevice));
	checkCudaErrors(cudaEventCreate(&stopDevice));

    cudaMemcpy(devInput, inputData, filesize*sizeof(uint8_t), cudaMemcpyHostToDevice);

    //Warm Up
    for(int i=0; i < warm_up_device ; i++) { 
        encrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize);
    }

#ifdef BENCH_ON
    printf("\nBENCH_ON\n");
    checkCudaErrors(cudaEventRecord(startHost, NULL));
    for(int j=0; j<1000; j++){ //for benchmarking
        encrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize);
    }
    //checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaEventRecord(stopHost, NULL));
#else
    checkCudaErrors(cudaEventRecord(startHost, NULL));
    //cudaMemcpy(devInput, inputData, filesize*sizeof(uint8_t), cudaMemcpyHostToDevice);
    encrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaEventRecord(stopHost, NULL));
#endif

    checkCudaErrors(cudaEventSynchronize(stopHost));
    cudaMemcpy(outputData, devOutput, filesize*sizeof(uint8_t), cudaMemcpyDeviceToHost);

    //Time spent
    float Hostmsec  = 0.0f;
    double throughput;
    checkCudaErrors(cudaEventElapsedTime(&Hostmsec, startHost, stopHost));
#ifdef BENCH_ON
    Hostmsec /= 1000;
#endif
    throughput = 1.0e-9f*8*filesize/(Hostmsec*1.0e-3f);
    printf("\nTotal processing time: %f (ms)", Hostmsec);
    printf("\nTotal throughput: %f (Gbps)\n", throughput);

    //Transposing back
    transposition  = (uint32_t*)outputData;
#ifndef BENCH_ON
    //Print
    /*printf("\n Cipher Text : \n");
    for(int i=0; i<filesize/512; i++){
        if(i%16==0 && i)
            printf("| ");
        if(i%32==0 && i)
            printf("\n");
        for(int state=0; state<32; state++)
            print_state_128(&transposition[i*128], state);
    }*/
#endif //BENCH_ON    

    for(int i=0; i<filesize/512; i++){
        invTranspose(transposition);
        transposition += 128;
    }

#ifndef BENCH_ON
/*
    //PRINT CIPHER 
    printf("\nCiphertext : \n");
    for(int i=0; i<filesize; i++){
        if(i%16==0 && i)
            printf("| ");
        if(i%32==0 && i)
            printf("\n");
        printf("%2x ",outputData[i]);
    }
*/
#endif //BENCH_ON

    //Writing results inside a file
    FILE * outputFile;
    outputFile = fopen("Result/resultBts.dat","wb");

    if (outputFile == NULL) {
        perror ("Error opening file");
        exit(1);
    }
    result = fwrite (outputData, sizeof(uint8_t), filesize-padElmt, outputFile);
    if(result != filesize-padElmt) {
        perror("Writting error to the output file");
        exit(1);
    }
    fclose(outputFile); 

    // Free device memory
    cudaFree(devInput);
    cudaFree(devOutput);
	return 0;

}
