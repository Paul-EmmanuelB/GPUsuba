#ifndef _TRANSPOSE_H_
#define _TRANSPOSE_H_

#include <stdint.h>


// ***Transposition***

//* Typical bitsliced transposition with 32 states for 128 registers
void transpose(uint32_t data[128])
{
    //** Transposing data in a local array
    uint32_t transposed_data[128] = { 0 };
    for (int i = 0; i < 32; i++){
        for (int j = 0; j < 32; j++){
            transposed_data[j   ] |= ((data[4*i+0] >> j) & 1) << i;
            transposed_data[j+32] |= ((data[4*i+1] >> j) & 1) << i;
            transposed_data[j+64] |= ((data[4*i+2] >> j) & 1) << i;
            transposed_data[j+96] |= ((data[4*i+3] >> j) & 1) << i;
        }
    }
    //** Copying the local array
    memcpy(data, transposed_data, 128 * sizeof(uint32_t));
}


//* Inverse transposition 
void invTranspose(uint32_t transposed_data[128])
{
    //** Transposing data in a local array
    uint32_t data[128] = { 0 };
    for (int i = 0; i < 32; i++){
        for (int j = 0; j < 32; j++){
            data[4*i  ] |= ((transposed_data[j+0 ] >> i) & 1) << j;
            data[4*i+1] |= ((transposed_data[j+32] >> i) & 1) << j;
            data[4*i+2] |= ((transposed_data[j+64] >> i) & 1) << j;
            data[4*i+3] |= ((transposed_data[j+96] >> i) & 1) << j;
        }
    }
    //** Copying the local array
    memcpy(transposed_data, data, 128 * sizeof(uint32_t));
}


//* Transposition with reorganization for easier lecture from GPU's threads within a warp
void transposeBts(uint32_t data[4096])
{
    uint32_t result[4096] = {0};
    for(int h=0; h<32; h++){
        transpose(&data[h*128]);
    }
    //** Reorganization
    for(int i=0; i<32; i++){
        for(int j=0; j<128; j++){
            result[32*j+i] = data[128*i+j];
        }
    }
    memcpy(data, result, 4096 * sizeof(uint32_t));
}


void invTransposeBts(uint32_t transposed_data[4096])
{
    uint32_t temp[4096] = {0};
    //** Reorganization
    for(int i=0; i<32; i++){
        for(int j=0; j<128; j++){
            temp[128*i+j] = transposed_data[32*j+i];
        }
    }
    for(int i=0; i<32; i++){
        invTranspose(&temp[i*128]);
    }
    memcpy(transposed_data, temp, 4096 * sizeof(uint32_t));
}

//* Transposition for the expanded key. Each bit is expanded into a 32 bit register.
void transposeKey(uint32_t key[44], uint32_t output[1408]){
    //** Transposing data in a local array
    uint32_t transposed_key[1408] = { 0 };
    for (int i = 0; i < 44; i++) {
        for (int j = 0; j < 32 ; j++) {
            transposed_key[32*i+0 ] |= ((key[i]      ) & 1) << j;
            transposed_key[32*i+1 ] |= ((key[i] >> 1 ) & 1) << j;
            transposed_key[32*i+2 ] |= ((key[i] >> 2 ) & 1) << j;
            transposed_key[32*i+3 ] |= ((key[i] >> 3 ) & 1) << j;
            transposed_key[32*i+4 ] |= ((key[i] >> 4 ) & 1) << j;
            transposed_key[32*i+5 ] |= ((key[i] >> 5 ) & 1) << j;
            transposed_key[32*i+6 ] |= ((key[i] >> 6 ) & 1) << j;
            transposed_key[32*i+7 ] |= ((key[i] >> 7 ) & 1) << j;
            transposed_key[32*i+8 ] |= ((key[i] >> 8 ) & 1) << j;
            transposed_key[32*i+9 ] |= ((key[i] >> 9 ) & 1) << j;
            transposed_key[32*i+10] |= ((key[i] >> 10) & 1) << j;
            transposed_key[32*i+11] |= ((key[i] >> 11) & 1) << j;
            transposed_key[32*i+12] |= ((key[i] >> 12) & 1) << j;
            transposed_key[32*i+13] |= ((key[i] >> 13) & 1) << j;
            transposed_key[32*i+14] |= ((key[i] >> 14) & 1) << j;
            transposed_key[32*i+15] |= ((key[i] >> 15) & 1) << j;
            transposed_key[32*i+16] |= ((key[i] >> 16) & 1) << j;
            transposed_key[32*i+17] |= ((key[i] >> 17) & 1) << j;
            transposed_key[32*i+18] |= ((key[i] >> 18) & 1) << j;
            transposed_key[32*i+19] |= ((key[i] >> 19) & 1) << j;
            transposed_key[32*i+20] |= ((key[i] >> 20) & 1) << j;
            transposed_key[32*i+21] |= ((key[i] >> 21) & 1) << j;
            transposed_key[32*i+22] |= ((key[i] >> 22) & 1) << j;
            transposed_key[32*i+23] |= ((key[i] >> 23) & 1) << j;
            transposed_key[32*i+24] |= ((key[i] >> 24) & 1) << j;
            transposed_key[32*i+25] |= ((key[i] >> 25) & 1) << j;
            transposed_key[32*i+26] |= ((key[i] >> 26) & 1) << j;
            transposed_key[32*i+27] |= ((key[i] >> 27) & 1) << j;
            transposed_key[32*i+28] |= ((key[i] >> 28) & 1) << j;
            transposed_key[32*i+29] |= ((key[i] >> 29) & 1) << j;
            transposed_key[32*i+30] |= ((key[i] >> 30) & 1) << j;
            transposed_key[32*i+31] |= ((key[i] >> 31) & 1) << j;
        }
    }
    //** Copying the local array
    memcpy(output, transposed_key, 1408 * sizeof(uint32_t));
}


#endif //_TRANSPOSE_H_