#ifndef _ENCRYPT_ECB_H_
#define _ENCRYPT_ECB_H_

#include "BtsUtils.h"
#include "addRoundKey.h"
#include "subBytes.h"
#include "shiftRows.h"
#include "mixColumns.h"

__device__ inline void shiftRows(uint32_t shared[128])  __attribute__((always_inline));

__global__ void encrypt_Kernel( uint32_t* dev_input, uint32_t* dev_output, size_t inputSize)
{
    // Index calculations
    int tid         = threadIdx.y*blockDim.x + threadIdx.x;     //local id
    int x           = blockIdx.x * blockDim.x + threadIdx.x;    //global x id
    int y           = blockIdx.y * blockDim.y + threadIdx.y;    //global y id
    int xwidth      = blockDim.x * gridDim.x;                   //X width of the grid
    int ywidth      = blockDim.y * gridDim.y;
    int global_tid  = y*xwidth + x;                             //global id
    int block_width = blockDim.x * blockDim.y;                  //block width
    
    
    // store the expended key in shared memory.
    __shared__ uint32_t sm_key[1408];
    __shared__ uint32_t plain[8192];

    for(int i = tid; i < 1408; i += block_width) {
        //load the extended key into the shared memory
        sm_key[i]   = const_expkey[i];
    }
    __syncthreads();
    // Each thread treat 512 bytes.
    int elements = inputSize/512;
    int gridSize = xwidth*ywidth;

    for(int i = global_tid; i < elements; i += gridSize) {
        //Loading plaintext
        plain[tid*128+0]   = dev_input[128*i+0];
        plain[tid*128+1]   = dev_input[128*i+1];
        plain[tid*128+2]   = dev_input[128*i+2];
        plain[tid*128+3]   = dev_input[128*i+3];
        plain[tid*128+4]   = dev_input[128*i+4];
        plain[tid*128+5]   = dev_input[128*i+5];
        plain[tid*128+6]   = dev_input[128*i+6];
        plain[tid*128+7]   = dev_input[128*i+7];
        plain[tid*128+8]   = dev_input[128*i+8];
        plain[tid*128+9]   = dev_input[128*i+9];
        plain[tid*128+10]  = dev_input[128*i+10];
        plain[tid*128+11]  = dev_input[128*i+11];
        plain[tid*128+12]  = dev_input[128*i+12];
        plain[tid*128+13]  = dev_input[128*i+13];
        plain[tid*128+14]  = dev_input[128*i+14];
        plain[tid*128+15]  = dev_input[128*i+15];
        plain[tid*128+16]  = dev_input[128*i+16];
        plain[tid*128+17]  = dev_input[128*i+17];
        plain[tid*128+18]  = dev_input[128*i+18];
        plain[tid*128+19]  = dev_input[128*i+19];
        plain[tid*128+20]  = dev_input[128*i+20];
        plain[tid*128+21]  = dev_input[128*i+21];
        plain[tid*128+22]  = dev_input[128*i+22];
        plain[tid*128+23]  = dev_input[128*i+23];
        plain[tid*128+24]  = dev_input[128*i+24];
        plain[tid*128+25]  = dev_input[128*i+25];
        plain[tid*128+26]  = dev_input[128*i+26];
        plain[tid*128+27]  = dev_input[128*i+27];
        plain[tid*128+28]  = dev_input[128*i+28];
        plain[tid*128+29]  = dev_input[128*i+29];
        plain[tid*128+30]  = dev_input[128*i+30];
        plain[tid*128+31]  = dev_input[128*i+31];
        plain[tid*128+32]  = dev_input[128*i+32];
        plain[tid*128+33]  = dev_input[128*i+33];
        plain[tid*128+34]  = dev_input[128*i+34];
        plain[tid*128+35]  = dev_input[128*i+35];
        plain[tid*128+36]  = dev_input[128*i+36];
        plain[tid*128+37]  = dev_input[128*i+37];
        plain[tid*128+38]  = dev_input[128*i+38];
        plain[tid*128+39]  = dev_input[128*i+39];
        plain[tid*128+40]  = dev_input[128*i+40];
        plain[tid*128+41]  = dev_input[128*i+41];
        plain[tid*128+42]  = dev_input[128*i+42];
        plain[tid*128+43]  = dev_input[128*i+43];
        plain[tid*128+44]  = dev_input[128*i+44];
        plain[tid*128+45]  = dev_input[128*i+45];
        plain[tid*128+46]  = dev_input[128*i+46];
        plain[tid*128+47]  = dev_input[128*i+47];
        plain[tid*128+48]  = dev_input[128*i+48];
        plain[tid*128+49]  = dev_input[128*i+49];
        plain[tid*128+50]  = dev_input[128*i+50];
        plain[tid*128+51]  = dev_input[128*i+51];
        plain[tid*128+52]  = dev_input[128*i+52];
        plain[tid*128+53]  = dev_input[128*i+53];
        plain[tid*128+54]  = dev_input[128*i+54];
        plain[tid*128+55]  = dev_input[128*i+55];
        plain[tid*128+56]  = dev_input[128*i+56];
        plain[tid*128+57]  = dev_input[128*i+57];
        plain[tid*128+58]  = dev_input[128*i+58];
        plain[tid*128+59]  = dev_input[128*i+59];
        plain[tid*128+60]  = dev_input[128*i+60];
        plain[tid*128+61]  = dev_input[128*i+61];
        plain[tid*128+62]  = dev_input[128*i+62];
        plain[tid*128+63]  = dev_input[128*i+63];
        plain[tid*128+64]  = dev_input[128*i+64];
        plain[tid*128+65]  = dev_input[128*i+65];
        plain[tid*128+66]  = dev_input[128*i+66];
        plain[tid*128+67]  = dev_input[128*i+67];
        plain[tid*128+68]  = dev_input[128*i+68];
        plain[tid*128+69]  = dev_input[128*i+69];
        plain[tid*128+70]  = dev_input[128*i+70];
        plain[tid*128+71]  = dev_input[128*i+71];
        plain[tid*128+72]  = dev_input[128*i+72];
        plain[tid*128+73]  = dev_input[128*i+73];
        plain[tid*128+74]  = dev_input[128*i+74];
        plain[tid*128+75]  = dev_input[128*i+75];
        plain[tid*128+76]  = dev_input[128*i+76];
        plain[tid*128+77]  = dev_input[128*i+77];
        plain[tid*128+78]  = dev_input[128*i+78];
        plain[tid*128+79]  = dev_input[128*i+79];
        plain[tid*128+80]  = dev_input[128*i+80];
        plain[tid*128+81]  = dev_input[128*i+81];
        plain[tid*128+82]  = dev_input[128*i+82];
        plain[tid*128+83]  = dev_input[128*i+83];
        plain[tid*128+84]  = dev_input[128*i+84];
        plain[tid*128+85]  = dev_input[128*i+85];
        plain[tid*128+86]  = dev_input[128*i+86];
        plain[tid*128+87]  = dev_input[128*i+87];
        plain[tid*128+88]  = dev_input[128*i+88];
        plain[tid*128+89]  = dev_input[128*i+89];
        plain[tid*128+90]  = dev_input[128*i+90];
        plain[tid*128+91]  = dev_input[128*i+91];
        plain[tid*128+92]  = dev_input[128*i+92];
        plain[tid*128+93]  = dev_input[128*i+93];
        plain[tid*128+94]  = dev_input[128*i+94];
        plain[tid*128+95]  = dev_input[128*i+95];
        plain[tid*128+96]  = dev_input[128*i+96];
        plain[tid*128+97]  = dev_input[128*i+97];
        plain[tid*128+98]  = dev_input[128*i+98];
        plain[tid*128+99]  = dev_input[128*i+99];
        plain[tid*128+100] = dev_input[128*i+100];
        plain[tid*128+101] = dev_input[128*i+101];
        plain[tid*128+102] = dev_input[128*i+102];
        plain[tid*128+103] = dev_input[128*i+103];
        plain[tid*128+104] = dev_input[128*i+104];
        plain[tid*128+105] = dev_input[128*i+105];
        plain[tid*128+106] = dev_input[128*i+106];
        plain[tid*128+107] = dev_input[128*i+107];
        plain[tid*128+108] = dev_input[128*i+108];
        plain[tid*128+109] = dev_input[128*i+109];
        plain[tid*128+110] = dev_input[128*i+110];
        plain[tid*128+111] = dev_input[128*i+111];
        plain[tid*128+112] = dev_input[128*i+112];
        plain[tid*128+113] = dev_input[128*i+113];
        plain[tid*128+114] = dev_input[128*i+114];
        plain[tid*128+115] = dev_input[128*i+115];
        plain[tid*128+116] = dev_input[128*i+116];
        plain[tid*128+117] = dev_input[128*i+117];
        plain[tid*128+118] = dev_input[128*i+118];
        plain[tid*128+119] = dev_input[128*i+119];
        plain[tid*128+120] = dev_input[128*i+120];
        plain[tid*128+121] = dev_input[128*i+121];
        plain[tid*128+122] = dev_input[128*i+122];
        plain[tid*128+123] = dev_input[128*i+123];
        plain[tid*128+124] = dev_input[128*i+124];
        plain[tid*128+125] = dev_input[128*i+125];
        plain[tid*128+126] = dev_input[128*i+126];
        plain[tid*128+127] = dev_input[128*i+127];

        // First round AddRoundKey: ex-or with round key
        addRoundKey(&plain[tid*128], sm_key);

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
            addRoundKey(&plain[tid*128], &sm_key[j*128]);
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
        addRoundKey(&plain[tid*128], &sm_key[1280]);

        //output
        dev_output[128*i+0]   = plain[tid*128+0];
        dev_output[128*i+1]   = plain[tid*128+1];
        dev_output[128*i+2]   = plain[tid*128+2];
        dev_output[128*i+3]   = plain[tid*128+3];
        dev_output[128*i+4]   = plain[tid*128+4];
        dev_output[128*i+5]   = plain[tid*128+5];
        dev_output[128*i+6]   = plain[tid*128+6];
        dev_output[128*i+7]   = plain[tid*128+7];
        dev_output[128*i+8]   = plain[tid*128+8];
        dev_output[128*i+9]   = plain[tid*128+9];    
        dev_output[128*i+10]  = plain[tid*128+10]; 
        dev_output[128*i+11]  = plain[tid*128+11]; 
        dev_output[128*i+12]  = plain[tid*128+12]; 
        dev_output[128*i+13]  = plain[tid*128+13]; 
        dev_output[128*i+14]  = plain[tid*128+14]; 
        dev_output[128*i+15]  = plain[tid*128+15]; 
        dev_output[128*i+16]  = plain[tid*128+16]; 
        dev_output[128*i+17]  = plain[tid*128+17]; 
        dev_output[128*i+18]  = plain[tid*128+18]; 
        dev_output[128*i+19]  = plain[tid*128+19]; 
        dev_output[128*i+20]  = plain[tid*128+20]; 
        dev_output[128*i+21]  = plain[tid*128+21]; 
        dev_output[128*i+22]  = plain[tid*128+22]; 
        dev_output[128*i+23]  = plain[tid*128+23]; 
        dev_output[128*i+24]  = plain[tid*128+24]; 
        dev_output[128*i+25]  = plain[tid*128+25]; 
        dev_output[128*i+26]  = plain[tid*128+26]; 
        dev_output[128*i+27]  = plain[tid*128+27]; 
        dev_output[128*i+28]  = plain[tid*128+28]; 
        dev_output[128*i+29]  = plain[tid*128+29]; 
        dev_output[128*i+30]  = plain[tid*128+30]; 
        dev_output[128*i+31]  = plain[tid*128+31]; 
        dev_output[128*i+32]  = plain[tid*128+32]; 
        dev_output[128*i+33]  = plain[tid*128+33]; 
        dev_output[128*i+34]  = plain[tid*128+34]; 
        dev_output[128*i+35]  = plain[tid*128+35]; 
        dev_output[128*i+36]  = plain[tid*128+36]; 
        dev_output[128*i+37]  = plain[tid*128+37]; 
        dev_output[128*i+38]  = plain[tid*128+38]; 
        dev_output[128*i+39]  = plain[tid*128+39]; 
        dev_output[128*i+40]  = plain[tid*128+40]; 
        dev_output[128*i+41]  = plain[tid*128+41]; 
        dev_output[128*i+42]  = plain[tid*128+42]; 
        dev_output[128*i+43]  = plain[tid*128+43]; 
        dev_output[128*i+44]  = plain[tid*128+44]; 
        dev_output[128*i+45]  = plain[tid*128+45]; 
        dev_output[128*i+46]  = plain[tid*128+46]; 
        dev_output[128*i+47]  = plain[tid*128+47]; 
        dev_output[128*i+48]  = plain[tid*128+48]; 
        dev_output[128*i+49]  = plain[tid*128+49]; 
        dev_output[128*i+50]  = plain[tid*128+50]; 
        dev_output[128*i+51]  = plain[tid*128+51]; 
        dev_output[128*i+52]  = plain[tid*128+52]; 
        dev_output[128*i+53]  = plain[tid*128+53]; 
        dev_output[128*i+54]  = plain[tid*128+54]; 
        dev_output[128*i+55]  = plain[tid*128+55]; 
        dev_output[128*i+56]  = plain[tid*128+56]; 
        dev_output[128*i+57]  = plain[tid*128+57]; 
        dev_output[128*i+58]  = plain[tid*128+58]; 
        dev_output[128*i+59]  = plain[tid*128+59]; 
        dev_output[128*i+60]  = plain[tid*128+60]; 
        dev_output[128*i+61]  = plain[tid*128+61]; 
        dev_output[128*i+62]  = plain[tid*128+62]; 
        dev_output[128*i+63]  = plain[tid*128+63]; 
        dev_output[128*i+64]  = plain[tid*128+64]; 
        dev_output[128*i+65]  = plain[tid*128+65]; 
        dev_output[128*i+66]  = plain[tid*128+66]; 
        dev_output[128*i+67]  = plain[tid*128+67]; 
        dev_output[128*i+68]  = plain[tid*128+68]; 
        dev_output[128*i+69]  = plain[tid*128+69]; 
        dev_output[128*i+70]  = plain[tid*128+70]; 
        dev_output[128*i+71]  = plain[tid*128+71]; 
        dev_output[128*i+72]  = plain[tid*128+72]; 
        dev_output[128*i+73]  = plain[tid*128+73]; 
        dev_output[128*i+74]  = plain[tid*128+74]; 
        dev_output[128*i+75]  = plain[tid*128+75]; 
        dev_output[128*i+76]  = plain[tid*128+76]; 
        dev_output[128*i+77]  = plain[tid*128+77]; 
        dev_output[128*i+78]  = plain[tid*128+78]; 
        dev_output[128*i+79]  = plain[tid*128+79]; 
        dev_output[128*i+80]  = plain[tid*128+80]; 
        dev_output[128*i+81]  = plain[tid*128+81]; 
        dev_output[128*i+82]  = plain[tid*128+82]; 
        dev_output[128*i+83]  = plain[tid*128+83]; 
        dev_output[128*i+84]  = plain[tid*128+84]; 
        dev_output[128*i+85]  = plain[tid*128+85]; 
        dev_output[128*i+86]  = plain[tid*128+86]; 
        dev_output[128*i+87]  = plain[tid*128+87]; 
        dev_output[128*i+88]  = plain[tid*128+88]; 
        dev_output[128*i+89]  = plain[tid*128+89]; 
        dev_output[128*i+90]  = plain[tid*128+90]; 
        dev_output[128*i+91]  = plain[tid*128+91]; 
        dev_output[128*i+92]  = plain[tid*128+92]; 
        dev_output[128*i+93]  = plain[tid*128+93]; 
        dev_output[128*i+94]  = plain[tid*128+94]; 
        dev_output[128*i+95]  = plain[tid*128+95]; 
        dev_output[128*i+96]  = plain[tid*128+96]; 
        dev_output[128*i+97]  = plain[tid*128+97]; 
        dev_output[128*i+98]  = plain[tid*128+98]; 
        dev_output[128*i+99]  = plain[tid*128+99];
        dev_output[128*i+100] = plain[tid*128+100] ;
        dev_output[128*i+101] = plain[tid*128+101] ;
        dev_output[128*i+102] = plain[tid*128+102] ;
        dev_output[128*i+103] = plain[tid*128+103] ;
        dev_output[128*i+104] = plain[tid*128+104] ;
        dev_output[128*i+105] = plain[tid*128+105] ;
        dev_output[128*i+106] = plain[tid*128+106] ;
        dev_output[128*i+107] = plain[tid*128+107] ;
        dev_output[128*i+108] = plain[tid*128+108] ;
        dev_output[128*i+109] = plain[tid*128+109] ;
        dev_output[128*i+110] = plain[tid*128+110] ;
        dev_output[128*i+111] = plain[tid*128+111] ;
        dev_output[128*i+112] = plain[tid*128+112] ;
        dev_output[128*i+113] = plain[tid*128+113] ;
        dev_output[128*i+114] = plain[tid*128+114] ;
        dev_output[128*i+115] = plain[tid*128+115] ;
        dev_output[128*i+116] = plain[tid*128+116] ;
        dev_output[128*i+117] = plain[tid*128+117] ;
        dev_output[128*i+118] = plain[tid*128+118] ;
        dev_output[128*i+119] = plain[tid*128+119] ;
        dev_output[128*i+120] = plain[tid*128+120] ;
        dev_output[128*i+121] = plain[tid*128+121] ;
        dev_output[128*i+122] = plain[tid*128+122] ;
        dev_output[128*i+123] = plain[tid*128+123] ;
        dev_output[128*i+124] = plain[tid*128+124] ;
        dev_output[128*i+125] = plain[tid*128+125] ;
        dev_output[128*i+126] = plain[tid*128+126] ;
        dev_output[128*i+127] = plain[tid*128+127] ;
    }
}

#endif //_ENCRYPT_ECB_H_