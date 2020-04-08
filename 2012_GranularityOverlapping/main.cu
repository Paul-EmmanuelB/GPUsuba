
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

#include "encryptKernelECB.h"
#include "decryptKernelECB.h"
#include "utils.h"
#include "sboxE.h"
#include "sboxD.h"

int main(int argc, char * argv[]) {


    ///////////////////////////////////////////////////////////////
    // command line arguments
    ///////////////////////////////////////////////////////////////
    int     warm_up_device      = 0;    // GPU kernel warm up
	int     warm_up_host        = 1;    // Host warm up (with GPU kernel inside)
    int     threadNum           = 512;  // Threads per block. This is a recommanded number.
    int     blockNum            = 0;    // Number of blocks in the grid
    int     mode                = 1;    // Encryption mode, 1 to encrypt or 0 to decrypt.
    char *  filename;
    char *  inputKey; 

    std::cout << std::endl << "********************************************************" << std::endl;
    std::cout << std::endl << "******                     AES CUDA               ******" << std::endl;
    std::cout << std::endl << "********************************************************" << std::endl;


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
                strcpy(filename,argv[n+1]);
            }
            else if((strcmp(argv[n],"-inputKey") == 0) && (n+1<argc)) {
                strcpy(inputKey,argv[n+1]);
            }
            else if((strcmp(argv[n],"-help") == 0)) {
                std::cout << "   This is a AES-128 implementation." << std::endl;
				std::cout << "   \"-options value\" availables are:" << std::endl;
                std::cout << "   -mode, 1 to encrypt and 0 to decrypt. Default value is 1." << std::endl;
                std::cout << "   -filename, the file path to encrypt or decrypt." << std::endl;
                std::cout << "   -inputKey, the 128 key file's path to use for encyption or decryption." << std::endl;
                std::cout << "   -threadNum to set the number of threads per block. Default recommended value is 512." << std::endl;
                std::cout << "   -blockNum to set the number of blocks in the grid. Default value is 0 and will create enough blocks
                taking into account the input file size and the threadNum argument." << std::endl;
                std::cout << "   -wuDevice number of device kernel launch before timing (around 1000 or less is adviced) " << std::endl;
                std::cout << "   -wuHost number of main Host code launch for several timing results (around 10 should be enough)" << std::endl;
                std::cout << "   The order between options is not important." << std::endl << std::endl;
                exit(0);
            }
            else {
                std::cout << "Argument " << argv[n] << " does not correspond to any valid arguments. Type -help for details
                about valid command line arguments." <<std::endl;
		        exit(0);
            }
        }


    }
	else {
		std::cout << "   Not enough arguments. Type -help option in the command line for further explanations." << std::endl;
		exit(0);
	}

    std::cout << "    mode		    = " << mode             << std::endl;
    std::cout << "    threadNum		= " << threadNum        << std::endl;
    std::cout << "    blockNum		= " << blockNum         << std::endl;
    std::cout << "    wuHost		= " << warm_up_host     << std::endl;
    std::cout << "    wuDevice		= " << warm_up_device   << std::endl;


    //Copying the key file
    const unsigned char * keyCryptFile;
    ifstream keyFile(filename, ios::binary);
    if(!keyFile) {
        cerr << "Read error";
        exit(1);
    }
    keyFile >> keyCryptFile;

    //Key scheduling
    AES_KEY *key;

    if(mode) {
        AES_cuda_set_encrypt_key(keyCryptFile, 128, key);
    }
    else {
        AES_cuda_set_decrypt_key(keyCryptFile, 128, key);
    }

    //Checking for the size of the file to encrypt/decrypt
    size_t filesize;
    filesize = filesize(filename);
    
    //CMS padding to have 16 bytes blocks of data
    uint8_t padElmt;
    uint8_t mod16 = filesize%16;

    padElmt = 16 - mod16; // We always add bytes for future padding detection

    uint32_t inputData[filesize+padElmt];
    uint32_t outputData[filesize];

    ifstream inputFile(filename, ios::binary);
    if(!inputFile) {
        cerr << "Read error";
        exit(1);
    }
    inputFile >> inputData;

    for (int i = 0; i < padElmt; i++) {
				inputData[filesize + i] = padElmt;
    }
	filesize += padElmt;

    
    //Determining grid size if not given
    if(!blockNum) {
        blockNum = 1+filesize/(threadNum*16);
    }

    //Device vectors declarations and allocations
    uint32_t * devIntput, * devOutput, * dev_sm_te1, * dev_sm_te2, * dev_sm_te3, * dev_sm_te4;
    cudaMalloc(&devInput, filesize*sizeof(uint32_t));
    cudaMalloc(&devOutput, filesize*sizeof(uint32_t));
    cudaMalloc(&dev_sm_te1, 256*sizeof(uint32_t));
    cudaMalloc(&dev_sm_te2, 256*sizeof(uint32_t));
    cudaMalloc(&dev_sm_te3, 256*sizeof(uint32_t));
    cudaMalloc(&dev_sm_te4, 256*sizeof(uint32_t));

    //Copy vectors from host memory to device memory
    if(mode) {
        cudaMemcpy(dev_sm_te1, TBox0, 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te2, TBox1, 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te3, TBox2, 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te4, TBox3, 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
    }
    else {
        cudaMemcpy(dev_sm_te1, TBoxi0, 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te2, TBoxi1, 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te3, TBoxi2, 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_sm_te4, TBoxi3, 256*sizeof(uint32_t), cudaMemcpyHostToDevice);
    }
    cudaMemcpy(devInput  , inputData, filesize*sizeof(uint32_t), cudaMemcpyHostToDevice);
    

    if(mode) {
        for(int i=0; i < warm_up_device ; i++) {						
            encrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_te1,
            dev_sm_te2, dev_sm_te3, dev_sm_te4);
	    }
    }
    else {
        for(int i=0; i < warm_up_device ; i++) {						
            decrypt_Kernel<<<blockNum,threadNum>>>(devInput, devOutput, filesize, dev_sm_te1,
            dev_sm_te2, dev_sm_te3, dev_sm_te4);
	    }
    }

    //Copy results from host memory to device memory
	cudaMemcpy(outputData, devOutput, (filesize-padElmt)*sizeof(uint32_t), cudaMemcpyDeviceToHost);


    //
    ifstream outputFile("res", ios::binary);
    if(!outputFile) {
        cerr << "Read error";
        exit(1);
    }
    outputData >> outputFile;

    // Free device memory
    cudaFree(devInput);
    cudaFree(devOutput);
    cudaFree(dev_sm_te1);
    cudaFree(dev_sm_te2);
    cudaFree(dev_sm_te3);
    cudaFree(dev_sm_te4);
    


	return 0;

}
