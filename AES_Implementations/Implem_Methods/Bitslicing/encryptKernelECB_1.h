#ifndef _ENCRYPT_ECB_H_
#define _ENCRYPT_ECB_H_

#include "BtsUtils.h"
#include "addRoundKey.h"
#include "subBytes.h"
#include "shiftRows.h"
#include "mixColumns.h"

//__device__ inline void shiftRows(uint32_t shared[128])  __attribute__((always_inline));

__global__ void encrypt_Kernel( uint32_t* dev_input, uint32_t* dev_output, size_t inputSize)
{
    // Index calculations
    int tid         = threadIdx.x;                  //** Local id
    int global_tid  = blockIdx.x*blockDim.x+tid;    //** Global id
    
    int warps  = (blockDim.x*gridDim.x)/warpSize;   //** Warps in the grid
    int global_warpID = global_tid/warpSize;
    int local_warpID  = tid/warpSize;
    int local_laneID  = tid%warpSize;
    int elements = inputSize/16384;                 //** One warp treat 16384 bytes
    
    // store the expended key in shared memory.
    //__shared__ uint32_t sm_key[1408];
    __shared__ uint32_t plain[8192]; 

    /*for(int i = tid; i < 1408; i += block_width) {
        //load the extended key into the shared memory
        sm_key[i]   = const_expkey[i];
    }
    __syncthreads(); */

    for(int i = global_warpID; i < elements; i += warps) {
        //Loading plaintext
        for(int k=0; k<128; k++){
            int l = 32*k+local_laneID;
            plain[local_warpID*4096+l] = dev_input[i*4096+l];
        }
       
        // First round AddRoundKey: ex-or with round key
        addRoundKey(&plain[tid*128], const_expkey);

        // Round transformation: a set of table lookups operations.
       #pragma unroll
        for (int j = 1; j < 10; j++) {
            //subBytes
            Sbox( &plain[tid*128+0]  );
            Sbox( &plain[tid*128+8]  );
            Sbox( &plain[tid*128+16] );
            Sbox( &plain[tid*128+24] );
            Sbox( &plain[tid*128+32] );
            Sbox( &plain[tid*128+40] );
            Sbox( &plain[tid*128+48] );
            Sbox( &plain[tid*128+56] );
            Sbox( &plain[tid*128+64] );
            Sbox( &plain[tid*128+72] );
            Sbox( &plain[tid*128+80] );
            Sbox( &plain[tid*128+88] );
            Sbox( &plain[tid*128+96] );
            Sbox( &plain[tid*128+104]);
            Sbox( &plain[tid*128+112]);
            Sbox( &plain[tid*128+120]);
            
            //ShiftRows
            shiftRows(&plain[tid*128]);
            //Mixcolums
            Mixcl(&plain[tid*128+0]);
            Mixcl(&plain[tid*128+32]);
            Mixcl(&plain[tid*128+64]);
            Mixcl(&plain[tid*128+96]);
            
            //AddRoundKey
            addRoundKey(&plain[tid*128], &const_expkey[j*128]);//&sm_key[j*128]);
        } 

        // The final round doesn’t include the MixColumns step
        //subBytes
        Sbox( &plain[tid*128+0]  );
        Sbox( &plain[tid*128+8]  );
        Sbox( &plain[tid*128+16] );
        Sbox( &plain[tid*128+24] );
        Sbox( &plain[tid*128+32] );
        Sbox( &plain[tid*128+40] );
        Sbox( &plain[tid*128+48] );
        Sbox( &plain[tid*128+56] );
        Sbox( &plain[tid*128+64] );
        Sbox( &plain[tid*128+72] );
        Sbox( &plain[tid*128+80] );
        Sbox( &plain[tid*128+88] );
        Sbox( &plain[tid*128+96] );
        Sbox( &plain[tid*128+104]);
        Sbox( &plain[tid*128+112]);
        Sbox( &plain[tid*128+120]);

        //ShiftRows
        shiftRows(&plain[tid*128]);

        //AddRoundKey
        addRoundKey(&plain[tid*128], &const_expkey[1280]);//&sm_key[1280]);

        //output
        for(int k=0; k<128; k++){
            int l = 32*k+local_laneID;
            dev_output[i*4096+l] = plain[local_warpID*4096+l];
        }
    }
}

#endif //_ENCRYPT_ECB_H_