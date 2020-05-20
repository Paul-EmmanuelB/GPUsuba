#ifndef _DECRYPT_ECB_H_
#define _DECRYPT_ECB_H_

#include "utils.h"

__global__ static void decrypt_Kernel( uint4* dev_input, uint4* dev_output, 
size_t stateBlocks, uint32_t* dev_sm_td1, uint32_t* dev_sm_td2, uint32_t* dev_sm_td3, uint32_t* dev_sm_td4, uint8_t* dev_sm_isbox_inv)
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
    __shared__ uint32_t sm_td1[256], sm_td2[256], sm_td3[256], sm_td4[256];
    __shared__ uint8_t sm_isbox[256];
    
    // Loading shared memory. 256 elements are needed
    int elemPerThread = 256/blockSize;

    if ( !elemPerThread && tid<256) {
        //load dev_sm_td1, dev_sm_td2, dev_sm_td3, dev_sm_td4 and
        // dev_sm_isbox_inv to share memory variables sm_td1, sm_td2,
        //sm_td3, sm_td4 and sm_isbox;
        sm_td1[tid]      = dev_sm_td1[tid];
        sm_td2[tid]      = dev_sm_td2[tid];
        sm_td3[tid]      = dev_sm_td3[tid];
        sm_td4[tid]      = dev_sm_td4[tid];
        sm_isbox[tid]    = dev_sm_isbox_inv[tid];
    }
    else {
        for(int i=0; i<elemPerThread; i++) {
            sm_td1[tid*elemPerThread  + i]   = dev_sm_td1[tid*elemPerThread + i];
            sm_td2[tid*elemPerThread  + i]   = dev_sm_td2[tid*elemPerThread + i];
            sm_td3[tid*elemPerThread  + i]   = dev_sm_td3[tid*elemPerThread + i];
            sm_td4[tid*elemPerThread  + i]   = dev_sm_td4[tid*elemPerThread + i];
            sm_isbox[tid*elemPerThread + i]   = dev_sm_isbox_inv[tid*elemPerThread + i];
        }
        int modEPT = 256%blockSize; //256 is not a multiple of blockSize
        if(!modEPT && (tid == blockSize-1)) {
            for(int i=0; i<modEPT; i++) {
                sm_td1[tid*(elemPerThread+1)  + i]   = dev_sm_td1[tid*(elemPerThread+1) + i];
                sm_td2[tid*(elemPerThread+1)  + i]   = dev_sm_td2[tid*(elemPerThread+1) + i];
                sm_td3[tid*(elemPerThread+1)  + i]   = dev_sm_td3[tid*(elemPerThread+1) + i];
                sm_td4[tid*(elemPerThread+1)  + i]   = dev_sm_td4[tid*(elemPerThread+1) + i];
                sm_isbox[tid*(elemPerThread+1) + i]   = dev_sm_isbox_inv[tid*(elemPerThread+1) + i];
            }
        }
    }
    __syncthreads();

    
    // Each thread treat 16 bytes. 
    if(global_tid < stateBlocks) {

        //load the cipher blocks, all the global memory transactions are
        //coalesced. The original plain text load from files, due to the read
        //procedure reverse the byte order of the 32-bit words, So a reverse
        //process was necessary.
        s.x  = dev_input[global_tid].x;
        s.y  = dev_input[global_tid].y;
        s.z  = dev_input[global_tid].z;
        s.w  = dev_input[global_tid].w;
        

        // First round
        s.x ^= const_expkey[(ROUNDS - 1) * 4]; //AddRoundKey
        s.y ^= const_expkey[(ROUNDS - 1) * 4 + 1]; //AddRoundKey
        s.z ^= const_expkey[(ROUNDS - 1) * 4 + 2]; //AddRoundKey
        s.w ^= const_expkey[(ROUNDS - 1) * 4 + 3]; //AddRoundKey

        // Round transformation: a set of table lookups operations.
        uint8_t *expkey = (uint8_t*)const_expkey;

        #pragma unroll
        for (int round = 9; round; round--) {
            w.x = (sm_td4[(s.y >> 24)] ^ sm_td3[(s.z >> 16) & 0xFF] ^ sm_td2[(s.w >> 8) & 0xFF] ^ sm_td1[s.x & 0xFF])
                ^ const_IK0[expkey[round*16]]
                ^ ((const_IK0[expkey[round * 16 + 1]] << 8) ^ (const_IK0[expkey[round * 16 + 1]] >> 24))
                ^ ((const_IK0[expkey[round * 16 + 2]] << 16) ^ (const_IK0[expkey[round * 16 + 2]] >> 16))
                ^ ((const_IK0[expkey[round * 16 + 3]] << 24) ^ (const_IK0[expkey[round * 16 + 3]] >> 8));
            w.y = (sm_td4[(s.z >> 24)] ^ sm_td3[(s.w >> 16) & 0xFF] ^ sm_td2[(s.x >> 8) & 0xFF] ^ sm_td1[s.y & 0xFF])
                ^ const_IK0[expkey[round * 16 + 4]]
                ^ ((const_IK0[expkey[round * 16 + 5]] << 8) ^ (const_IK0[expkey[round * 16 + 5]] >> 24))
                ^ ((const_IK0[expkey[round * 16 + 6]] << 16) ^ (const_IK0[expkey[round * 16 + 6]] >> 16))
                ^ ((const_IK0[expkey[round * 16 + 7]] << 24) ^ (const_IK0[expkey[round * 16 + 7]] >> 8));
            w.z = (sm_td4[(s.w >> 24)] ^ sm_td3[(s.x >> 16) & 0xFF] ^ sm_td2[(s.y >> 8) & 0xFF] ^ sm_td1[s.z & 0xFF])
                ^ const_IK0[expkey[round * 16 + 8]]
                ^ ((const_IK0[expkey[round * 16 + 9]] << 8) ^ (const_IK0[expkey[round * 16 + 9]] >> 24))
                ^ ((const_IK0[expkey[round * 16 + 10]] << 16) ^ (const_IK0[expkey[round * 16 + 10]] >> 16))
                ^ ((const_IK0[expkey[round * 16 + 11]] << 24) ^ (const_IK0[expkey[round * 16 + 11]] >> 8));
            w.w = (sm_td4[(s.x >> 24)] ^ sm_td3[(s.y >> 16) & 0xFF] ^ sm_td2[(s.z >> 8) & 0xFF] ^ sm_td1[s.w & 0xFF])
                ^ const_IK0[expkey[round * 16 + 12]]
                ^ ((const_IK0[expkey[round * 16 + 13]] << 8) ^ (const_IK0[expkey[round * 16 + 13]] >> 24))
                ^ ((const_IK0[expkey[round * 16 + 14]] << 16) ^ (const_IK0[expkey[round * 16 + 14]] >> 16))
                ^ ((const_IK0[expkey[round * 16 + 15]] << 24) ^ (const_IK0[expkey[round * 16 + 15]] >> 8));


            s.x = w.x;
            s.y = w.y;
            s.z = w.z;
            s.w = w.w;

        }

        w.x  = (uint32_t)(sm_isbox[ s.x & 0xFF]);
        w.x |= (uint32_t)(sm_isbox[(s.w >>  8) & 0xFF]) << 8;
        w.x |= (uint32_t)(sm_isbox[(s.z >> 16) & 0xFF]) << 16;
        w.x |= (uint32_t)(sm_isbox[(s.y >> 24)       ]) << 24; //SubBytes and ShiftRows
        w.x ^= const_expkey[0];
        dev_output[global_tid].x = w.x ; //store the cipher text

        w.y  = (uint32_t)(sm_isbox[ s.y & 0xFF]);
        w.y |= (uint32_t)(sm_isbox[(s.x >>  8) & 0xFF]) << 8;
        w.y |= (uint32_t)(sm_isbox[(s.w >> 16) & 0xFF]) << 16;
        w.y |= (uint32_t)(sm_isbox[(s.z >> 24)       ]) << 24; //SubBytes and ShiftRows
        w.y ^= const_expkey[1];
        dev_output[global_tid].y = w.y ; //store the cipher text

        w.z  = (uint32_t)(sm_isbox[ s.z & 0xFF]);
        w.z |= (uint32_t)(sm_isbox[(s.y >> 8) & 0xFF]) << 8;
        w.z |= (uint32_t)(sm_isbox[(s.x >> 16) & 0xFF]) << 16;
        w.z |= (uint32_t)(sm_isbox[(s.w >> 24)       ]) << 24; //SubBytes and ShiftRows
        w.z ^= const_expkey[2];
        dev_output[global_tid].z = w.z ; //store the cipher text

        w.w  = (uint32_t)(sm_isbox[ s.w & 0xFF]);
        w.w |= (uint32_t)(sm_isbox[(s.z >> 8) & 0xFF]) <<  8;
        w.w |= (uint32_t)(sm_isbox[(s.y >> 16) & 0xFF]) << 16;
        w.w |= (uint32_t)(sm_isbox[(s.x >> 24)       ]) << 24; //SubBytes and ShiftRows
        w.w ^= const_expkey[3];
        dev_output[global_tid].w = w.w ; //store the cipher text
    }

}

#endif //_DECRYPT_ECB_H_