#ifndef _ENCRYPT_ECB_H_
#define _ENCRYPT_ECB_H_


#include "BtsUtils.h"
#include "addRoundKey.h"
#include "subBytes.h"
#include "shiftRows.h"
#include "mixColumns.h"

//__device__ inline void addRoundKey(uint32_t &r0 , uint32_t &r1 , uint32_t &r2 , uint32_t &r3 , uint32_t &r4 , uint32_t &r5 , uint32_t &r6 , uint32_t &r7 ,
//                             uint32_t &r8 , uint32_t &r9 , uint32_t &r10, uint32_t &r11, uint32_t &r12, uint32_t &r13, uint32_t &r14, uint32_t &r15,
//                             uint32_t &r16, uint32_t &r17, uint32_t &r18, uint32_t &r19, uint32_t &r20, uint32_t &r21, uint32_t &r22, uint32_t &r23,
//                             uint32_t &r24, uint32_t &r25, uint32_t &r26, uint32_t &r27, uint32_t &r28, uint32_t &r29, uint32_t &r30, uint32_t &r31,
//                             uint32_t key[32]) __attribute__((always_inline));
//__device__ inline void shiftFirstRow(uint32_t &r0 , uint32_t &r1 , uint32_t &r2 , uint32_t &r3 , uint32_t &r4 , uint32_t &r5 , uint32_t &r6 , uint32_t &r7 ,
//                              uint32_t &r8 , uint32_t &r9 , uint32_t &r10, uint32_t &r11, uint32_t &r12, uint32_t &r13, uint32_t &r14, uint32_t &r15,
//                              uint32_t &r16, uint32_t &r17, uint32_t &r18, uint32_t &r19, uint32_t &r20, uint32_t &r21, uint32_t &r22, uint32_t &r23,
//                              uint32_t &r24, uint32_t &r25, uint32_t &r26, uint32_t &r27, uint32_t &r28, uint32_t &r29, uint32_t &r30, uint32_t &r31) __attribute__((always_inline));
//__device__ inline void shiftSecondRow(uint32_t &r0 , uint32_t &r1 , uint32_t &r2 , uint32_t &r3 , uint32_t &r4 , uint32_t &r5 , uint32_t &r6 , uint32_t &r7 ,
//                              uint32_t &r8 , uint32_t &r9 , uint32_t &r10, uint32_t &r11, uint32_t &r12, uint32_t &r13, uint32_t &r14, uint32_t &r15,
//                              uint32_t &r16, uint32_t &r17, uint32_t &r18, uint32_t &r19, uint32_t &r20, uint32_t &r21, uint32_t &r22, uint32_t &r23,
//                              uint32_t &r24, uint32_t &r25, uint32_t &r26, uint32_t &r27, uint32_t &r28, uint32_t &r29, uint32_t &r30, uint32_t &r31) __attribute__((always_inline));
//__device__ inline void shiftThirdRow(uint32_t &r0 , uint32_t &r1 , uint32_t &r2 , uint32_t &r3 , uint32_t &r4 , uint32_t &r5 , uint32_t &r6 , uint32_t &r7 ,
//                              uint32_t &r8 , uint32_t &r9 , uint32_t &r10, uint32_t &r11, uint32_t &r12, uint32_t &r13, uint32_t &r14, uint32_t &r15,
//                              uint32_t &r16, uint32_t &r17, uint32_t &r18, uint32_t &r19, uint32_t &r20, uint32_t &r21, uint32_t &r22, uint32_t &r23,
//                              uint32_t &r24, uint32_t &r25, uint32_t &r26, uint32_t &r27, uint32_t &r28, uint32_t &r29, uint32_t &r30, uint32_t &r31) __attribute__((always_inline));
//__device__ void Sbox(T &r0,T &r1,T &r2,T &r3,T &r4,T &r5,T &r6,T &r7) __attribute__((always_inline));


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
                  x##104, x##105, x##106, x##107, x##108, x##109, x##110, x##111, \
                  x##112, x##113, x##114, x##115, x##116, x##117, x##118, x##119, \
                  x##120, x##121, x##122, x##123, x##124, x##125, x##126, x##127

