
/***************************************************************************
 *   Copyright (C) 2006                                                    *
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


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


#define BENCH_ON




extern "C" void aesEncryptHandler128(unsigned *d_Result, unsigned *d_Input, int inputSize) {

	dim3  threads(BSIZE, 1);
    dim3  grid((inputSize/BSIZE)/4, 1);

	aesEncrypt128<<< grid, threads >>>( d_Result, d_Input, inputSize);
    CUDA_SAFE_CALL( cudaDeviceSynchronize() );
}

extern "C" void aesDecryptHandler128(unsigned *d_Result, unsigned *d_Input, int inputSize) {

	dim3  threads(BSIZE, 1);
    dim3  grid((inputSize/BSIZE)/4, 1);

	aesDecrypt128<<< grid, threads >>>( d_Result, d_Input, inputSize);
    CUDA_SAFE_CALL( cudaDeviceSynchronize() );
}

extern "C" void aesEncryptHandler256(unsigned *d_Result, unsigned *d_Input, int inputSize) {

	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	dim3  threads(BSIZE, 1);
    dim3  grid((inputSize/BSIZE)/4, 1);


	for(int i = 0; i < 10; i++){
		aesEncrypt256<<< grid, threads >>>( d_Result, d_Input, inputSize); //warmup
	}


	cudaEventRecord(start);
	aesEncrypt256<<< grid, threads >>>( d_Result, d_Input, inputSize); 
	cudaEventRecord(stop);	
	
    CUDA_SAFE_CALL( cudaDeviceSynchronize() );
	
	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);
	printf("GPU, other method processing time: %f (ms)\n", milliseconds);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);

}

extern "C" void aesDecryptHandler256(unsigned *d_Result, unsigned *d_Input, int inputSize) {

	dim3  threads(BSIZE, 1);
    dim3  grid((inputSize/BSIZE)/4, 1);

	aesDecrypt256<<< grid, threads >>>( d_Result, d_Input, inputSize);
    CUDA_SAFE_CALL( cudaDeviceSynchronize() );
}


extern "C" int aesHost(unsigned char* result, const unsigned char* inData, int inputSize, const unsigned char* key, int keySize, bool toEncrypt)
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
		
		/*
		//Device informations
		printf("Device Number: %d\n", dev);
		printf("  Device name: %s\n", deviceProp.name);
		printf("  Memory Clock Rate (KHz): %d\n",
		       deviceProp.memoryClockRate);
		printf("  Memory Bus Width (bits): %d\n",
		       deviceProp.memoryBusWidth);
		printf("  Peak Memory Bandwidth (GB/s): %f\n\n",
		       2.0*deviceProp.memoryClockRate*(deviceProp.memoryBusWidth/8)/1.0e6);
		printf("  Total global memory (bytes): %ld\n",
		       deviceProp.totalGlobalMem);
		printf("  Total constant memory (bytes): %d\n",
		       deviceProp.totalConstMem);
		*/

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
	//We replace it with following lines 
	StopWatchInterface *int_timer = NULL;
	CUT_SAFE_CALL( sdkCreateTimer(&int_timer) );
	CUT_SAFE_CALL( sdkStartTimer(&int_timer) );

	/*
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	
	cudaEventRecord(start);
	*/

	if (!toEncrypt) {
		#ifndef BENCH_ON
			printf("\nDECRYPTION.....\n\n");
		#endif //#ifndef BENCH_ON
		if (keySize != 240)
			aesDecryptHandler128( d_Result, d_Input, inputSize);
		else
			aesDecryptHandler256( d_Result, d_Input, inputSize);
	} else {
		#ifndef BENCH_ON
			printf("\nENCRYPTION.....\n\n");
		#endif //#ifndef BENCH_ON
		if (keySize != 240)
			aesEncryptHandler128( d_Result, d_Input, inputSize);
		else
			aesEncryptHandler256( d_Result, d_Input, inputSize);
	}
	
	/*
	//Outdated
	CUT_SAFE_CALL(cutStopTimer(int_timer));
    printf("GPU processing time: %f (ms)\n", cutGetTimerValue(int_timer));
    CUT_SAFE_CALL(cutDeleteTimer(int_timer));
	*/
	//Replacement with followings lines
	CUT_SAFE_CALL(sdkStopTimer(&int_timer));
    printf("GPU processing time: %f (ms)\n", sdkGetTimerValue(&int_timer));
    CUT_SAFE_CALL(sdkDeleteTimer(&int_timer));
	
	//cudaEventRecord(stop);

    // check if kernel execution generated and error
    CUT_CHECK_ERROR("Kernel execution failed");

    // copy result from device to host
    CUDA_SAFE_CALL(cudaMemcpy(result, d_Result, size_Result, cudaMemcpyDeviceToHost) );
	

	/*cudaEventSynchronize(stop);
	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);
	printf("GPU, other method processing time: %f (ms)\n", milliseconds);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	*/

	/*
	//Outdated
    CUT_SAFE_CALL(cutStopTimer(ext_timer));
    printf("Total processing time: %f (ms)\n\n", cutGetTimerValue(ext_timer));
    CUT_SAFE_CALL(cutDeleteTimer(ext_timer));
	*/
	//Replacement with followings lines
	CUT_SAFE_CALL(sdkStopTimer(&ext_timer));
    printf("Total processing time: %f (ms)\n\n", sdkGetTimerValue(&ext_timer));
    CUT_SAFE_CALL(sdkDeleteTimer(&ext_timer));

    // cleanup memory
    CUDA_SAFE_CALL(cudaFree(d_Input));
    CUDA_SAFE_CALL(cudaFree(d_Key));
    CUDA_SAFE_CALL(cudaFree(d_Result));

    return 0;
}

