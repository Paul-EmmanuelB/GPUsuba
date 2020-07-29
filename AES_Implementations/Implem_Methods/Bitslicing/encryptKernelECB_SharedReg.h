#ifndef _ENCRYPT_ECB_H_
#define _ENCRYPT_ECB_H_

#include "BtsUtils.h"
#include "addRoundKey.h"
#include "subBytes.h"
#include "shiftRows.h"
#include "mixColumns.h"

#define reg128(x) x##0  , x##1  , x##2  , x##3  , x##4  , x##5  , x##6  , x##7  , \
                  x##8  , x##9  , x##10 , x##11 , x##12 , x##13 , x##14 , x##15 , \
                  x##16 , x##17 , x##18 , x##19 , x##20 , x##21 , x##22 , x##23 , \
                  x##24 , x##25 , x##26 , x##27 , x##28 , x##29 , x##30 , x##31 , \
                  x##32 , x##33 , x##34 , x##35 , x##36 , x##37 , x##38 , x##39 , \
                  x##40 , x##41 , x##42 , x##43 , x##44 , x##45 , x##46 , x##47 , \
                  x##48 , x##49 , x##50 , x##51 , x##52 , x##53 , x##54 , x##55 , \
                  x##56 , x##57 , x##58 , x##59 , x##60 , x##61 , x##62 , x##63 , \
                  x##64 , x##65 , x##66 , x##67 , x##68 , x##69 , x##70 , x##71 , \
                  x##72 , x##73 , x##74 , x##75 , x##76 , x##77 , x##78 , x##79 , \
                  x##80 , x##81 , x##82 , x##83 , x##84 , x##85 , x##86 , x##87 , \
                  x##88 , x##89 , x##90 , x##91 , x##92 , x##93 , x##94 , x##95 , \
                  x##96 , x##97 , x##98 , x##99 , x##100, x##101, x##102, x##103, \
                  x##104, x##105, x##106, x##107, x##108, x##109, x##110, x##111

