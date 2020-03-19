
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
	@original author Svetlin Manavski <svetlin@manavski.com>
	@updated_version author Broux Paul-Emmanuel <paulemmanuelb@gmail.com>
*/

// includes, system
#include <iostream>
#include <fstream>
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <math.h>


//#include <cutil.h> Outdated, we have to find an alternative to all concerned functions
//#include <helper_cuda.h> // It can help to handle some errors
#include <helper_timer.h> // To replace timers

#include <cuda_runtime_api.h> //Recommended API for a lot of functions


// includes, project
#include "sbox_E.h"
#include "sbox_D.h"
#include "util.h"
#include <aesEncrypt128_kernel.h>
#include <aesDecrypt128_kernel.h>
#include <aesEncrypt256_kernel.h>
#include <aesDecrypt256_kernel.h>

using namespace std;


extern "C" void aesEncryptHandler128(unsigned *d_Result, unsigned *d_Input, int inputSize, cudaEvent_t *start, cudaEvent_t *stop, cudaEvent_t *warm, int warm_up_device) {

	dim3  threads(BSIZE, 1);
    dim3  grid((inputSize/BSIZE)/4, 1);

	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(*warm));
	for(int i = 0; i < warm_up_device; i++){
		aesEncrypt128<<< grid, threads >>>( d_Result, d_Input, inputSize); //warmup
	}

	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(*start));
	aesEncrypt128<<< grid, threads >>>( d_Result, d_Input, inputSize); 
	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(*stop));

	CUDA_SAFE_CALL_NO_SYNC(cudaEventSynchronize(*stop));
    CUDA_SAFE_CALL( cudaDeviceSynchronize() );
}

extern "C" void aesDecryptHandler128(unsigned *d_Result, unsigned *d_Input, int inputSize, cudaEvent_t *start, cudaEvent_t *stop, cudaEvent_t *warm, int warm_up_device) {

	dim3  threads(BSIZE, 1);
    dim3  grid((inputSize/BSIZE)/4, 1);

	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(*warm));
	for(int i = 0; i < warm_up_device; i++){
		aesDecrypt128<<< grid, threads >>>( d_Result, d_Input, inputSize); //warmup
	}

	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(*start));
	aesDecrypt128<<< grid, threads >>>( d_Result, d_Input, inputSize); 
	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(*stop));

	CUDA_SAFE_CALL_NO_SYNC(cudaEventSynchronize(*stop));
    CUDA_SAFE_CALL( cudaDeviceSynchronize() );
}

extern "C" void aesEncryptHandler256(unsigned *d_Result, unsigned *d_Input, int inputSize, cudaEvent_t *start, cudaEvent_t *stop, cudaEvent_t *warm, int warm_up_device) {


	dim3  threads(BSIZE, 1);
    dim3  grid((inputSize/BSIZE)/4, 1);

	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(*warm));
	for(int i = 0; i < warm_up_device; i++){
		aesEncrypt256<<< grid, threads >>>( d_Result, d_Input, inputSize); //warmup
	}


	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(*start));
	aesEncrypt256<<< grid, threads >>>( d_Result, d_Input, inputSize); 
	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(*stop));	
	
	CUDA_SAFE_CALL_NO_SYNC(cudaEventSynchronize(*stop));
    CUDA_SAFE_CALL( cudaDeviceSynchronize() );
}

extern "C" void aesDecryptHandler256(unsigned *d_Result, unsigned *d_Input, int inputSize, cudaEvent_t *start, cudaEvent_t *stop, cudaEvent_t *warm, int warm_up_device) {

	
	dim3  threads(BSIZE, 1);
    dim3  grid((inputSize/BSIZE)/4, 1);

	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(*warm));
	for(int i = 0; i < warm_up_device; i++){
		aesDecrypt256<<< grid, threads >>>( d_Result, d_Input, inputSize); //warmup
	}

	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(*start));
	aesDecrypt256<<< grid, threads >>>( d_Result, d_Input, inputSize); 
	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(*stop));	
	
	CUDA_SAFE_CALL_NO_SYNC(cudaEventSynchronize(*stop));
    CUDA_SAFE_CALL( cudaDeviceSynchronize() );
}


