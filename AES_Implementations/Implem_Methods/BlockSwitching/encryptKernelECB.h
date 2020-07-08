#ifndef _ENCRYPT_ECB_H_
#define _ENCRYPT_ECB_H_

#include "utils.h"

__global__ static void encrypt_Kernel( uint32_t* dev_input, uint32_t* dev_output, 
size_t inputSize, uint32_t* dev_sm_te1, uint32_t* dev_sm_te2, uint32_t* dev_sm_te3, uint32_t* dev_sm_te4,  uint8_t* dev_sm_sbox)
{
    // Index calculations
    int tid         = threadIdx.x;                           //local id
    int global_tid  = blockIdx.x * blockDim.x + threadIdx.x; //global id

    uint32_t w1,w2,w3,w4,s1,s2,s3,s4;
    int ROUNDS = 11;

    // store the T-boxes and sbox in shared memory.
    __shared__ uint32_t sm_te1[256], sm_te2[256], sm_te3[256], sm_te4[256];
    __shared__ uint8_t sm_sbox[256];
    
    //Loading TBoxes and Sbox
    for(int i = tid; i < 256; i += blockDim.x) {
        //load dev_sm_te1, dev_sm_te2, dev_sm_te3, dev_sm_te4 and
        // sm_sbox to share memory variables sm_te1, sm_te2,
        //sm_te3, sm_te4 and sm_sbox;
        sm_te1[i]   = dev_sm_te1[i];
        sm_te2[i]   = dev_sm_te2[i];
        sm_te3[i]   = dev_sm_te3[i];
        sm_te4[i]   = dev_sm_te4[i];
        sm_sbox[i]  = dev_sm_sbox[i];
    }
    __syncthreads();

    // Each thread treat 16 bytes.
    int elements = inputSize/16;
    int gridSize = blockDim.x*gridDim.x;

    for(int i = global_tid; i < elements; i += gridSize) {
        //Loading plaintext
        w1 = dev_input[4*i];
        w2 = dev_input[4*i+1];
        w3 = dev_input[4*i+2];
        w4 = dev_input[4*i+3];

        // First round AddRoundKey: ex-or with round key
        w1 ^= const_expkey[0];
        w2 ^= const_expkey[1];
        w3 ^= const_expkey[2];
        w4 ^= const_expkey[3];

        // Round transformation: a set of table lookups operations.
        #pragma unroll
        for (int j = 1; j < 10; j++) {
            s1 = (sm_te4[(w4 >> 24)] ^ sm_te3[(w3 >> 16) & 0xFF] ^ sm_te2[(w2 >> 8) & 0xFF] ^ sm_te1[w1 & 0xFF]);
            s2 = (sm_te4[(w1 >> 24)] ^ sm_te3[(w4 >> 16) & 0xFF] ^ sm_te2[(w3 >> 8) & 0xFF] ^ sm_te1[w2 & 0xFF]);
            s3 = (sm_te4[(w2 >> 24)] ^ sm_te3[(w1 >> 16) & 0xFF] ^ sm_te2[(w4 >> 8) & 0xFF] ^ sm_te1[w3 & 0xFF]);
            s4 = (sm_te4[(w3 >> 24)] ^ sm_te3[(w2 >> 16) & 0xFF] ^ sm_te2[(w1 >> 8) & 0xFF] ^ sm_te1[w4 & 0xFF]);

            w1 = s1 ^ const_expkey[j * 4];
            w2 = s2 ^ const_expkey[j * 4 + 1];
            w3 = s3 ^ const_expkey[j * 4 + 2];
            w4 = s4 ^ const_expkey[j * 4 + 3];	
        } 

        // The final round doesn’t include the MixColumns
        s1  = (uint32_t)(sm_sbox[ w1        & 0xFF]);
        s1 |= (uint32_t)(sm_sbox[(w2 >>  8) & 0xFF]) << 8;
        s1 |= (uint32_t)(sm_sbox[(w3 >> 16) & 0xFF]) << 16;
        s1 |= (uint32_t)(sm_sbox[(w4 >> 24)       ]) << 24; //SubBytes and ShiftRows
        s1 ^= const_expkey[(ROUNDS - 1) * 4]; //AddRoundKey
        dev_output[4*i] = s1 ; //store the cipher text
        
        s2  = (uint32_t)(sm_sbox[ w2        & 0xFF]);
        s2 |= (uint32_t)(sm_sbox[(w3 >>  8) & 0xFF]) << 8;
        s2 |= (uint32_t)(sm_sbox[(w4 >> 16) & 0xFF]) << 16;
        s2 |= (uint32_t)(sm_sbox[(w1 >> 24)       ]) << 24; //SubBytes and ShiftRows
        s2 ^= const_expkey[(ROUNDS - 1) * 4 + 1]; //AddRoundKey
        dev_output[4*i+1] = s2 ; //store the cipher text

        s3  = (uint32_t)(sm_sbox[ w3        & 0xFF]);
        s3 |= (uint32_t)(sm_sbox[(w4 >>  8) & 0xFF]) << 8;
        s3 |= (uint32_t)(sm_sbox[(w1 >> 16) & 0xFF]) << 16;
        s3 |= (uint32_t)(sm_sbox[(w2 >> 24)       ]) << 24; //SubBytes and ShiftRows
        s3 ^= const_expkey[(ROUNDS - 1) * 4 + 2]; //AddRoundKey
        dev_output[4*i+2] = s3 ; //store the cipher text

        s4  = (uint32_t)(sm_sbox[ w4        & 0xFF]);
        s4 |= (uint32_t)(sm_sbox[(w1 >>  8) & 0xFF]) << 8;
        s4 |= (uint32_t)(sm_sbox[(w2 >> 16) & 0xFF]) << 16;
        s4 |= (uint32_t)(sm_sbox[(w3 >> 24)       ]) << 24; //SubBytes and ShiftRows
        s4 ^= const_expkey[(ROUNDS - 1) * 4 + 3]; //AddRoundKey
        dev_output[4*i+3] = s4 ; //store the cipher text
    }
}

#endif //_ENCRYPT_ECB_H_