__launch_bounds__(64)
__global__ void encrypt_Kernel(uint32_t* dev_input, uint32_t* dev_output, size_t inputSize)
{

    __shared__ uint32_t r[1024];
    //* Index calculations
    int tid = threadIdx.x;                           //** localID
    int laneID = tid & (warpSize - 1);                          //** laneID 
    int global_warpID = (blockIdx.x * blockDim.x + tid) >> 5;  //** global warpID


    //* Loading plaintext
    uint32_t reg128(r);
    r0 = dev_input[global_warpID * 4096 + laneID + 0];
    r1 = dev_input[global_warpID * 4096 + laneID + 32];
    r2 = dev_input[global_warpID * 4096 + laneID + 64];
    r3 = dev_input[global_warpID * 4096 + laneID + 96];
    r4 = dev_input[global_warpID * 4096 + laneID + 128];
    r5 = dev_input[global_warpID * 4096 + laneID + 160];
    r6 = dev_input[global_warpID * 4096 + laneID + 192];
    r7 = dev_input[global_warpID * 4096 + laneID + 224];
    r8 = dev_input[global_warpID * 4096 + laneID + 256];
    r9 = dev_input[global_warpID * 4096 + laneID + 288];
    r10 = dev_input[global_warpID * 4096 + laneID + 320];
    r11 = dev_input[global_warpID * 4096 + laneID + 352];
    r12 = dev_input[global_warpID * 4096 + laneID + 384];
    r13 = dev_input[global_warpID * 4096 + laneID + 416];
    r14 = dev_input[global_warpID * 4096 + laneID + 448];
    r15 = dev_input[global_warpID * 4096 + laneID + 480];
    r16 = dev_input[global_warpID * 4096 + laneID + 512];
    r17 = dev_input[global_warpID * 4096 + laneID + 544];
    r18 = dev_input[global_warpID * 4096 + laneID + 576];
    r19 = dev_input[global_warpID * 4096 + laneID + 608];
    r20 = dev_input[global_warpID * 4096 + laneID + 640];
    r21 = dev_input[global_warpID * 4096 + laneID + 672];
    r22 = dev_input[global_warpID * 4096 + laneID + 704];
    r23 = dev_input[global_warpID * 4096 + laneID + 736];
    r24 = dev_input[global_warpID * 4096 + laneID + 768];
    r25 = dev_input[global_warpID * 4096 + laneID + 800];
    r26 = dev_input[global_warpID * 4096 + laneID + 832];
    r27 = dev_input[global_warpID * 4096 + laneID + 864];
    r28 = dev_input[global_warpID * 4096 + laneID + 896];
    r29 = dev_input[global_warpID * 4096 + laneID + 928];
    r30 = dev_input[global_warpID * 4096 + laneID + 960];
    r31 = dev_input[global_warpID * 4096 + laneID + 992];
    addRoundKey(r0, r1, r2, r3, r4, r5, r6, r7,
        r8, r9, r10, r11, r12, r13, r14, r15,
        r16, r17, r18, r19, r20, r21, r22, r23,
        r24, r25, r26, r27, r28, r29, r30, r31,
        &const_expkey[0]);
    r32 = dev_input[global_warpID * 4096 + laneID + 1024];
    r33 = dev_input[global_warpID * 4096 + laneID + 1056];
    r34 = dev_input[global_warpID * 4096 + laneID + 1088];
    r35 = dev_input[global_warpID * 4096 + laneID + 1120];
    r36 = dev_input[global_warpID * 4096 + laneID + 1152];
    r37 = dev_input[global_warpID * 4096 + laneID + 1184];
    r38 = dev_input[global_warpID * 4096 + laneID + 1216];
    r39 = dev_input[global_warpID * 4096 + laneID + 1248];
    r40 = dev_input[global_warpID * 4096 + laneID + 1280];
    r41 = dev_input[global_warpID * 4096 + laneID + 1312];
    r42 = dev_input[global_warpID * 4096 + laneID + 1344];
    r43 = dev_input[global_warpID * 4096 + laneID + 1376];
    r44 = dev_input[global_warpID * 4096 + laneID + 1408];
    r45 = dev_input[global_warpID * 4096 + laneID + 1440];
    r46 = dev_input[global_warpID * 4096 + laneID + 1472];
    r47 = dev_input[global_warpID * 4096 + laneID + 1504];
    r48 = dev_input[global_warpID * 4096 + laneID + 1536];
    r49 = dev_input[global_warpID * 4096 + laneID + 1568];
    r50 = dev_input[global_warpID * 4096 + laneID + 1600];
    r51 = dev_input[global_warpID * 4096 + laneID + 1632];
    r52 = dev_input[global_warpID * 4096 + laneID + 1664];
    r53 = dev_input[global_warpID * 4096 + laneID + 1696];
    r54 = dev_input[global_warpID * 4096 + laneID + 1728];
    r55 = dev_input[global_warpID * 4096 + laneID + 1760];
    r56 = dev_input[global_warpID * 4096 + laneID + 1792];
    r57 = dev_input[global_warpID * 4096 + laneID + 1824];
    r58 = dev_input[global_warpID * 4096 + laneID + 1856];
    r59 = dev_input[global_warpID * 4096 + laneID + 1888];
    r60 = dev_input[global_warpID * 4096 + laneID + 1920];
    r61 = dev_input[global_warpID * 4096 + laneID + 1952];
    r62 = dev_input[global_warpID * 4096 + laneID + 1984];
    r63 = dev_input[global_warpID * 4096 + laneID + 2016];
    addRoundKey(r32, r33, r34, r35, r36, r37, r38, r39,
        r40, r41, r42, r43, r44, r45, r46, r47,
        r48, r49, r50, r51, r52, r53, r54, r55,
        r56, r57, r58, r59, r60, r61, r62, r63,
        &const_expkey[32]);
    r64 = dev_input[global_warpID * 4096 + laneID + 2048];
    r65 = dev_input[global_warpID * 4096 + laneID + 2080];
    r66 = dev_input[global_warpID * 4096 + laneID + 2112];
    r67 = dev_input[global_warpID * 4096 + laneID + 2144];
    r68 = dev_input[global_warpID * 4096 + laneID + 2176];
    r69 = dev_input[global_warpID * 4096 + laneID + 2208];
    r70 = dev_input[global_warpID * 4096 + laneID + 2240];
    r71 = dev_input[global_warpID * 4096 + laneID + 2272];
    r72 = dev_input[global_warpID * 4096 + laneID + 2304];
    r73 = dev_input[global_warpID * 4096 + laneID + 2336];
    r74 = dev_input[global_warpID * 4096 + laneID + 2368];
    r75 = dev_input[global_warpID * 4096 + laneID + 2400];
    r76 = dev_input[global_warpID * 4096 + laneID + 2432];
    r77 = dev_input[global_warpID * 4096 + laneID + 2464];
    r78 = dev_input[global_warpID * 4096 + laneID + 2496];
    r79 = dev_input[global_warpID * 4096 + laneID + 2528];
    r80 = dev_input[global_warpID * 4096 + laneID + 2560];
    r81 = dev_input[global_warpID * 4096 + laneID + 2592];
    r82 = dev_input[global_warpID * 4096 + laneID + 2624];
    r83 = dev_input[global_warpID * 4096 + laneID + 2656];
    r84 = dev_input[global_warpID * 4096 + laneID + 2688];
    r85 = dev_input[global_warpID * 4096 + laneID + 2720];
    r86 = dev_input[global_warpID * 4096 + laneID + 2752];
    r87 = dev_input[global_warpID * 4096 + laneID + 2784];
    r88 = dev_input[global_warpID * 4096 + laneID + 2816];
    r89 = dev_input[global_warpID * 4096 + laneID + 2848];
    r90 = dev_input[global_warpID * 4096 + laneID + 2880];
    r91 = dev_input[global_warpID * 4096 + laneID + 2912];
    r92 = dev_input[global_warpID * 4096 + laneID + 2944];
    r93 = dev_input[global_warpID * 4096 + laneID + 2976];
    r94 = dev_input[global_warpID * 4096 + laneID + 3008];
    r95 = dev_input[global_warpID * 4096 + laneID + 3040];
    addRoundKey(r64, r65, r66, r67, r68, r69, r70, r71,
        r72, r73, r74, r75, r76, r77, r78, r79,
        r80, r81, r82, r83, r84, r85, r86, r87,
        r88, r89, r90, r91, r92, r93, r94, r95,
        &const_expkey[64]);
    r96 = dev_input[global_warpID * 4096 + laneID + 3072];
    r97 = dev_input[global_warpID * 4096 + laneID + 3104];
    r98 = dev_input[global_warpID * 4096 + laneID + 3136];
    r99 = dev_input[global_warpID * 4096 + laneID + 3168];
    r100 = dev_input[global_warpID * 4096 + laneID + 3200];
    r101 = dev_input[global_warpID * 4096 + laneID + 3232];
    r102 = dev_input[global_warpID * 4096 + laneID + 3264];
    r103 = dev_input[global_warpID * 4096 + laneID + 3296];
    r104 = dev_input[global_warpID * 4096 + laneID + 3328];
    r105 = dev_input[global_warpID * 4096 + laneID + 3360];
    r106 = dev_input[global_warpID * 4096 + laneID + 3392];
    r107 = dev_input[global_warpID * 4096 + laneID + 3424];
    r108 = dev_input[global_warpID * 4096 + laneID + 3456];
    r109 = dev_input[global_warpID * 4096 + laneID + 3488];
    r110 = dev_input[global_warpID * 4096 + laneID + 3520];
    r111 = dev_input[global_warpID * 4096 + laneID + 3552];
    r[tid + 0] = dev_input[global_warpID * 4096 + laneID + 3584];
    r[tid + 64] = dev_input[global_warpID * 4096 + laneID + 3616];
    r[tid + 128] = dev_input[global_warpID * 4096 + laneID + 3648];
    r[tid + 192] = dev_input[global_warpID * 4096 + laneID + 3680];
    r[tid + 256] = dev_input[global_warpID * 4096 + laneID + 3712];
    r[tid + 320] = dev_input[global_warpID * 4096 + laneID + 3744];
    r[tid + 384] = dev_input[global_warpID * 4096 + laneID + 3776];
    r[tid + 448] = dev_input[global_warpID * 4096 + laneID + 3808];
    r[tid + 512] = dev_input[global_warpID * 4096 + laneID + 3840];
    r[tid + 576] = dev_input[global_warpID * 4096 + laneID + 3872];
    r[tid + 640] = dev_input[global_warpID * 4096 + laneID + 3904];
    r[tid + 704] = dev_input[global_warpID * 4096 + laneID + 3936];
    r[tid + 768] = dev_input[global_warpID * 4096 + laneID + 3968];
    r[tid + 832] = dev_input[global_warpID * 4096 + laneID + 4000];
    r[tid + 896] = dev_input[global_warpID * 4096 + laneID + 4032];
    r[tid + 960] = dev_input[global_warpID * 4096 + laneID + 4064];
    addRoundKey(r96, r97, r98, r99, r100, r101, r102, r103,
        r104, r105, r106, r107, r108, r109, r110, r111,
        r[tid + 0], r[tid + 64], r[tid + 128], r[tid + 192], r[tid + 256], r[tid + 320], r[tid + 384], r[tid + 448],
        r[tid + 512], r[tid + 576], r[tid + 640], r[tid + 704], r[tid + 768], r[tid + 832], r[tid + 896], r[tid + 960],
        &const_expkey[96]);


    //BIG LOOP
    #pragma unroll(9)
    for (int j = 1; j < 10; j++) {

        //** SubBytes + Shift
        Sbox(r8, r9, r10, r11, r12, r13, r14, r15);
        Sbox(r40, r41, r42, r43, r44, r45, r46, r47);
        Sbox(r72, r73, r74, r75, r76, r77, r78, r79);
        Sbox(r104, r105, r106, r107, r108, r109, r110, r111);
        shiftFirstRow(r8, r9, r10, r11, r12, r13, r14, r15,
            r40, r41, r42, r43, r44, r45, r46, r47,
            r72, r73, r74, r75, r76, r77, r78, r79,
            r104, r105, r106, r107, r108, r109, r110, r111);


        Sbox(r16, r17, r18, r19, r20, r21, r22, r23);
        Sbox(r48, r49, r50, r51, r52, r53, r54, r55);
        Sbox(r80, r81, r82, r83, r84, r85, r86, r87);
        Sbox(r[tid + 0], r[tid + 64], r[tid + 128], r[tid + 192], r[tid + 256], r[tid + 320], r[tid + 384], r[tid + 448]);
        shiftSecondRow(r16, r17, r18, r19, r20, r21, r22, r23,
            r80, r81, r82, r83, r84, r85, r86, r87,
            r48, r49, r50, r51, r52, r53, r54, r55,
            r[tid + 0], r[tid + 64], r[tid + 128], r[tid + 192], r[tid + 256], r[tid + 320], r[tid + 384], r[tid + 448]);


        Sbox(r24, r25, r26, r27, r28, r29, r30, r31);
        Sbox(r56, r57, r58, r59, r60, r61, r62, r63);
        Sbox(r88, r89, r90, r91, r92, r93, r94, r95);
        Sbox(r[tid + 512], r[tid + 576], r[tid + 640], r[tid + 704], r[tid + 768], r[tid + 832], r[tid + 896], r[tid + 960]);
        shiftThirdRow(r24, r25, r26, r27, r28, r29, r30, r31,
            r[tid + 512], r[tid + 576], r[tid + 640], r[tid + 704], r[tid + 768], r[tid + 832], r[tid + 896], r[tid + 960],
            r88, r89, r90, r91, r92, r93, r94, r95,
            r56, r57, r58, r59, r60, r61, r62, r63);


        Sbox(r0, r1, r2, r3, r4, r5, r6, r7);
        Sbox(r32, r33, r34, r35, r36, r37, r38, r39);
        Sbox(r64, r65, r66, r67, r68, r69, r70, r71);
        Sbox(r96, r97, r98, r99, r100, r101, r102, r103);

        //** Mix + RoundKey
        //#pragma unroll(4)
        //for(int l=0; l<4; l++){
        //    Mixcl( &r[l*32 ] );
        //    addRoundKey( &r[l*32 ],  &const_expkey[128*j+l*32]);
        //}
        Mixcl(r0, r1, r2, r3, r4, r5, r6, r7,
            r8, r9, r10, r11, r12, r13, r14, r15,
            r16, r17, r18, r19, r20, r21, r22, r23,
            r24, r25, r26, r27, r28, r29, r30, r31);
        addRoundKey(r0, r1, r2, r3, r4, r5, r6, r7,
            r8, r9, r10, r11, r12, r13, r14, r15,
            r16, r17, r18, r19, r20, r21, r22, r23,
            r24, r25, r26, r27, r28, r29, r30, r31,
            &const_expkey[128 * j + 0]);
        Mixcl(r32, r33, r34, r35, r36, r37, r38, r39,
            r40, r41, r42, r43, r44, r45, r46, r47,
            r48, r49, r50, r51, r52, r53, r54, r55,
            r56, r57, r58, r59, r60, r61, r62, r63);
        addRoundKey(r32, r33, r34, r35, r36, r37, r38, r39,
            r40, r41, r42, r43, r44, r45, r46, r47,
            r48, r49, r50, r51, r52, r53, r54, r55,
            r56, r57, r58, r59, r60, r61, r62, r63,
            &const_expkey[128 * j + 32]);
        Mixcl(r64, r65, r66, r67, r68, r69, r70, r71,
            r72, r73, r74, r75, r76, r77, r78, r79,
            r80, r81, r82, r83, r84, r85, r86, r87,
            r88, r89, r90, r91, r92, r93, r94, r95);
        addRoundKey(r64, r65, r66, r67, r68, r69, r70, r71,
            r72, r73, r74, r75, r76, r77, r78, r79,
            r80, r81, r82, r83, r84, r85, r86, r87,
            r88, r89, r90, r91, r92, r93, r94, r95,
            &const_expkey[128 * j + 64]);
        Mixcl(r96, r97, r98, r99, r100, r101, r102, r103,
            r104, r105, r106, r107, r108, r109, r110, r111,
            r[tid + 0], r[tid + 64], r[tid + 128], r[tid + 192], r[tid + 256], r[tid + 320], r[tid + 384], r[tid + 448],
            r[tid + 512], r[tid + 576], r[tid + 640], r[tid + 704], r[tid + 768], r[tid + 832], r[tid + 896], r[tid + 960]);
        addRoundKey(r96, r97, r98, r99, r100, r101, r102, r103,
            r104, r105, r106, r107, r108, r109, r110, r111,
            r[tid + 0], r[tid + 64], r[tid + 128], r[tid + 192], r[tid + 256], r[tid + 320], r[tid + 384], r[tid + 448],
            r[tid + 512], r[tid + 576], r[tid + 640], r[tid + 704], r[tid + 768], r[tid + 832], r[tid + 896], r[tid + 960],
            &const_expkey[128 * j + 96]);
    }

    //** FINAL ROUND
    Sbox(r0, r1, r2, r3, r4, r5, r6, r7);
    addRoundKey(r0, r1, r2, r3, r4, r5, r6, r7, &const_expkey[1280 + 0]);
    Sbox(r32, r33, r34, r35, r36, r37, r38, r39);
    addRoundKey(r32, r33, r34, r35, r36, r37, r38, r39, &const_expkey[1280 + 32]);
    Sbox(r64, r65, r66, r67, r68, r69, r70, r71);
    addRoundKey(r64, r65, r66, r67, r68, r69, r70, r71, &const_expkey[1280 + 64]);
    Sbox(r96, r97, r98, r99, r100, r101, r102, r103);
    addRoundKey(r96, r97, r98, r99, r100, r101, r102, r103, &const_expkey[1280 + 96]);
    dev_output[global_warpID * 4096 + laneID + 0] = r0;
    dev_output[global_warpID * 4096 + laneID + 32] = r1;
    dev_output[global_warpID * 4096 + laneID + 64] = r2;
    dev_output[global_warpID * 4096 + laneID + 96] = r3;
    dev_output[global_warpID * 4096 + laneID + 128] = r4;
    dev_output[global_warpID * 4096 + laneID + 160] = r5;
    dev_output[global_warpID * 4096 + laneID + 192] = r6;
    dev_output[global_warpID * 4096 + laneID + 224] = r7;
    dev_output[global_warpID * 4096 + laneID + 1024] = r32;
    dev_output[global_warpID * 4096 + laneID + 1056] = r33;
    dev_output[global_warpID * 4096 + laneID + 1088] = r34;
    dev_output[global_warpID * 4096 + laneID + 1120] = r35;
    dev_output[global_warpID * 4096 + laneID + 1152] = r36;
    dev_output[global_warpID * 4096 + laneID + 1184] = r37;
    dev_output[global_warpID * 4096 + laneID + 1216] = r38;
    dev_output[global_warpID * 4096 + laneID + 1248] = r39;
    dev_output[global_warpID * 4096 + laneID + 2048] = r64;
    dev_output[global_warpID * 4096 + laneID + 2080] = r65;
    dev_output[global_warpID * 4096 + laneID + 2112] = r66;
    dev_output[global_warpID * 4096 + laneID + 2144] = r67;
    dev_output[global_warpID * 4096 + laneID + 2176] = r68;
    dev_output[global_warpID * 4096 + laneID + 2208] = r69;
    dev_output[global_warpID * 4096 + laneID + 2240] = r70;
    dev_output[global_warpID * 4096 + laneID + 2272] = r71;
    dev_output[global_warpID * 4096 + laneID + 3072] = r96;
    dev_output[global_warpID * 4096 + laneID + 3104] = r97;
    dev_output[global_warpID * 4096 + laneID + 3136] = r98;
    dev_output[global_warpID * 4096 + laneID + 3168] = r99;
    dev_output[global_warpID * 4096 + laneID + 3200] = r100;
    dev_output[global_warpID * 4096 + laneID + 3232] = r101;
    dev_output[global_warpID * 4096 + laneID + 3264] = r102;
    dev_output[global_warpID * 4096 + laneID + 3296] = r103;


    Sbox(r8, r9, r10, r11, r12, r13, r14, r15);
    Sbox(r40, r41, r42, r43, r44, r45, r46, r47);
    Sbox(r72, r73, r74, r75, r76, r77, r78, r79);
    Sbox(r104, r105, r106, r107, r108, r109, r110, r111);
    shiftFirstRow(r8, r9, r10, r11, r12, r13, r14, r15,
        r40, r41, r42, r43, r44, r45, r46, r47,
        r72, r73, r74, r75, r76, r77, r78, r79,
        r104, r105, r106, r107, r108, r109, r110, r111);
    addRoundKey(r8, r9, r10, r11, r12, r13, r14, r15, &const_expkey[1280 + 8]);
    dev_output[global_warpID * 4096 + laneID + 256] = r8;
    dev_output[global_warpID * 4096 + laneID + 288] = r9;
    dev_output[global_warpID * 4096 + laneID + 320] = r10;
    dev_output[global_warpID * 4096 + laneID + 352] = r11;
    dev_output[global_warpID * 4096 + laneID + 384] = r12;
    dev_output[global_warpID * 4096 + laneID + 416] = r13;
    dev_output[global_warpID * 4096 + laneID + 448] = r14;
    dev_output[global_warpID * 4096 + laneID + 480] = r15;
    addRoundKey(r40, r41, r42, r43, r44, r45, r46, r47, &const_expkey[1280 + 40]);
    dev_output[global_warpID * 4096 + laneID + 1280] = r40;
    dev_output[global_warpID * 4096 + laneID + 1312] = r41;
    dev_output[global_warpID * 4096 + laneID + 1344] = r42;
    dev_output[global_warpID * 4096 + laneID + 1376] = r43;
    dev_output[global_warpID * 4096 + laneID + 1408] = r44;
    dev_output[global_warpID * 4096 + laneID + 1440] = r45;
    dev_output[global_warpID * 4096 + laneID + 1472] = r46;
    dev_output[global_warpID * 4096 + laneID + 1504] = r47;
    addRoundKey(r72, r73, r74, r75, r76, r77, r78, r79, &const_expkey[1280 + 72]);
    dev_output[global_warpID * 4096 + laneID + 2304] = r72;
    dev_output[global_warpID * 4096 + laneID + 2336] = r73;
    dev_output[global_warpID * 4096 + laneID + 2368] = r74;
    dev_output[global_warpID * 4096 + laneID + 2400] = r75;
    dev_output[global_warpID * 4096 + laneID + 2432] = r76;
    dev_output[global_warpID * 4096 + laneID + 2464] = r77;
    dev_output[global_warpID * 4096 + laneID + 2496] = r78;
    dev_output[global_warpID * 4096 + laneID + 2528] = r79;
    addRoundKey(r104, r105, r106, r107, r108, r109, r110, r111, &const_expkey[1280 + 104]);
    dev_output[global_warpID * 4096 + laneID + 3328] = r104;
    dev_output[global_warpID * 4096 + laneID + 3360] = r105;
    dev_output[global_warpID * 4096 + laneID + 3392] = r106;
    dev_output[global_warpID * 4096 + laneID + 3424] = r107;
    dev_output[global_warpID * 4096 + laneID + 3456] = r108;
    dev_output[global_warpID * 4096 + laneID + 3488] = r109;
    dev_output[global_warpID * 4096 + laneID + 3520] = r110;
    dev_output[global_warpID * 4096 + laneID + 3552] = r111;


    Sbox(r16, r17, r18, r19, r20, r21, r22, r23);
    Sbox(r48, r49, r50, r51, r52, r53, r54, r55);
    Sbox(r80, r81, r82, r83, r84, r85, r86, r87);
    Sbox(r[tid + 0], r[tid + 64], r[tid + 128], r[tid + 192], r[tid + 256], r[tid + 320], r[tid + 384], r[tid + 448]);
    shiftSecondRow(r16, r17, r18, r19, r20, r21, r22, r23,
        r80, r81, r82, r83, r84, r85, r86, r87,
        r48, r49, r50, r51, r52, r53, r54, r55,
        r[tid + 0], r[tid + 64], r[tid + 128], r[tid + 192], r[tid + 256], r[tid + 320], r[tid + 384], r[tid + 448]);
    addRoundKey(r16, r17, r18, r19, r20, r21, r22, r23, &const_expkey[1280 + 16]);
    dev_output[global_warpID * 4096 + laneID + 512] = r16;
    dev_output[global_warpID * 4096 + laneID + 544] = r17;
    dev_output[global_warpID * 4096 + laneID + 576] = r18;
    dev_output[global_warpID * 4096 + laneID + 608] = r19;
    dev_output[global_warpID * 4096 + laneID + 640] = r20;
    dev_output[global_warpID * 4096 + laneID + 672] = r21;
    dev_output[global_warpID * 4096 + laneID + 704] = r22;
    dev_output[global_warpID * 4096 + laneID + 736] = r23;
    addRoundKey(r48, r49, r50, r51, r52, r53, r54, r55, &const_expkey[1280 + 48]);
    dev_output[global_warpID * 4096 + laneID + 1536] = r48;
    dev_output[global_warpID * 4096 + laneID + 1568] = r49;
    dev_output[global_warpID * 4096 + laneID + 1600] = r50;
    dev_output[global_warpID * 4096 + laneID + 1632] = r51;
    dev_output[global_warpID * 4096 + laneID + 1664] = r52;
    dev_output[global_warpID * 4096 + laneID + 1696] = r53;
    dev_output[global_warpID * 4096 + laneID + 1728] = r54;
    dev_output[global_warpID * 4096 + laneID + 1760] = r55;
    addRoundKey(r80, r81, r82, r83, r84, r85, r86, r87, &const_expkey[1280 + 80]);
    dev_output[global_warpID * 4096 + laneID + 2560] = r80;
    dev_output[global_warpID * 4096 + laneID + 2592] = r81;
    dev_output[global_warpID * 4096 + laneID + 2624] = r82;
    dev_output[global_warpID * 4096 + laneID + 2656] = r83;
    dev_output[global_warpID * 4096 + laneID + 2688] = r84;
    dev_output[global_warpID * 4096 + laneID + 2720] = r85;
    dev_output[global_warpID * 4096 + laneID + 2752] = r86;
    dev_output[global_warpID * 4096 + laneID + 2784] = r87;
    addRoundKey(r[tid + 0], r[tid + 64], r[tid + 128], r[tid + 192], r[tid + 256], r[tid + 320], r[tid + 384], r[tid + 448], &const_expkey[1280 + 112]);
    dev_output[global_warpID * 4096 + laneID + 3584] = r[tid + 0];
    dev_output[global_warpID * 4096 + laneID + 3616] = r[tid + 64];
    dev_output[global_warpID * 4096 + laneID + 3648] = r[tid + 128];
    dev_output[global_warpID * 4096 + laneID + 3680] = r[tid + 192];
    dev_output[global_warpID * 4096 + laneID + 3712] = r[tid + 256];
    dev_output[global_warpID * 4096 + laneID + 3744] = r[tid + 320];
    dev_output[global_warpID * 4096 + laneID + 3776] = r[tid + 384];
    dev_output[global_warpID * 4096 + laneID + 3808] = r[tid + 448];


    Sbox(r24, r25, r26, r27, r28, r29, r30, r31);
    Sbox(r56, r57, r58, r59, r60, r61, r62, r63);
    Sbox(r88, r89, r90, r91, r92, r93, r94, r95);
    Sbox(r[tid + 512], r[tid + 576], r[tid + 640], r[tid + 704], r[tid + 768], r[tid + 832], r[tid + 896], r[tid + 960]);
    shiftThirdRow(r24, r25, r26, r27, r28, r29, r30, r31,
        r[tid + 512], r[tid + 576], r[tid + 640], r[tid + 704], r[tid + 768], r[tid + 832], r[tid + 896], r[tid + 960],
        r88, r89, r90, r91, r92, r93, r94, r95,
        r56, r57, r58, r59, r60, r61, r62, r63);
    addRoundKey(r24, r25, r26, r27, r28, r29, r30, r31, &const_expkey[1280 + 24]);
    dev_output[global_warpID * 4096 + laneID + 768] = r24;
    dev_output[global_warpID * 4096 + laneID + 800] = r25;
    dev_output[global_warpID * 4096 + laneID + 832] = r26;
    dev_output[global_warpID * 4096 + laneID + 864] = r27;
    dev_output[global_warpID * 4096 + laneID + 896] = r28;
    dev_output[global_warpID * 4096 + laneID + 928] = r29;
    dev_output[global_warpID * 4096 + laneID + 960] = r30;
    dev_output[global_warpID * 4096 + laneID + 992] = r31;
    addRoundKey(r56, r57, r58, r59, r60, r61, r62, r63, &const_expkey[1280 + 56]);
    dev_output[global_warpID * 4096 + laneID + 1792] = r56;
    dev_output[global_warpID * 4096 + laneID + 1824] = r57;
    dev_output[global_warpID * 4096 + laneID + 1856] = r58;
    dev_output[global_warpID * 4096 + laneID + 1888] = r59;
    dev_output[global_warpID * 4096 + laneID + 1920] = r60;
    dev_output[global_warpID * 4096 + laneID + 1952] = r61;
    dev_output[global_warpID * 4096 + laneID + 1984] = r62;
    dev_output[global_warpID * 4096 + laneID + 2016] = r63;
    addRoundKey(r88, r89, r90, r91, r92, r93, r94, r95, &const_expkey[1280 + 88]);
    dev_output[global_warpID * 4096 + laneID + 2816] = r88;
    dev_output[global_warpID * 4096 + laneID + 2848] = r89;
    dev_output[global_warpID * 4096 + laneID + 2880] = r90;
    dev_output[global_warpID * 4096 + laneID + 2912] = r91;
    dev_output[global_warpID * 4096 + laneID + 2944] = r92;
    dev_output[global_warpID * 4096 + laneID + 2976] = r93;
    dev_output[global_warpID * 4096 + laneID + 3008] = r94;
    dev_output[global_warpID * 4096 + laneID + 3040] = r95;
    addRoundKey(r[tid + 512], r[tid + 576], r[tid + 640], r[tid + 704], r[tid + 768], r[tid + 832], r[tid + 896], r[tid + 960], &const_expkey[1280 + 120]);
    dev_output[global_warpID * 4096 + laneID + 3840] = r[tid + 512];
    dev_output[global_warpID * 4096 + laneID + 3872] = r[tid + 576];
    dev_output[global_warpID * 4096 + laneID + 3904] = r[tid + 640];
    dev_output[global_warpID * 4096 + laneID + 3936] = r[tid + 704];
    dev_output[global_warpID * 4096 + laneID + 3968] = r[tid + 768];
    dev_output[global_warpID * 4096 + laneID + 4000] = r[tid + 832];
    dev_output[global_warpID * 4096 + laneID + 4032] = r[tid + 896];
    dev_output[global_warpID * 4096 + laneID + 4064] = r[tid + 960];
}

#endif //ENCRYPT