extern "C" int aesHost(unsigned char* result, const unsigned char* inData, int inputSize, const unsigned char* key, int keySize, bool toEncrypt, int bench_mode, int warm_up_device)
{
	if (inputSize < 256) 
		return -1;
	if (inputSize % 256 > 0) 
		return -11;
	if (keySize != 240 && keySize != 176) 
		return -2;
	if (!result || !inData || !key)
		return -3;

    int deviceCount;                                                         
    CUDA_SAFE_CALL_NO_SYNC(cudaGetDeviceCount(&deviceCount));            
    if (deviceCount == 0) {                                                  
        fprintf(stderr, "There is no device.\n");                            
        exit(EXIT_FAILURE);                                                  
    }                                                                        
    int dev;                                                                 
    for (dev = 0; dev < deviceCount; ++dev) {                                
        cudaDeviceProp deviceProp;                                           
        CUDA_SAFE_CALL_NO_SYNC(cudaGetDeviceProperties(&deviceProp, dev));

        if (deviceProp.major >= 1)                                           
            break;                                                           
    }                                                                        
    if (dev == deviceCount) {                                                
        fprintf(stderr, "There is no device supporting CUDA.\n");            
        exit(EXIT_FAILURE);                                                  
    }                                                                        
    else                                                                     
        CUDA_SAFE_CALL(cudaSetDevice(dev));                                  


    // allocate device memory
    unsigned * d_Input;
    CUDA_SAFE_CALL( cudaMalloc((void**) &d_Input, inputSize) );

	// the size of the memory for the key must be equal to keySize (every thread copies one key byte to shared memory)
    unsigned * d_Key;
    CUDA_SAFE_CALL( cudaMalloc((void**) &d_Key, keySize) );

	/*
	//Outdated code
	unsigned int ext_timer = 0;
    CUT_SAFE_CALL(cutCreateTimer(&ext_timer));
    CUT_SAFE_CALL(cutStartTimer(ext_timer));
	*/
	//We replace it with following lines 
	StopWatchInterface *ext_timer = NULL;
	CUT_SAFE_CALL( sdkCreateTimer(&ext_timer) );
	CUT_SAFE_CALL( sdkStartTimer(&ext_timer) );

	cudaEvent_t startHost, stopHost;
	CUDA_SAFE_CALL_NO_SYNC(cudaEventCreate(&startHost));
	CUDA_SAFE_CALL_NO_SYNC(cudaEventCreate(&stopHost));
	CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(startHost));


    // copy host memory to device
    CUDA_SAFE_CALL( cudaMemcpy(d_Input, inData, inputSize, cudaMemcpyHostToDevice) );
    CUDA_SAFE_CALL( cudaMemcpy(d_Key, key, keySize, cudaMemcpyHostToDevice) );

	//texture
	cudaChannelFormatDesc chDesc;
	chDesc.x = 32;
	chDesc.y = 0;
	chDesc.z = 0;
	chDesc.w = 0;
	chDesc.f = cudaChannelFormatKindUnsigned;
	texEKey.normalized = false;
	texDKey.normalized = false;
	texEKey128.normalized = false;
	texDKey128.normalized = false;

	CUDA_SAFE_CALL( cudaBindTexture( 0, &texEKey128, d_Key, &chDesc, (size_t)keySize) );
	CUDA_SAFE_CALL( cudaBindTexture( 0, &texDKey128, d_Key, &chDesc, (size_t)keySize) );
	CUDA_SAFE_CALL( cudaBindTexture( 0, &texEKey, d_Key, &chDesc, (size_t)keySize) );
	CUDA_SAFE_CALL( cudaBindTexture( 0, &texDKey, d_Key, &chDesc, (size_t)keySize) );

    // allocate device memory for result
    unsigned int size_Result = inputSize;
    unsigned * d_Result;
    CUDA_SAFE_CALL( cudaMalloc((void**) &d_Result, size_Result) );
	CUDA_SAFE_CALL( cudaMemset(d_Result, 0, size_Result) );
	
	/*
	// Outdated
	unsigned int int_timer = 0;
    CUT_SAFE_CALL(cutCreateTimer(&int_timer));
    CUT_SAFE_CALL(cutStartTimer(int_timer));
	*/
	//We replace it with following lines for timing.	
	cudaEvent_t startDevice, stopDevice, warmUp;
	CUDA_SAFE_CALL_NO_SYNC(cudaEventCreate(&startDevice));
	CUDA_SAFE_CALL_NO_SYNC(cudaEventCreate(&stopDevice));
	CUDA_SAFE_CALL_NO_SYNC(cudaEventCreate(&warmUp));


	//Encryption or decryption
	if (!toEncrypt) {
		if(!bench_mode)
			printf("\nDECRYPTION.....\n\n");
		
		if (keySize != 240)
			aesDecryptHandler128( d_Result, d_Input, inputSize, &startDevice, &stopDevice, &warmUp, warm_up_device);
		else
			aesDecryptHandler256( d_Result, d_Input, inputSize, &startDevice, &stopDevice, &warmUp, warm_up_device);
	
	} else {
		if(!bench_mode)
			printf("\nENCRYPTION.....\n\n");

		if (keySize != 240)
			aesEncryptHandler128( d_Result, d_Input, inputSize, &startDevice, &stopDevice, &warmUp, warm_up_device);
		else
			aesEncryptHandler256( d_Result, d_Input, inputSize, &startDevice, &stopDevice, &warmUp, warm_up_device);
	}
	
	/*
	//Outdated
	CUT_SAFE_CALL(cutStopTimer(int_timer));
    printf("GPU processing time: %f (ms)\n", cutGetTimerValue(int_timer));
    CUT_SAFE_CALL(cutDeleteTimer(int_timer));
	*/
	//Replacement with followings lines fot timing.
	float milliseconds = 0;
	float millisecondsWarmUp = 0;	

	std::ofstream benchFile;
	if(bench_mode) {
		
		benchFile.open("bench/benchResult", ios::app);

		cudaEventElapsedTime(&milliseconds, startDevice, stopDevice);
		cudaEventElapsedTime(&millisecondsWarmUp, warmUp, startDevice);
		printf("	GPU processing time: %f (ms)\n", milliseconds);
		benchFile << "	BENCH RESULT for device's warm up of " << warm_up_device << std::endl;
		benchFile << "	GPU kernel execution  (ms)	" << milliseconds << std::endl;
	}

    // check if kernel execution generated and error
    CUT_CHECK_ERROR("Kernel execution failed");

    // copy result from device to host
    CUDA_SAFE_CALL(cudaMemcpy(result, d_Result, size_Result, cudaMemcpyDeviceToHost) );
	

	/*
	//Outdated
    CUT_SAFE_CALL(cutStopTimer(ext_timer));
    printf("Total processing time: %f (ms)\n\n", cutGetTimerValue(ext_timer));
    CUT_SAFE_CALL(cutDeleteTimer(ext_timer));
	*/
	//Replacement with followings lines for timing.
	if(bench_mode) {
		CUDA_SAFE_CALL_NO_SYNC(cudaEventRecord(stopHost));
		CUDA_SAFE_CALL_NO_SYNC(cudaEventSynchronize(stopHost));
		cudaEventElapsedTime(&milliseconds, startHost, stopHost);
		
	
		float totalProcessingTime = milliseconds - millisecondsWarmUp;
		printf("	Total processing time : %f (ms)\n\n", totalProcessingTime);
		//printf("	Warmup: %f (ms)\n\n", millisecondsWarmUp);
		benchFile << "	Total processing time  (ms)	" << totalProcessingTime << std::endl << std::endl;
	}

	//Deleting Clocks
	CUDA_SAFE_CALL_NO_SYNC(cudaEventDestroy(startDevice));
	CUDA_SAFE_CALL_NO_SYNC(cudaEventDestroy(stopDevice));
	CUDA_SAFE_CALL_NO_SYNC(cudaEventDestroy(warmUp));
	CUDA_SAFE_CALL_NO_SYNC(cudaEventDestroy(startHost));
	CUDA_SAFE_CALL_NO_SYNC(cudaEventDestroy(stopHost));

    // cleanup memory
    CUDA_SAFE_CALL(cudaFree(d_Input));
    CUDA_SAFE_CALL(cudaFree(d_Key));
    CUDA_SAFE_CALL(cudaFree(d_Result));

    return 0;
}