__global__ void encrypt_Kernel( uint32_t* dev_input, uint32_t* dev_output, size_t inputSize)
{
    //* Index calculations
    int tid             = threadIdx.x;                           //** localID
    int laneID          = tid%warpSize;                          //** laneID 
    int global_warpID   = (blockIdx.x*blockDim.x+tid)/warpSize;  //** global warpID
    int warps           = (blockDim.x*gridDim.x)/warpSize;       //** Warps in the grid
    int elements        = inputSize/16384;                       //** One warp treat 16384 bytes

    for(int i = global_warpID; i < elements; i += warps) {
        //* Loading plaintext
        uint32_t reg128(r);
        r0   = dev_input[i*4096+laneID+0   ];
        r1   = dev_input[i*4096+laneID+32  ];
        r2   = dev_input[i*4096+laneID+64  ];
        r3   = dev_input[i*4096+laneID+96  ];
        r4   = dev_input[i*4096+laneID+128 ];
        r5   = dev_input[i*4096+laneID+160 ];
        r6   = dev_input[i*4096+laneID+192 ];
        r7   = dev_input[i*4096+laneID+224 ];
        r8   = dev_input[i*4096+laneID+256 ];
        r9   = dev_input[i*4096+laneID+288 ];
        r10  = dev_input[i*4096+laneID+320 ];
        r11  = dev_input[i*4096+laneID+352 ];
        r12  = dev_input[i*4096+laneID+384 ];
        r13  = dev_input[i*4096+laneID+416 ];
        r14  = dev_input[i*4096+laneID+448 ];
        r15  = dev_input[i*4096+laneID+480 ];
        r16  = dev_input[i*4096+laneID+512 ];
        r17  = dev_input[i*4096+laneID+544 ];
        r18  = dev_input[i*4096+laneID+576 ];
        r19  = dev_input[i*4096+laneID+608 ];
        r20  = dev_input[i*4096+laneID+640 ];
        r21  = dev_input[i*4096+laneID+672 ];
        r22  = dev_input[i*4096+laneID+704 ];
        r23  = dev_input[i*4096+laneID+736 ];
        r24  = dev_input[i*4096+laneID+768 ];
        r25  = dev_input[i*4096+laneID+800 ];
        r26  = dev_input[i*4096+laneID+832 ];
        r27  = dev_input[i*4096+laneID+864 ];
        r28  = dev_input[i*4096+laneID+896 ];
        r29  = dev_input[i*4096+laneID+928 ];
        r30  = dev_input[i*4096+laneID+960 ];
        r31  = dev_input[i*4096+laneID+992 ];
        r32  = dev_input[i*4096+laneID+1024];
        r33  = dev_input[i*4096+laneID+1056];
        r34  = dev_input[i*4096+laneID+1088];
        r35  = dev_input[i*4096+laneID+1120];
        r36  = dev_input[i*4096+laneID+1152];
        r37  = dev_input[i*4096+laneID+1184];
        r38  = dev_input[i*4096+laneID+1216];
        r39  = dev_input[i*4096+laneID+1248];
        r40  = dev_input[i*4096+laneID+1280];
        r41  = dev_input[i*4096+laneID+1312];
        r42  = dev_input[i*4096+laneID+1344];
        r43  = dev_input[i*4096+laneID+1376];
        r44  = dev_input[i*4096+laneID+1408];
        r45  = dev_input[i*4096+laneID+1440];
        r46  = dev_input[i*4096+laneID+1472];
        r47  = dev_input[i*4096+laneID+1504];
        r48  = dev_input[i*4096+laneID+1536];
        r49  = dev_input[i*4096+laneID+1568];
        r50  = dev_input[i*4096+laneID+1600];
        r51  = dev_input[i*4096+laneID+1632];
        r52  = dev_input[i*4096+laneID+1664];
        r53  = dev_input[i*4096+laneID+1696];
        r54  = dev_input[i*4096+laneID+1728];
        r55  = dev_input[i*4096+laneID+1760];
        r56  = dev_input[i*4096+laneID+1792];
        r57  = dev_input[i*4096+laneID+1824];
        r58  = dev_input[i*4096+laneID+1856];
        r59  = dev_input[i*4096+laneID+1888];
        r60  = dev_input[i*4096+laneID+1920];
        r61  = dev_input[i*4096+laneID+1952];
        r62  = dev_input[i*4096+laneID+1984];
        r63  = dev_input[i*4096+laneID+2016];
        r64  = dev_input[i*4096+laneID+2048];
        r65  = dev_input[i*4096+laneID+2080];
        r66  = dev_input[i*4096+laneID+2112];
        r67  = dev_input[i*4096+laneID+2144];
        r68  = dev_input[i*4096+laneID+2176];
        r69  = dev_input[i*4096+laneID+2208];
        r70  = dev_input[i*4096+laneID+2240];
        r71  = dev_input[i*4096+laneID+2272];
        r72  = dev_input[i*4096+laneID+2304];
        r73  = dev_input[i*4096+laneID+2336];
        r74  = dev_input[i*4096+laneID+2368];
        r75  = dev_input[i*4096+laneID+2400];
        r76  = dev_input[i*4096+laneID+2432];
        r77  = dev_input[i*4096+laneID+2464];
        r78  = dev_input[i*4096+laneID+2496];
        r79  = dev_input[i*4096+laneID+2528];
        r80  = dev_input[i*4096+laneID+2560];
        r81  = dev_input[i*4096+laneID+2592];
        r82  = dev_input[i*4096+laneID+2624];
        r83  = dev_input[i*4096+laneID+2656];
        r84  = dev_input[i*4096+laneID+2688];
        r85  = dev_input[i*4096+laneID+2720];
        r86  = dev_input[i*4096+laneID+2752];
        r87  = dev_input[i*4096+laneID+2784];
        r88  = dev_input[i*4096+laneID+2816];
        r89  = dev_input[i*4096+laneID+2848];
        r90  = dev_input[i*4096+laneID+2880];
        r91  = dev_input[i*4096+laneID+2912];
        r92  = dev_input[i*4096+laneID+2944];
        r93  = dev_input[i*4096+laneID+2976];
        r94  = dev_input[i*4096+laneID+3008];
        r95  = dev_input[i*4096+laneID+3040];
        r96  = dev_input[i*4096+laneID+3072];
        r97  = dev_input[i*4096+laneID+3104];
        r98  = dev_input[i*4096+laneID+3136];
        r99  = dev_input[i*4096+laneID+3168];
        r100 = dev_input[i*4096+laneID+3200];
        r101 = dev_input[i*4096+laneID+3232];
        r102 = dev_input[i*4096+laneID+3264];
        r103 = dev_input[i*4096+laneID+3296];
        r104 = dev_input[i*4096+laneID+3328];
        r105 = dev_input[i*4096+laneID+3360];
        r106 = dev_input[i*4096+laneID+3392];
        r107 = dev_input[i*4096+laneID+3424];
        r108 = dev_input[i*4096+laneID+3456];
        r109 = dev_input[i*4096+laneID+3488];
        r110 = dev_input[i*4096+laneID+3520];
        r111 = dev_input[i*4096+laneID+3552];
        r112 = dev_input[i*4096+laneID+3584];
        r113 = dev_input[i*4096+laneID+3616];
        r114 = dev_input[i*4096+laneID+3648];
        r115 = dev_input[i*4096+laneID+3680];
        r116 = dev_input[i*4096+laneID+3712];
        r117 = dev_input[i*4096+laneID+3744];
        r118 = dev_input[i*4096+laneID+3776];
        r119 = dev_input[i*4096+laneID+3808];
        r120 = dev_input[i*4096+laneID+3840];
        r121 = dev_input[i*4096+laneID+3872];
        r122 = dev_input[i*4096+laneID+3904];
        r123 = dev_input[i*4096+laneID+3936];
        r124 = dev_input[i*4096+laneID+3968];
        r125 = dev_input[i*4096+laneID+4000];
        r126 = dev_input[i*4096+laneID+4032];
        r127 = dev_input[i*4096+laneID+4064];            

        //BIG LOOP
        //for (int j = 1; j < 10; j++) {

            //** SubBytes + Shift
            Sbox(r8 , r9 , r10, r11, r12, r13, r14, r15);
            Sbox(r40, r41, r42, r43, r44, r45, r46, r47);
            Sbox(r72, r73, r74, r75, r76, r77, r78, r79);
            Sbox(r104, r105, r106, r107, r108, r109, r110, r111);


            
            Sbox(r16 , r17 , r18 , r19 , r20 , r21 , r22 , r23);
            Sbox(r48 , r49 , r50 , r51 , r52 , r53 , r54 , r55);
            Sbox(r80 , r81 , r82 , r83 , r84 , r85 , r86 , r87);
            Sbox(r112, r113, r114, r115, r116, r117, r118, r119);



            Sbox(r24 , r25 , r26 , r27 , r28 , r29 , r30 , r31);
            Sbox(r56 , r57 , r58 , r59 , r60 , r61 , r62 , r63);
            Sbox(r88 , r89 , r90 , r91 , r92 , r93 , r94 , r95);
            Sbox(r120, r121, r122, r123, r124, r125, r126, r127);

            
            Sbox(r0 , r1 , r2 , r3 , r4  , r5  , r6  , r7);
            Sbox(r32, r33, r34, r35, r36 , r37 , r38 , r39);
            Sbox(r64, r65, r66, r67, r68 , r69 , r70 , r71);
            Sbox(r96, r97, r98, r99, r100, r101, r102, r103);


        //}

        //** FINAL ROUND
        dev_output[i*4096+laneID+0   ]   = r0 ;
        dev_output[i*4096+laneID+32  ]   = r1 ;
        dev_output[i*4096+laneID+64  ]   = r2 ;
        dev_output[i*4096+laneID+96  ]   = r3 ;
        dev_output[i*4096+laneID+128 ]   = r4 ;
        dev_output[i*4096+laneID+160 ]   = r5 ;
        dev_output[i*4096+laneID+192 ]   = r6 ;
        dev_output[i*4096+laneID+224 ]   = r7 ;
        dev_output[i*4096+laneID+1024]  = r32 ;
        dev_output[i*4096+laneID+1056]  = r33 ;
        dev_output[i*4096+laneID+1088]  = r34 ;
        dev_output[i*4096+laneID+1120]  = r35 ;
        dev_output[i*4096+laneID+1152]  = r36 ;
        dev_output[i*4096+laneID+1184]  = r37 ;
        dev_output[i*4096+laneID+1216]  = r38 ;
        dev_output[i*4096+laneID+1248]  = r39 ;
        dev_output[i*4096+laneID+2048]  = r64 ;
        dev_output[i*4096+laneID+2080]  = r65 ;
        dev_output[i*4096+laneID+2112]  = r66 ;
        dev_output[i*4096+laneID+2144]  = r67 ;
        dev_output[i*4096+laneID+2176]  = r68 ;
        dev_output[i*4096+laneID+2208]  = r69 ;
        dev_output[i*4096+laneID+2240]  = r70 ;
        dev_output[i*4096+laneID+2272]  = r71 ;
        dev_output[i*4096+laneID+3072]  = r96 ;
        dev_output[i*4096+laneID+3104]  = r97 ;
        dev_output[i*4096+laneID+3136]  = r98 ;
        dev_output[i*4096+laneID+3168]  = r99 ;
        dev_output[i*4096+laneID+3200] = r100 ;
        dev_output[i*4096+laneID+3232] = r101 ;
        dev_output[i*4096+laneID+3264] = r102 ;
        dev_output[i*4096+laneID+3296] = r103 ;
        dev_output[i*4096+laneID+256 ]   = r8 ;
        dev_output[i*4096+laneID+288 ]   = r9 ;
        dev_output[i*4096+laneID+320 ]  = r10 ;
        dev_output[i*4096+laneID+352 ]  = r11 ;
        dev_output[i*4096+laneID+384 ]  = r12 ;
        dev_output[i*4096+laneID+416 ]  = r13 ;
        dev_output[i*4096+laneID+448 ]  = r14 ;
        dev_output[i*4096+laneID+480 ]  = r15 ;
        dev_output[i*4096+laneID+1280]  = r40 ;
        dev_output[i*4096+laneID+1312]  = r41 ;
        dev_output[i*4096+laneID+1344]  = r42 ;
        dev_output[i*4096+laneID+1376]  = r43 ;
        dev_output[i*4096+laneID+1408]  = r44 ;
        dev_output[i*4096+laneID+1440]  = r45 ;
        dev_output[i*4096+laneID+1472]  = r46 ;
        dev_output[i*4096+laneID+1504]  = r47 ;
        dev_output[i*4096+laneID+2304]  = r72 ;
        dev_output[i*4096+laneID+2336]  = r73 ;
        dev_output[i*4096+laneID+2368]  = r74 ;
        dev_output[i*4096+laneID+2400]  = r75 ;
        dev_output[i*4096+laneID+2432]  = r76 ;
        dev_output[i*4096+laneID+2464]  = r77 ;
        dev_output[i*4096+laneID+2496]  = r78 ;
        dev_output[i*4096+laneID+2528]  = r79 ;
        dev_output[i*4096+laneID+3328] = r104 ;
        dev_output[i*4096+laneID+3360] = r105 ;
        dev_output[i*4096+laneID+3392] = r106 ;
        dev_output[i*4096+laneID+3424] = r107 ;
        dev_output[i*4096+laneID+3456] = r108 ;
        dev_output[i*4096+laneID+3488] = r109 ;
        dev_output[i*4096+laneID+3520] = r110 ;
        dev_output[i*4096+laneID+3552] = r111 ;
        dev_output[i*4096+laneID+512 ]  = r16 ;
        dev_output[i*4096+laneID+544 ]  = r17 ;
        dev_output[i*4096+laneID+576 ]  = r18 ;
        dev_output[i*4096+laneID+608 ]  = r19 ;
        dev_output[i*4096+laneID+640 ]  = r20 ;
        dev_output[i*4096+laneID+672 ]  = r21 ;
        dev_output[i*4096+laneID+704 ]  = r22 ;
        dev_output[i*4096+laneID+736 ]  = r23 ;
        dev_output[i*4096+laneID+1536]  = r48 ;
        dev_output[i*4096+laneID+1568]  = r49 ;
        dev_output[i*4096+laneID+1600]  = r50 ;
        dev_output[i*4096+laneID+1632]  = r51 ;
        dev_output[i*4096+laneID+1664]  = r52 ;
        dev_output[i*4096+laneID+1696]  = r53 ;
        dev_output[i*4096+laneID+1728]  = r54 ;
        dev_output[i*4096+laneID+1760]  = r55 ;
        dev_output[i*4096+laneID+2560]  = r80 ;
        dev_output[i*4096+laneID+2592]  = r81 ;
        dev_output[i*4096+laneID+2624]  = r82 ;
        dev_output[i*4096+laneID+2656]  = r83 ;
        dev_output[i*4096+laneID+2688]  = r84 ;
        dev_output[i*4096+laneID+2720]  = r85 ;
        dev_output[i*4096+laneID+2752]  = r86 ;
        dev_output[i*4096+laneID+2784]  = r87 ;
        dev_output[i*4096+laneID+3584] = r112 ;
        dev_output[i*4096+laneID+3616] = r113 ;
        dev_output[i*4096+laneID+3648] = r114 ;
        dev_output[i*4096+laneID+3680] = r115 ;
        dev_output[i*4096+laneID+3712] = r116 ;
        dev_output[i*4096+laneID+3744] = r117 ;
        dev_output[i*4096+laneID+3776] = r118 ;
        dev_output[i*4096+laneID+3808] = r119 ;
        dev_output[i*4096+laneID+768 ]  = r24 ;
        dev_output[i*4096+laneID+800 ]  = r25 ;
        dev_output[i*4096+laneID+832 ]  = r26 ;
        dev_output[i*4096+laneID+864 ]  = r27 ;
        dev_output[i*4096+laneID+896 ]  = r28 ;
        dev_output[i*4096+laneID+928 ]  = r29 ;
        dev_output[i*4096+laneID+960 ]  = r30 ;
        dev_output[i*4096+laneID+992 ]  = r31 ;
        dev_output[i*4096+laneID+1792]  = r56 ;
        dev_output[i*4096+laneID+1824]  = r57 ;
        dev_output[i*4096+laneID+1856]  = r58 ;
        dev_output[i*4096+laneID+1888]  = r59 ;
        dev_output[i*4096+laneID+1920]  = r60 ;
        dev_output[i*4096+laneID+1952]  = r61 ;
        dev_output[i*4096+laneID+1984]  = r62 ;
        dev_output[i*4096+laneID+2016]  = r63 ;
        dev_output[i*4096+laneID+2816]  = r88 ;
        dev_output[i*4096+laneID+2848]  = r89 ;
        dev_output[i*4096+laneID+2880]  = r90 ;
        dev_output[i*4096+laneID+2912]  = r91 ;
        dev_output[i*4096+laneID+2944]  = r92 ;
        dev_output[i*4096+laneID+2976]  = r93 ;
        dev_output[i*4096+laneID+3008]  = r94 ;
        dev_output[i*4096+laneID+3040]  = r95 ;
        dev_output[i*4096+laneID+3840] = r120 ;
        dev_output[i*4096+laneID+3872] = r121 ;
        dev_output[i*4096+laneID+3904] = r122 ;
        dev_output[i*4096+laneID+3936] = r123 ;
        dev_output[i*4096+laneID+3968] = r124 ;
        dev_output[i*4096+laneID+4000] = r125 ;
        dev_output[i*4096+laneID+4032] = r126 ;
        dev_output[i*4096+laneID+4064] = r127 ;
    }
}


#endif //_ENCRYPT_ECB_H_
