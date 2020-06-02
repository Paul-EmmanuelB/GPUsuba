#ifndef _ENCRYPT_ECB_H_
#define _ENCRYPT_ECB_H_

#include "utils.h"

__global__ static void encrypt_Kernel( uint4* dev_input, uint4* dev_output, 
size_t stateBlocks, uint32_t* dev_sm_te1, uint32_t* dev_sm_te2, uint32_t* dev_sm_te3, uint32_t* dev_sm_te4,  uint8_t* dev_sm_sbox)
{
    // Index calculations
    int tid         = threadIdx.y*blockDim.x + threadIdx.x;     //local id
    int x           = blockIdx.x * blockDim.x + threadIdx.x;    //global x id
    int y           = blockIdx.y * blockDim.y + threadIdx.y;    //global y id
    int width       = blockDim.x * gridDim.x;                   //width of the grid
    int global_tid  = y*width + x;                                  //global id

    int blockSize = blockDim.x * blockDim.y; 
    
    uint4 w, s;
    int ROUNDS = 11;

    // store the T-boxes and sbox in shared memory.
    __shared__ uint32_t sm_te1[256], sm_te2[256], sm_te3[256], sm_te4[256];
    __shared__ uint8_t sm_sbox[256];
    
    // Loading shared memory. 256 elements are needed
    int elemPerThread = 256/blockSize;
    
    if ( !elemPerThread && tid<256) {
        //load dev_sm_te1, dev_sm_te2, dev_sm_te3, dev_sm_te4 and
        // sm_sbox to share memory variables sm_te1, sm_te2,
        //sm_te3, sm_te4 and sm_sbox;
        sm_te1[tid]   = dev_sm_te1[tid];
        sm_te2[tid]   = dev_sm_te2[tid];
        sm_te3[tid]   = dev_sm_te3[tid];
        sm_te4[tid]   = dev_sm_te4[tid];
        sm_sbox[tid]  = dev_sm_sbox[tid];
    }
    else {
        for(int i=0; i<elemPerThread; i++) {
            sm_te1[tid*elemPerThread  + i]   = dev_sm_te1[tid*elemPerThread + i];
            sm_te2[tid*elemPerThread  + i]   = dev_sm_te2[tid*elemPerThread + i];
            sm_te3[tid*elemPerThread  + i]   = dev_sm_te3[tid*elemPerThread + i];
            sm_te4[tid*elemPerThread  + i]   = dev_sm_te4[tid*elemPerThread + i];
            sm_sbox[tid*elemPerThread + i]   = dev_sm_sbox[tid*elemPerThread + i];
        }
        int modEPT = 256%blockSize; //256 is not a multiple of blockSize
        if(!modEPT && (tid == blockSize-1)) {
            for(int i=0; i<modEPT; i++) {
                sm_te1[tid*(elemPerThread+1)  + i]   = dev_sm_te1[tid*(elemPerThread+1) + i];
                sm_te2[tid*(elemPerThread+1)  + i]   = dev_sm_te2[tid*(elemPerThread+1) + i];
                sm_te3[tid*(elemPerThread+1)  + i]   = dev_sm_te3[tid*(elemPerThread+1) + i];
                sm_te4[tid*(elemPerThread+1)  + i]   = dev_sm_te4[tid*(elemPerThread+1) + i];
                sm_sbox[tid*(elemPerThread+1) + i]   = dev_sm_sbox[tid*(elemPerThread+1) + i];
            }
        }
    }
    __syncthreads();

    // Each thread treat 16 bytes. 
    if(global_tid < stateBlocks) {
        //Loading plaintext
        w.x = dev_input[global_tid].x;
        w.y = dev_input[global_tid].y;
        w.z = dev_input[global_tid].z;
        w.w = dev_input[global_tid].w;

        // First round AddRoundKey: ex-or with round key
        w.x ^= const_expkey[0];
        w.y ^= const_expkey[1];
        w.z ^= const_expkey[2];
        w.w ^= const_expkey[3];

        // Round transformation: a set of table lookups operations.
        #pragma unroll
        for (int i = 1; i < 10; i++) {
            s.x = (sm_te4[(w.w >> 24)] ^ sm_te3[(w.z >> 16) & 0xFF] ^ sm_te2[(w.y >> 8) & 0xFF] ^ sm_te1[w.x & 0xFF]);
            s.y = (sm_te4[(w.x >> 24)] ^ sm_te3[(w.w >> 16) & 0xFF] ^ sm_te2[(w.z >> 8) & 0xFF] ^ sm_te1[w.y & 0xFF]);
            s.z = (sm_te4[(w.y >> 24)] ^ sm_te3[(w.x >> 16) & 0xFF] ^ sm_te2[(w.w >> 8) & 0xFF] ^ sm_te1[w.z & 0xFF]);
            s.w = (sm_te4[(w.z >> 24)] ^ sm_te3[(w.y >> 16) & 0xFF] ^ sm_te2[(w.x >> 8) & 0xFF] ^ sm_te1[w.w & 0xFF]);

            w.x = s.x ^ const_expkey[i * 4];
            w.y = s.y ^ const_expkey[i * 4 + 1];
            w.z = s.z ^ const_expkey[i * 4 + 2];
            w.w = s.w ^ const_expkey[i * 4 + 3];	
        } 

        // The final round doesn’t include the MixColumns
        s.x  = (uint32_t)(sm_sbox[ w.x        & 0xFF]);
        s.x |= (uint32_t)(sm_sbox[(w.y >>  8) & 0xFF]) << 8;
        s.x |= (uint32_t)(sm_sbox[(w.z >> 16) & 0xFF]) << 16;
        s.x |= (uint32_t)(sm_sbox[(w.w >> 24)       ]) << 24; //SubBytes and ShiftRows
        s.x ^= const_expkey[(ROUNDS - 1) * 4]; //AddRoundKey
        dev_output[global_tid].x = s.x ; //store the cipher text
        
        s.y  = (uint32_t)(sm_sbox[ w.y        & 0xFF]);
        s.y |= (uint32_t)(sm_sbox[(w.z >>  8) & 0xFF]) << 8;
        s.y |= (uint32_t)(sm_sbox[(w.w >> 16) & 0xFF]) << 16;
        s.y |= (uint32_t)(sm_sbox[(w.x >> 24)       ]) << 24; //SubBytes and ShiftRows
        s.y ^= const_expkey[(ROUNDS - 1) * 4 + 1]; //AddRoundKey
        dev_output[global_tid].y = s.y ; //store the cipher text

        s.z  = (uint32_t)(sm_sbox[ w.z        & 0xFF]);
        s.z |= (uint32_t)(sm_sbox[(w.w >>  8) & 0xFF]) << 8;
        s.z |= (uint32_t)(sm_sbox[(w.x >> 16) & 0xFF]) << 16;
        s.z |= (uint32_t)(sm_sbox[(w.y >> 24)       ]) << 24; //SubBytes and ShiftRows
        s.z ^= const_expkey[(ROUNDS - 1) * 4 + 2]; //AddRoundKey
        dev_output[global_tid].z = s.z ; //store the cipher text

        s.w  = (uint32_t)(sm_sbox[ w.w        & 0xFF]);
        s.w |= (uint32_t)(sm_sbox[(w.x >>  8) & 0xFF]) << 8;
        s.w |= (uint32_t)(sm_sbox[(w.y >> 16) & 0xFF]) << 16;
        s.w |= (uint32_t)(sm_sbox[(w.z >> 24)       ]) << 24; //SubBytes and ShiftRows
        s.w ^= const_expkey[(ROUNDS - 1) * 4 + 3]; //AddRoundKey
        dev_output[global_tid].w = s.w ; //store the cipher text
    }
}

#endif //_ENCRYPT_ECB_H_