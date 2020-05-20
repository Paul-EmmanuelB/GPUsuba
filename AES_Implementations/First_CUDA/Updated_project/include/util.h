
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


#ifndef _UTIL_H_
#define _UTIL_H_


#include <cuda.h>

// Macro to catch CUDA errors in kernel launches
#define CHECK_LAUNCH_ERROR()                                          \
    do {                                                                  \
        /* Check synchronous errors, i.e. pre-launch */                   \
        cudaError_t err = cudaGetLastError();                             \
        if (cudaSuccess != err) {                                         \
            fprintf (stderr, "Cuda error in file '%s' in line %i : %s.\n",\
                     __FILE__, __LINE__, cudaGetErrorString(err) );       \
            exit(EXIT_FAILURE);                                           \
        }                                                                 \
        /* Check asynchronous errors, i.e. kernel failed (ULF) */         \
        err = cudaDeviceSynchronize();                                    \
        if (cudaSuccess != err) {                                         \
            fprintf (stderr, "Cuda error in file '%s' in line %i : %s!\n",\
                     __FILE__, __LINE__, cudaGetErrorString( err) );      \
            exit(EXIT_FAILURE);                                           \
        }                                                                 \
    } while (0)

// Macro to catch CUDA errors in CUDA runtime calls
#define CUDA_CHECK(call)                                              \
    do {                                                                  \
        cudaError_t err = call;                                           \
        if (cudaSuccess != err) {                                         \
            fprintf (stderr, "Cuda error in file '%s' in line %i : %s.\n",\
                     __FILE__, __LINE__, cudaGetErrorString(err) );       \
            exit(EXIT_FAILURE);                                           \
        }                                                                 \
    } while (0)


///////////////////////////////////////////////////////////////////////////////////
//	CUDA_SAFE_CALL
///////////////////////////////////////////////////////////////////////////////////


//Alternative for CUDA_SAFE_CALL_NO_SYNC
#define CUDA_SAFE_CALL_NO_SYNC(x)                                 \
  do {                                                            \
    cudaError_t err = x;			                              \
    if (err != cudaSuccess) {                                 	  \
		fprintf(stderr, "Cuda error in file '%s' in line %i : %s.\n",       \
		            __FILE__, __LINE__, cudaGetErrorString( err) );         \
		exit(EXIT_FAILURE);                                                 \
    }                                                             \
  } while(0)

//Alternative for CUDA_SAFE_CALL
#  define CUDA_SAFE_CALL( call)                                             \
    do {								  									\
		CUDA_SAFE_CALL_NO_SYNC(call);                                       \
		cudaError_t err = cudaDeviceSynchronize();                              \
		if( cudaSuccess != err) {                                               \
		    fprintf(stderr, "Cuda error in file '%s' in line %i : %s.\n",       \
		            __FILE__, __LINE__, cudaGetErrorString( err) );             \
		    exit(EXIT_FAILURE);                                                 \
    	}																		\
	} while (0)

/*
//An other way to do CUDA_SAFE_CALL_NO_SYNC
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}
*/



///////////////////////////////////////////////////////////////////////////////////
//	Cut functions checkers
///////////////////////////////////////////////////////////////////////////////////

#define CUT_BANK_CHECKER( array, index)  array[index]

//! Check for CUDA error
#ifdef _DEBUG
#  define CUT_CHECK_ERROR(errorMessage) {                	                 \
    cudaError_t err = cudaGetLastError();                                    \
    if( cudaSuccess != err) {                                                \
        fprintf(stderr, "Cuda error: %s in file '%s' in line %i : %s.\n",    \
                errorMessage, __FILE__, __LINE__, cudaGetErrorString( err) );\
        exit(EXIT_FAILURE);                                                  \
    }                                                                        \
    err = CUT_DEVICE_SYNCHRONIZE();                                          \
    if( cudaSuccess != err) {                                                \
        fprintf(stderr, "Cuda error: %s in file '%s' in line %i : %s.\n",    \
                errorMessage, __FILE__, __LINE__, cudaGetErrorString( err) );\
        exit(EXIT_FAILURE);                                                  \
    }                                                                        \
    } while(0)
#else
#  define CUT_CHECK_ERROR(errorMessage) {	                                 \
    cudaError_t err = cudaGetLastError();                                    \
    if( cudaSuccess != err) {                                                \
        fprintf(stderr, "Cuda error: %s in file '%s' in line %i : %s.\n",    \
                errorMessage, __FILE__, __LINE__, cudaGetErrorString( err) );\
        exit(EXIT_FAILURE);                                                  \
    }                                                                        \
    }
#endif


//Alternative for CUT_SAVE_CALL, we have the file helper_timer.h in cuda/samples/common/inc
# define CUT_SAFE_CALL( x )												\
    do {								  									\
    	bool err = x;                                          			\
    	if( true != err) {                                               \
        	fprintf(stderr, "Timer error in file '%s' in line %i.\n",       \
                __FILE__, __LINE__ );							          	    \
        	exit(EXIT_FAILURE);                                                 \
  		}																		\
	} while (0)


#endif //#ifndef _UTIL_H_
