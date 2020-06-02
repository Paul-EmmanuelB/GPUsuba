#ifndef _ENCRYPT_ECB_H_
#define _ENCRYPT_ECB_H_

#include "utils.h"

__global__ static void encrypt_Kernel( uint32_t* dev_input, uint32_t* dev_output, size_t inputSize, uint8_t* dev_sm_sbox,
cudaTextureObject_t tex0, cudaTextureObject_t tex1, cudaTextureObject_t tex2, cudaTextureObject_t tex3)
{
    // Index calculations
    int tid         = threadIdx.y*blockDim.x + threadIdx.x;     //local id
    int x           = blockIdx.x * blockDim.x + threadIdx.x;    //global x id
    int y           = blockIdx.y * blockDim.y + threadIdx.y;    //global y id
    int w           = blockDim.x * gridDim.x;                   //width of the grid
    int global_tid  = y*w + x;                                  //global id

    int blockSize = blockDim.x * blockDim.y; 
    
    uint32_t w1,w2,w3,w4,s1,s2,s3,s4;
    int ROUNDS = 11;

    // store the T-boxes and sbox in shared memory.
    __shared__ uint32_t sm_te0[256], sm_te1[256], sm_te2[256], sm_te3[256];
    __shared__ uint8_t sm_sbox[256];
    
    // Loading shared memory. 256 elements are needed
    int elemPerThread = 256/blockSize;
    

    if ( !elemPerThread && tid<256) {
        //load dev_sm_te0, dev_sm_te1, dev_sm_te2, dev_sm_te3 and
        // sm_sbox to share memory variables sm_te0, sm_te1,
        //sm_te2, sm_te3 and sm_sbox;
        sm_te0[tid]   = tex1Dfetch<uint32>(tex0, tid);
        sm_te1[tid]   = tex1Dfetch<uint32>(tex1, tid);
        sm_te2[tid]   = tex1Dfetch<uint32>(tex2, tid);
        sm_te3[tid]   = tex1Dfetch<uint32>(tex3, tid);
        sm_sbox[tid]  = dev_sm_sbox[tid];
    }

    // Each thread treat 16 bytes. 
    if(global_tid < inputSize/16) {
        //Loading plaintext
        w1 = dev_input[4*global_tid];
        w2 = dev_input[4*global_tid+1];
        w3 = dev_input[4*global_tid+2];
        w4 = dev_input[4*global_tid+3];

        // First round AddRoundKey: ex-or with round key
        w1 ^= const_expkey[0];
        w2 ^= const_expkey[1];
        w3 ^= const_expkey[2];
        w4 ^= const_expkey[3];

        // Round transformation: a set of table lookups operations.

        __syncthreads();
        #pragma unroll
        for (int i = 1; i < 10; i++) {
            s1 = (sm_te3[(w4 >> 24)] ^ sm_te2[(w3 >> 16) & 0xFF] ^ sm_te1[(w2 >> 8) & 0xFF] ^ sm_te0[w1 & 0xFF]);
            s2 = (sm_te3[(w1 >> 24)] ^ sm_te2[(w4 >> 16) & 0xFF] ^ sm_te1[(w3 >> 8) & 0xFF] ^ sm_te0[w2 & 0xFF]);
            s3 = (sm_te3[(w2 >> 24)] ^ sm_te2[(w1 >> 16) & 0xFF] ^ sm_te1[(w4 >> 8) & 0xFF] ^ sm_te0[w3 & 0xFF]);
            s4 = (sm_te3[(w3 >> 24)] ^ sm_te2[(w2 >> 16) & 0xFF] ^ sm_te1[(w1 >> 8) & 0xFF] ^ sm_te0[w4 & 0xFF]);

            w1 = s1 ^ const_expkey[i * 4];
            w2 = s2 ^ const_expkey[i * 4 + 1];
            w3 = s3 ^ const_expkey[i * 4 + 2];
            w4 = s4 ^ const_expkey[i * 4 + 3];	
        } 

        // The final round doesn’t include the MixColumns
        s1  = (uint32_t)(sm_sbox[ w1        & 0xFF]);
        s1 |= (uint32_t)(sm_sbox[(w2 >>  8) & 0xFF]) << 8;
        s1 |= (uint32_t)(sm_sbox[(w3 >> 16) & 0xFF]) << 16;
        s1 |= (uint32_t)(sm_sbox[(w4 >> 24)       ]) << 24; //SubBytes and ShiftRows
        s1 ^= const_expkey[(ROUNDS - 1) * 4]; //AddRoundKey
        dev_output[4*global_tid] = s1 ; //store the cipher text
        
        s2  = (uint32_t)(sm_sbox[ w2        & 0xFF]);
        s2 |= (uint32_t)(sm_sbox[(w3 >>  8) & 0xFF]) << 8;
        s2 |= (uint32_t)(sm_sbox[(w4 >> 16) & 0xFF]) << 16;
        s2 |= (uint32_t)(sm_sbox[(w1 >> 24)       ]) << 24; //SubBytes and ShiftRows
        s2 ^= const_expkey[(ROUNDS - 1) * 4 + 1]; //AddRoundKey
        dev_output[4*global_tid+1] = s2 ; //store the cipher text

        s3  = (uint32_t)(sm_sbox[ w3        & 0xFF]);
        s3 |= (uint32_t)(sm_sbox[(w4 >>  8) & 0xFF]) << 8;
        s3 |= (uint32_t)(sm_sbox[(w1 >> 16) & 0xFF]) << 16;
        s3 |= (uint32_t)(sm_sbox[(w2 >> 24)       ]) << 24; //SubBytes and ShiftRows
        s3 ^= const_expkey[(ROUNDS - 1) * 4 + 2]; //AddRoundKey
        dev_output[4*global_tid+2] = s3 ; //store the cipher text

        s4  = (uint32_t)(sm_sbox[ w4        & 0xFF]);
        s4 |= (uint32_t)(sm_sbox[(w1 >>  8) & 0xFF]) << 8;
        s4 |= (uint32_t)(sm_sbox[(w2 >> 16) & 0xFF]) << 16;
        s4 |= (uint32_t)(sm_sbox[(w3 >> 24)       ]) << 24; //SubBytes and ShiftRows
        s4 ^= const_expkey[(ROUNDS - 1) * 4 + 3]; //AddRoundKey
        dev_output[4*global_tid+3] = s4 ; //store the cipher text
    }
}

#endif //_ENCRYPT_ECB_H_