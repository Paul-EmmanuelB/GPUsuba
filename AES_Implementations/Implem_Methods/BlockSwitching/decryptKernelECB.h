#ifndef _DECRYPT_ECB_H_
#define _DECRYPT_ECB_H_

#include "utils.h"

__global__ static void decrypt_Kernel( uint32_t* dev_input, uint32_t* dev_output, 
size_t inputSize, uint32_t* dev_sm_td1, uint32_t* dev_sm_td2, uint32_t* dev_sm_td3, uint32_t* dev_sm_td4, uint8_t* dev_sm_isbox_inv)
{
    // Index calculations
    int tid         = threadIdx.y*blockDim.x + threadIdx.x;     //local id
    int x           = blockIdx.x * blockDim.x + threadIdx.x;    //global x id
    int y           = blockIdx.y * blockDim.y + threadIdx.y;    //global y id
    int xwidth      = blockDim.x * gridDim.x;                   //X width of the grid
    int ywidth      = blockDim.y * gridDim.y;
    int global_tid  = y*xwidth + x;                                  //global id

    int blockSize = blockDim.x * blockDim.y; 
    
    uint32_t w1,w2,w3,w4,s1,s2,s3,s4;
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
    int elements = inputSize/16;
    int gridSize = xwidth*ywidth;

    for(int i = global_tid; i < elements; i += gridSize) {
        //Loading plaintext
        s1  = dev_input[4*i];
        s2  = dev_input[4*i+1];
        s3  = dev_input[4*i+2];
        s4  = dev_input[4*i+3];
        

        // First round
        s1 ^= const_expkey[(ROUNDS - 1) * 4]; //AddRoundKey
        s2 ^= const_expkey[(ROUNDS - 1) * 4 + 1]; //AddRoundKey
        s3 ^= const_expkey[(ROUNDS - 1) * 4 + 2]; //AddRoundKey
        s4 ^= const_expkey[(ROUNDS - 1) * 4 + 3]; //AddRoundKey

        // Round transformation: a set of table lookups operations.
        uint8_t *expkey = (uint8_t*)const_expkey;

        #pragma unroll
        for (int round = 9; round; round--) {
            w1 = (sm_td4[(s2 >> 24)] ^ sm_td3[(s3 >> 16) & 0xFF] ^ sm_td2[(s4 >> 8) & 0xFF] ^ sm_td1[s1 & 0xFF])
                ^ const_IK0[expkey[round*16]]
                ^ ((const_IK0[expkey[round * 16 + 1]] << 8) ^ (const_IK0[expkey[round * 16 + 1]] >> 24))
                ^ ((const_IK0[expkey[round * 16 + 2]] << 16) ^ (const_IK0[expkey[round * 16 + 2]] >> 16))
                ^ ((const_IK0[expkey[round * 16 + 3]] << 24) ^ (const_IK0[expkey[round * 16 + 3]] >> 8));
            w2 = (sm_td4[(s3 >> 24)] ^ sm_td3[(s4 >> 16) & 0xFF] ^ sm_td2[(s1 >> 8) & 0xFF] ^ sm_td1[s2 & 0xFF])
                ^ const_IK0[expkey[round * 16 + 4]]
                ^ ((const_IK0[expkey[round * 16 + 5]] << 8) ^ (const_IK0[expkey[round * 16 + 5]] >> 24))
                ^ ((const_IK0[expkey[round * 16 + 6]] << 16) ^ (const_IK0[expkey[round * 16 + 6]] >> 16))
                ^ ((const_IK0[expkey[round * 16 + 7]] << 24) ^ (const_IK0[expkey[round * 16 + 7]] >> 8));
            w3 = (sm_td4[(s4 >> 24)] ^ sm_td3[(s1 >> 16) & 0xFF] ^ sm_td2[(s2 >> 8) & 0xFF] ^ sm_td1[s3 & 0xFF])
                ^ const_IK0[expkey[round * 16 + 8]]
                ^ ((const_IK0[expkey[round * 16 + 9]] << 8) ^ (const_IK0[expkey[round * 16 + 9]] >> 24))
                ^ ((const_IK0[expkey[round * 16 + 10]] << 16) ^ (const_IK0[expkey[round * 16 + 10]] >> 16))
                ^ ((const_IK0[expkey[round * 16 + 11]] << 24) ^ (const_IK0[expkey[round * 16 + 11]] >> 8));
            w4 = (sm_td4[(s1 >> 24)] ^ sm_td3[(s2 >> 16) & 0xFF] ^ sm_td2[(s3 >> 8) & 0xFF] ^ sm_td1[s4 & 0xFF])
                ^ const_IK0[expkey[round * 16 + 12]]
                ^ ((const_IK0[expkey[round * 16 + 13]] << 8) ^ (const_IK0[expkey[round * 16 + 13]] >> 24))
                ^ ((const_IK0[expkey[round * 16 + 14]] << 16) ^ (const_IK0[expkey[round * 16 + 14]] >> 16))
                ^ ((const_IK0[expkey[round * 16 + 15]] << 24) ^ (const_IK0[expkey[round * 16 + 15]] >> 8));


            s1 = w1;
            s2 = w2;
            s3 = w3;
            s4 = w4;

        }

        w1  = (uint32_t)(sm_isbox[ s1 & 0xFF]);
        w1 |= (uint32_t)(sm_isbox[(s4 >>  8) & 0xFF]) << 8;
        w1 |= (uint32_t)(sm_isbox[(s3 >> 16) & 0xFF]) << 16;
        w1 |= (uint32_t)(sm_isbox[(s2 >> 24)       ]) << 24; //SubBytes and ShiftRows
        w1 ^= const_expkey[0];
        dev_output[4*i] = w1 ; //store the cipher text

        w2  = (uint32_t)(sm_isbox[ s2 & 0xFF]);
        w2 |= (uint32_t)(sm_isbox[(s1 >>  8) & 0xFF]) << 8;
        w2 |= (uint32_t)(sm_isbox[(s4 >> 16) & 0xFF]) << 16;
        w2 |= (uint32_t)(sm_isbox[(s3 >> 24)       ]) << 24; //SubBytes and ShiftRows
        w2 ^= const_expkey[1];
        dev_output[4*i+1] = w2 ; //store the cipher text

        w3  = (uint32_t)(sm_isbox[ s3 & 0xFF]);
        w3 |= (uint32_t)(sm_isbox[(s2 >> 8) & 0xFF]) << 8;
        w3 |= (uint32_t)(sm_isbox[(s1 >> 16) & 0xFF]) << 16;
        w3 |= (uint32_t)(sm_isbox[(s4 >> 24)       ]) << 24; //SubBytes and ShiftRows
        w3 ^= const_expkey[2];
        dev_output[4*i+2] = w3 ; //store the cipher text

        w4  = (uint32_t)(sm_isbox[ s4 & 0xFF]);
        w4 |= (uint32_t)(sm_isbox[(s3 >> 8) & 0xFF]) <<  8;
        w4 |= (uint32_t)(sm_isbox[(s2 >> 16) & 0xFF]) << 16;
        w4 |= (uint32_t)(sm_isbox[(s1 >> 24)       ]) << 24; //SubBytes and ShiftRows
        w4 ^= const_expkey[3];
        dev_output[4*i+3] = w4 ; //store the cipher text
    }

}

#endif //_DECRYPT_ECB_H_