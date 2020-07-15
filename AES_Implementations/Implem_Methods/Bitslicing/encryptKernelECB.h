#ifndef _ENCRYPT_ECB_H_
#define _ENCRYPT_ECB_H_

#include "BtsUtils.h"
#include "addRoundKey.h"
#include "subBytes.h"
#include "shiftRows.h"
#include "mixColumns.h"

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
        uint32_t r0   = dev_input[128*i+0];
        uint32_t r1   = dev_input[128*i+1];
        uint32_t r2   = dev_input[128*i+2];
        uint32_t r3   = dev_input[128*i+3];
        uint32_t r4   = dev_input[128*i+4];
        uint32_t r5   = dev_input[128*i+5];
        uint32_t r6   = dev_input[128*i+6];
        uint32_t r7   = dev_input[128*i+7];
        uint32_t r8   = dev_input[128*i+8];
        uint32_t r9   = dev_input[128*i+9];
        uint32_t r10  = dev_input[128*i+10];
        uint32_t r11  = dev_input[128*i+11];
        uint32_t r12  = dev_input[128*i+12];
        uint32_t r13  = dev_input[128*i+13];
        uint32_t r14  = dev_input[128*i+14];
        uint32_t r15  = dev_input[128*i+15];
        uint32_t r16  = dev_input[128*i+16];
        uint32_t r17  = dev_input[128*i+17];
        uint32_t r18  = dev_input[128*i+18];
        uint32_t r19  = dev_input[128*i+19];
        uint32_t r20  = dev_input[128*i+20];
        uint32_t r21  = dev_input[128*i+21];
        uint32_t r22  = dev_input[128*i+22];
        uint32_t r23  = dev_input[128*i+23];
        uint32_t r24  = dev_input[128*i+24];
        uint32_t r25  = dev_input[128*i+25];
        uint32_t r26  = dev_input[128*i+26];
        uint32_t r27  = dev_input[128*i+27];
        uint32_t r28  = dev_input[128*i+28];
        uint32_t r29  = dev_input[128*i+29];
        uint32_t r30  = dev_input[128*i+30];
        uint32_t r31  = dev_input[128*i+31];
        uint32_t r32  = dev_input[128*i+32];
        uint32_t r33  = dev_input[128*i+33];
        uint32_t r34  = dev_input[128*i+34];
        uint32_t r35  = dev_input[128*i+35];
        uint32_t r36  = dev_input[128*i+36];
        uint32_t r37  = dev_input[128*i+37];
        uint32_t r38  = dev_input[128*i+38];
        uint32_t r39  = dev_input[128*i+39];
        uint32_t r40  = dev_input[128*i+40];
        uint32_t r41  = dev_input[128*i+41];
        uint32_t r42  = dev_input[128*i+42];
        uint32_t r43  = dev_input[128*i+43];
        uint32_t r44  = dev_input[128*i+44];
        uint32_t r45  = dev_input[128*i+45];
        uint32_t r46  = dev_input[128*i+46];
        uint32_t r47  = dev_input[128*i+47];
        uint32_t r48  = dev_input[128*i+48];
        uint32_t r49  = dev_input[128*i+49];
        uint32_t r50  = dev_input[128*i+50];
        uint32_t r51  = dev_input[128*i+51];
        uint32_t r52  = dev_input[128*i+52];
        uint32_t r53  = dev_input[128*i+53];
        uint32_t r54  = dev_input[128*i+54];
        uint32_t r55  = dev_input[128*i+55];
        uint32_t r56  = dev_input[128*i+56];
        uint32_t r57  = dev_input[128*i+57];
        uint32_t r58  = dev_input[128*i+58];
        uint32_t r59  = dev_input[128*i+59];
        uint32_t r60  = dev_input[128*i+60];
        uint32_t r61  = dev_input[128*i+61];
        uint32_t r62  = dev_input[128*i+62];
        uint32_t r63  = dev_input[128*i+63];
        uint32_t r64  = dev_input[128*i+64];
        uint32_t r65  = dev_input[128*i+65];
        uint32_t r66  = dev_input[128*i+66];
        uint32_t r67  = dev_input[128*i+67];
        uint32_t r68  = dev_input[128*i+68];
        uint32_t r69  = dev_input[128*i+69];
        uint32_t r70  = dev_input[128*i+70];
        uint32_t r71  = dev_input[128*i+71];
        uint32_t r72  = dev_input[128*i+72];
        uint32_t r73  = dev_input[128*i+73];
        uint32_t r74  = dev_input[128*i+74];
        uint32_t r75  = dev_input[128*i+75];
        uint32_t r76  = dev_input[128*i+76];
        uint32_t r77  = dev_input[128*i+77];
        uint32_t r78  = dev_input[128*i+78];
        uint32_t r79  = dev_input[128*i+79];
        uint32_t r80  = dev_input[128*i+80];
        uint32_t r81  = dev_input[128*i+81];
        uint32_t r82  = dev_input[128*i+82];
        uint32_t r83  = dev_input[128*i+83];
        uint32_t r84  = dev_input[128*i+84];
        uint32_t r85  = dev_input[128*i+85];
        uint32_t r86  = dev_input[128*i+86];
        uint32_t r87  = dev_input[128*i+87];
        uint32_t r88  = dev_input[128*i+88];
        uint32_t r89  = dev_input[128*i+89];
        uint32_t r90  = dev_input[128*i+90];
        uint32_t r91  = dev_input[128*i+91];
        uint32_t r92  = dev_input[128*i+92];
        uint32_t r93  = dev_input[128*i+93];
        uint32_t r94  = dev_input[128*i+94];
        uint32_t r95  = dev_input[128*i+95];
        uint32_t r96  = dev_input[128*i+96];
        uint32_t r97  = dev_input[128*i+97];
        uint32_t r98  = dev_input[128*i+98];
        uint32_t r99  = dev_input[128*i+99];
        uint32_t r100 = dev_input[128*i+100];
        uint32_t r101 = dev_input[128*i+101];
        uint32_t r102 = dev_input[128*i+102];
        uint32_t r103 = dev_input[128*i+103];
        uint32_t r104 = dev_input[128*i+104];
        uint32_t r105 = dev_input[128*i+105];
        uint32_t r106 = dev_input[128*i+106];
        uint32_t r107 = dev_input[128*i+107];
        uint32_t r108 = dev_input[128*i+108];
        uint32_t r109 = dev_input[128*i+109];
        uint32_t r110 = dev_input[128*i+110];
        uint32_t r111 = dev_input[128*i+111];
        uint32_t r112 = dev_input[128*i+112];
        uint32_t r113 = dev_input[128*i+113];
        uint32_t r114 = dev_input[128*i+114];
        uint32_t r115 = dev_input[128*i+115];
        uint32_t r116 = dev_input[128*i+116];
        uint32_t r117 = dev_input[128*i+117];
        uint32_t r118 = dev_input[128*i+118];
        uint32_t r119 = dev_input[128*i+119];
        uint32_t r120 = dev_input[128*i+120];
        uint32_t r121 = dev_input[128*i+121];
        uint32_t r122 = dev_input[128*i+122];
        uint32_t r123 = dev_input[128*i+123];
        uint32_t r124 = dev_input[128*i+124];
        uint32_t r125 = dev_input[128*i+125];
        uint32_t r126 = dev_input[128*i+126];
        uint32_t r127 = dev_input[128*i+127];


        // First round AddRoundKey: ex-or with round key
        addRoundKey(r0  , r1  , r2  , r3  , r4  , r5  , r6  , r7  ,
                    r8  , r9  , r10 , r11 , r12 , r13 , r14 , r15 ,
                    r16 , r17 , r18 , r19 , r20 , r21 , r22 , r23 ,
                    r24 , r25 , r26 , r27 , r28 , r29 , r30 , r31 ,
                    r32 , r33 , r34 , r35 , r36 , r37 , r38 , r39 ,
                    r40 , r41 , r42 , r43 , r44 , r45 , r46 , r47 ,
                    r48 , r49 , r50 , r51 , r52 , r53 , r54 , r55 ,
                    r56 , r57 , r58 , r59 , r60 , r61 , r62 , r63 ,
                    r64 , r65 , r66 , r67 , r68 , r69 , r70 , r71 ,
                    r72 , r73 , r74 , r75 , r76 , r77 , r78 , r79 ,
                    r80 , r81 , r82 , r83 , r84 , r85 , r86 , r87 ,
                    r88 , r89 , r90 , r91 , r92 , r93 , r94 , r95 ,
                    r96 , r97 , r98 , r99 , r100, r101, r102, r103,
                    r104, r105, r106, r107, r108, r109, r110, r111,
                    r112, r113, r114, r115, r116, r117, r118, r119,
                    r120, r121, r122, r123, r124, r125, r126, r127,
                    sm_key);

        // Round transformation: a set of table lookups operations.
        //#pragma unroll
        for (int j = 1; j < 10; j++) {
            //subBytes
            Sbox( r0  , r1  , r2  , r3  , r4  , r5  , r6  , r7  );
            Sbox( r8  , r9  , r10 , r11 , r12 , r13 , r14 , r15 );
            Sbox( r16 , r17 , r18 , r19 , r20 , r21 , r22 , r23 );
            Sbox( r24 , r25 , r26 , r27 , r28 , r29 , r30 , r31 );
            Sbox( r32 , r33 , r34 , r35 , r36 , r37 , r38 , r39 );
            Sbox( r40 , r41 , r42 , r43 , r44 , r45 , r46 , r47 );
            Sbox( r48 , r49 , r50 , r51 , r52 , r53 , r54 , r55 );
            Sbox( r56 , r57 , r58 , r59 , r60 , r61 , r62 , r63 );
            Sbox( r64 , r65 , r66 , r67 , r68 , r69 , r70 , r71 );
            Sbox( r72 , r73 , r74 , r75 , r76 , r77 , r78 , r79 );
            Sbox( r80 , r81 , r82 , r83 , r84 , r85 , r86 , r87 );
            Sbox( r88 , r89 , r90 , r91 , r92 , r93 , r94 , r95 );
            Sbox( r96 , r97 , r98 , r99 , r100, r101, r102, r103);
            Sbox( r104, r105, r106, r107, r108, r109, r110, r111);
            Sbox( r112, r113, r114, r115, r116, r117, r118, r119);
            Sbox( r120, r121, r122, r123, r124, r125, r126, r127);
            
            //ShiftRows
            shiftRows( r8  ,  r9  ,  r10 ,  r11 ,  r12 ,  r13 ,  r14 ,  r15 ,  r16 ,  r17 ,  r18 ,  r19 ,  r20 ,  r21 ,  r22 ,  r23 ,  r24 ,  r25 ,  r26 ,  r27 ,  r28 ,  r29 ,  r30 ,  r31 ,
                       r40 ,  r41 ,  r42 ,  r43 ,  r44 ,  r45 ,  r46 ,  r47 ,  r48 ,  r49 ,  r50 ,  r51 ,  r52 ,  r53 ,  r54 ,  r55 ,  r56 ,  r57 ,  r58 ,  r59 ,  r60 ,  r61 ,  r62 ,  r63 ,
                       r72 ,  r73 ,  r74 ,  r75 ,  r76 ,  r77 ,  r78 ,  r79 ,  r80 ,  r81 ,  r82 ,  r83 ,  r84 ,  r85 ,  r86 ,  r87 ,  r88 ,  r89 ,  r90 ,  r91 ,  r92 ,  r93 ,  r94 ,  r95 ,
                       r104,  r105,  r106,  r107,  r108,  r109,  r110,  r111,  r112,  r113,  r114,  r115,  r116,  r117,  r118,  r119,  r120,  r121,  r122,  r123,  r124,  r125,  r126,  r127);

            //Mixcolums
            Mixcl(r0 , r1 , r2 , r3 , r4 , r5 , r6 , r7 ,
                     r8 , r9 , r10, r11, r12, r13, r14, r15,
                     r16, r17, r18, r19, r20, r21, r22, r23,
                     r24, r25, r26, r27, r28, r29, r30, r31);
                     
            Mixcl(r32, r33, r34, r35, r36, r37, r38, r39,
                     r40, r41, r42, r43, r44, r45, r46, r47,
                     r48, r49, r50, r51, r52, r53, r54, r55,
                     r56, r57, r58, r59, r60, r61, r62, r63);

            Mixcl(r64, r65, r66, r67, r68, r69, r70, r71,
                     r72, r73, r74, r75, r76, r77, r78, r79,
                     r80, r81, r82, r83, r84, r85, r86, r87,
                     r88, r89, r90, r91, r92, r93, r94, r95);

            Mixcl(r96 , r97 , r98 , r99 , r100, r101, r102, r103,
                     r104, r105, r106, r107, r108, r109, r110, r111,
                     r112, r113, r114, r115, r116, r117, r118, r119,
                     r120, r121, r122, r123, r124, r125, r126, r127);
            
            //AddRoundKey
            addRoundKey(r0  , r1  , r2  , r3  , r4  , r5  , r6  , r7  ,
                        r8  , r9  , r10 , r11 , r12 , r13 , r14 , r15 ,
                        r16 , r17 , r18 , r19 , r20 , r21 , r22 , r23 ,
                        r24 , r25 , r26 , r27 , r28 , r29 , r30 , r31 ,
                        r32 , r33 , r34 , r35 , r36 , r37 , r38 , r39 ,
                        r40 , r41 , r42 , r43 , r44 , r45 , r46 , r47 ,
                        r48 , r49 , r50 , r51 , r52 , r53 , r54 , r55 ,
                        r56 , r57 , r58 , r59 , r60 , r61 , r62 , r63 ,
                        r64 , r65 , r66 , r67 , r68 , r69 , r70 , r71 ,
                        r72 , r73 , r74 , r75 , r76 , r77 , r78 , r79 ,
                        r80 , r81 , r82 , r83 , r84 , r85 , r86 , r87 ,
                        r88 , r89 , r90 , r91 , r92 , r93 , r94 , r95 ,
                        r96 , r97 , r98 , r99 , r100, r101, r102, r103,
                        r104, r105, r106, r107, r108, r109, r110, r111,
                        r112, r113, r114, r115, r116, r117, r118, r119,
                        r120, r121, r122, r123, r124, r125, r126, r127,
                        &sm_key[j*128]);
        } 

        // The final round doesn’t include the MixColumns step
        //subBytes
        Sbox( r0  , r1  , r2  , r3  , r4  , r5  , r6  , r7  );
        Sbox( r8  , r9  , r10 , r11 , r12 , r13 , r14 , r15 );
        Sbox( r16 , r17 , r18 , r19 , r20 , r21 , r22 , r23 );
        Sbox( r24 , r25 , r26 , r27 , r28 , r29 , r30 , r31 );
        Sbox( r32 , r33 , r34 , r35 , r36 , r37 , r38 , r39 );
        Sbox( r40 , r41 , r42 , r43 , r44 , r45 , r46 , r47 );
        Sbox( r48 , r49 , r50 , r51 , r52 , r53 , r54 , r55 );
        Sbox( r56 , r57 , r58 , r59 , r60 , r61 , r62 , r63 );
        Sbox( r64 , r65 , r66 , r67 , r68 , r69 , r70 , r71 );
        Sbox( r72 , r73 , r74 , r75 , r76 , r77 , r78 , r79 );
        Sbox( r80 , r81 , r82 , r83 , r84 , r85 , r86 , r87 );
        Sbox( r88 , r89 , r90 , r91 , r92 , r93 , r94 , r95 );
        Sbox( r96 , r97 , r98 , r99 , r100, r101, r102, r103);
        Sbox( r104, r105, r106, r107, r108, r109, r110, r111);
        Sbox( r112, r113, r114, r115, r116, r117, r118, r119);
        Sbox( r120, r121, r122, r123, r124, r125, r126, r127);

        
        //ShiftRows
        shiftRows( r8  ,  r9  ,  r10 ,  r11 ,  r12 ,  r13 ,  r14 ,  r15 ,  r16 ,  r17 ,  r18 ,  r19 ,  r20 ,  r21 ,  r22 ,  r23 ,  r24 ,  r25 ,  r26 ,  r27 ,  r28 ,  r29 ,  r30 ,  r31 ,
                   r40 ,  r41 ,  r42 ,  r43 ,  r44 ,  r45 ,  r46 ,  r47 ,  r48 ,  r49 ,  r50 ,  r51 ,  r52 ,  r53 ,  r54 ,  r55 ,  r56 ,  r57 ,  r58 ,  r59 ,  r60 ,  r61 ,  r62 ,  r63 ,
                   r72 ,  r73 ,  r74 ,  r75 ,  r76 ,  r77 ,  r78 ,  r79 ,  r80 ,  r81 ,  r82 ,  r83 ,  r84 ,  r85 ,  r86 ,  r87 ,  r88 ,  r89 ,  r90 ,  r91 ,  r92 ,  r93 ,  r94 ,  r95 ,
                   r104,  r105,  r106,  r107,  r108,  r109,  r110,  r111,  r112,  r113,  r114,  r115,  r116,  r117,  r118,  r119,  r120,  r121,  r122,  r123,  r124,  r125,  r126,  r127);

        //AddRoundKey
        addRoundKey(r0  , r1  , r2  , r3  , r4  , r5  , r6  , r7  ,
                    r8  , r9  , r10 , r11 , r12 , r13 , r14 , r15 ,
                    r16 , r17 , r18 , r19 , r20 , r21 , r22 , r23 ,
                    r24 , r25 , r26 , r27 , r28 , r29 , r30 , r31 ,
                    r32 , r33 , r34 , r35 , r36 , r37 , r38 , r39 ,
                    r40 , r41 , r42 , r43 , r44 , r45 , r46 , r47 ,
                    r48 , r49 , r50 , r51 , r52 , r53 , r54 , r55 ,
                    r56 , r57 , r58 , r59 , r60 , r61 , r62 , r63 ,
                    r64 , r65 , r66 , r67 , r68 , r69 , r70 , r71 ,
                    r72 , r73 , r74 , r75 , r76 , r77 , r78 , r79 ,
                    r80 , r81 , r82 , r83 , r84 , r85 , r86 , r87 ,
                    r88 , r89 , r90 , r91 , r92 , r93 , r94 , r95 ,
                    r96 , r97 , r98 , r99 , r100, r101, r102, r103,
                    r104, r105, r106, r107, r108, r109, r110, r111,
                    r112, r113, r114, r115, r116, r117, r118, r119,
                    r120, r121, r122, r123, r124, r125, r126, r127,
                    &sm_key[1280]);

        //output
        dev_output[128*i+0]   = r0;
        dev_output[128*i+1]   = r1;
        dev_output[128*i+2]   = r2;
        dev_output[128*i+3]   = r3;
        dev_output[128*i+4]   = r4;
        dev_output[128*i+5]   = r5;
        dev_output[128*i+6]   = r6;
        dev_output[128*i+7]   = r7;
        dev_output[128*i+8]   = r8;
        dev_output[128*i+9]   = r9;    
        dev_output[128*i+10]  = r10; 
        dev_output[128*i+11]  = r11; 
        dev_output[128*i+12]  = r12; 
        dev_output[128*i+13]  = r13; 
        dev_output[128*i+14]  = r14; 
        dev_output[128*i+15]  = r15; 
        dev_output[128*i+16]  = r16; 
        dev_output[128*i+17]  = r17; 
        dev_output[128*i+18]  = r18; 
        dev_output[128*i+19]  = r19; 
        dev_output[128*i+20]  = r20; 
        dev_output[128*i+21]  = r21; 
        dev_output[128*i+22]  = r22; 
        dev_output[128*i+23]  = r23; 
        dev_output[128*i+24]  = r24; 
        dev_output[128*i+25]  = r25; 
        dev_output[128*i+26]  = r26; 
        dev_output[128*i+27]  = r27; 
        dev_output[128*i+28]  = r28; 
        dev_output[128*i+29]  = r29; 
        dev_output[128*i+30]  = r30; 
        dev_output[128*i+31]  = r31; 
        dev_output[128*i+32]  = r32; 
        dev_output[128*i+33]  = r33; 
        dev_output[128*i+34]  = r34; 
        dev_output[128*i+35]  = r35; 
        dev_output[128*i+36]  = r36; 
        dev_output[128*i+37]  = r37; 
        dev_output[128*i+38]  = r38; 
        dev_output[128*i+39]  = r39; 
        dev_output[128*i+40]  = r40; 
        dev_output[128*i+41]  = r41; 
        dev_output[128*i+42]  = r42; 
        dev_output[128*i+43]  = r43; 
        dev_output[128*i+44]  = r44; 
        dev_output[128*i+45]  = r45; 
        dev_output[128*i+46]  = r46; 
        dev_output[128*i+47]  = r47; 
        dev_output[128*i+48]  = r48; 
        dev_output[128*i+49]  = r49; 
        dev_output[128*i+50]  = r50; 
        dev_output[128*i+51]  = r51; 
        dev_output[128*i+52]  = r52; 
        dev_output[128*i+53]  = r53; 
        dev_output[128*i+54]  = r54; 
        dev_output[128*i+55]  = r55; 
        dev_output[128*i+56]  = r56; 
        dev_output[128*i+57]  = r57; 
        dev_output[128*i+58]  = r58; 
        dev_output[128*i+59]  = r59; 
        dev_output[128*i+60]  = r60; 
        dev_output[128*i+61]  = r61; 
        dev_output[128*i+62]  = r62; 
        dev_output[128*i+63]  = r63; 
        dev_output[128*i+64]  = r64; 
        dev_output[128*i+65]  = r65; 
        dev_output[128*i+66]  = r66; 
        dev_output[128*i+67]  = r67; 
        dev_output[128*i+68]  = r68; 
        dev_output[128*i+69]  = r69; 
        dev_output[128*i+70]  = r70; 
        dev_output[128*i+71]  = r71; 
        dev_output[128*i+72]  = r72; 
        dev_output[128*i+73]  = r73; 
        dev_output[128*i+74]  = r74; 
        dev_output[128*i+75]  = r75; 
        dev_output[128*i+76]  = r76; 
        dev_output[128*i+77]  = r77; 
        dev_output[128*i+78]  = r78; 
        dev_output[128*i+79]  = r79; 
        dev_output[128*i+80]  = r80; 
        dev_output[128*i+81]  = r81; 
        dev_output[128*i+82]  = r82; 
        dev_output[128*i+83]  = r83; 
        dev_output[128*i+84]  = r84; 
        dev_output[128*i+85]  = r85; 
        dev_output[128*i+86]  = r86; 
        dev_output[128*i+87]  = r87; 
        dev_output[128*i+88]  = r88; 
        dev_output[128*i+89]  = r89; 
        dev_output[128*i+90]  = r90; 
        dev_output[128*i+91]  = r91; 
        dev_output[128*i+92]  = r92; 
        dev_output[128*i+93]  = r93; 
        dev_output[128*i+94]  = r94; 
        dev_output[128*i+95]  = r95; 
        dev_output[128*i+96]  = r96; 
        dev_output[128*i+97]  = r97; 
        dev_output[128*i+98]  = r98; 
        dev_output[128*i+99]  = r99;
        dev_output[128*i+100] = r100 ;
        dev_output[128*i+101] = r101 ;
        dev_output[128*i+102] = r102 ;
        dev_output[128*i+103] = r103 ;
        dev_output[128*i+104] = r104 ;
        dev_output[128*i+105] = r105 ;
        dev_output[128*i+106] = r106 ;
        dev_output[128*i+107] = r107 ;
        dev_output[128*i+108] = r108 ;
        dev_output[128*i+109] = r109 ;
        dev_output[128*i+110] = r110 ;
        dev_output[128*i+111] = r111 ;
        dev_output[128*i+112] = r112 ;
        dev_output[128*i+113] = r113 ;
        dev_output[128*i+114] = r114 ;
        dev_output[128*i+115] = r115 ;
        dev_output[128*i+116] = r116 ;
        dev_output[128*i+117] = r117 ;
        dev_output[128*i+118] = r118 ;
        dev_output[128*i+119] = r119 ;
        dev_output[128*i+120] = r120 ;
        dev_output[128*i+121] = r121 ;
        dev_output[128*i+122] = r122 ;
        dev_output[128*i+123] = r123 ;
        dev_output[128*i+124] = r124 ;
        dev_output[128*i+125] = r125 ;
        dev_output[128*i+126] = r126 ;
        dev_output[128*i+127] = r127 ;
    }
}

#endif //_ENCRYPT_ECB_H_