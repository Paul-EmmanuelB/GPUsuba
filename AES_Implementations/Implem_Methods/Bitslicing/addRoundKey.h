#ifndef _ADD_ROUND_KEY_H_
#define _ADD_ROUND_KEY_H_

//test with only one value
template<typename T>
__device__  __forceinline__ void addRoundKey(T &r0 , T &r1 , T &r2 , T &r3 , T &r4 , T &r5 , T &r6 , T &r7 ,
                             T &r8 , T &r9 , T &r10, T &r11, T &r12, T &r13, T &r14, T &r15,
                             T &r16, T &r17, T &r18, T &r19, T &r20, T &r21, T &r22, T &r23,
                             T &r24, T &r25, T &r26, T &r27, T &r28, T &r29, T &r30, T &r31,
                             int val){
    r0   ^= val;
    r1   ^= val;
    r2   ^= val;
    r3   ^= val;
    r4   ^= val;
    r5   ^= val;
    r6   ^= val;
    r7   ^= val;
    r8   ^= val;
    r9   ^= val;
    r10  ^= val;
    r11  ^= val;
    r12  ^= val;
    r13  ^= val;
    r14  ^= val;
    r15  ^= val;
    r16  ^= val;
    r17  ^= val;
    r18  ^= val;
    r19  ^= val;
    r20  ^= val;
    r21  ^= val;
    r22  ^= val;
    r23  ^= val;
    r24  ^= val;
    r25  ^= val;
    r26  ^= val;
    r27  ^= val;
    r28  ^= val;
    r29  ^= val;
    r30  ^= val;
    r31  ^= val;
}



template<typename T>
__device__  __forceinline__ void addRoundKey(T &r0 , T &r1 , T &r2 , T &r3 , T &r4 , T &r5 , T &r6 , T &r7 ,
                             int val){
    r0   ^= val;
    r1   ^= val;
    r2   ^= val;
    r3   ^= val;
    r4   ^= val;
    r5   ^= val;
    r6   ^= val;
    r7   ^= val;
}

//Function with registers
template<typename T>
__device__ __forceinline__ void addRoundKey(T &cipher, T key) {
    cipher   ^= key;
}

template<typename T>
__device__  __forceinline__ void addRoundKey(T &r0 , T &r1 , T &r2 , T &r3 , T &r4 , T &r5 , T &r6 , T &r7 ,
                             T &r8 , T &r9 , T &r10, T &r11, T &r12, T &r13, T &r14, T &r15,
                             T &r16, T &r17, T &r18, T &r19, T &r20, T &r21, T &r22, T &r23,
                             T &r24, T &r25, T &r26, T &r27, T &r28, T &r29, T &r30, T &r31,
                             T key[32]){
    r0   ^= key[0  ];
    r1   ^= key[1  ];
    r2   ^= key[2  ];
    r3   ^= key[3  ];
    r4   ^= key[4  ];
    r5   ^= key[5  ];
    r6   ^= key[6  ];
    r7   ^= key[7  ];
    r8   ^= key[8  ];
    r9   ^= key[9  ];
    r10  ^= key[10 ];
    r11  ^= key[11 ];
    r12  ^= key[12 ];
    r13  ^= key[13 ];
    r14  ^= key[14 ];
    r15  ^= key[15 ];
    r16  ^= key[16 ];
    r17  ^= key[17 ];
    r18  ^= key[18 ];
    r19  ^= key[19 ];
    r20  ^= key[20 ];
    r21  ^= key[21 ];
    r22  ^= key[22 ];
    r23  ^= key[23 ];
    r24  ^= key[24 ];
    r25  ^= key[25 ];
    r26  ^= key[26 ];
    r27  ^= key[27 ];
    r28  ^= key[28 ];
    r29  ^= key[29 ];
    r30  ^= key[30 ];
    r31  ^= key[31 ];
}

template<typename T>
__device__ __forceinline__ void addRoundKey(T &r0 , T &r1 , T &r2 , T &r3 , T &r4 , T &r5 , T &r6 , T &r7 ,
                             T key[8]){
    r0   ^= key[0  ];
    r1   ^= key[1  ];
    r2   ^= key[2  ];
    r3   ^= key[3  ];
    r4   ^= key[4  ];
    r5   ^= key[5  ];
    r6   ^= key[6  ];
    r7   ^= key[7  ];
}

template<typename T>
__device__ __forceinline__ void addRoundKey(T &r0  , T &r1  , T &r2  , T &r3  , T &r4  , T &r5  , T &r6  , T &r7  ,
                 T &r8  , T &r9  , T &r10 , T &r11 , T &r12 , T &r13 , T &r14 , T &r15 ,
                 T &r16 , T &r17 , T &r18 , T &r19 , T &r20 , T &r21 , T &r22 , T &r23 ,
                 T &r24 , T &r25 , T &r26 , T &r27 , T &r28 , T &r29 , T &r30 , T &r31 ,
                 T &r32 , T &r33 , T &r34 , T &r35 , T &r36 , T &r37 , T &r38 , T &r39 ,
                 T &r40 , T &r41 , T &r42 , T &r43 , T &r44 , T &r45 , T &r46 , T &r47 ,
                 T &r48 , T &r49 , T &r50 , T &r51 , T &r52 , T &r53 , T &r54 , T &r55 ,
                 T &r56 , T &r57 , T &r58 , T &r59 , T &r60 , T &r61 , T &r62 , T &r63 ,
                 T &r64 , T &r65 , T &r66 , T &r67 , T &r68 , T &r69 , T &r70 , T &r71 ,
                 T &r72 , T &r73 , T &r74 , T &r75 , T &r76 , T &r77 , T &r78 , T &r79 ,
                 T &r80 , T &r81 , T &r82 , T &r83 , T &r84 , T &r85 , T &r86 , T &r87 ,
                 T &r88 , T &r89 , T &r90 , T &r91 , T &r92 , T &r93 , T &r94 , T &r95 ,
                 T &r96 , T &r97 , T &r98 , T &r99 , T &r100, T &r101, T &r102, T &r103,
                 T &r104, T &r105, T &r106, T &r107, T &r108, T &r109, T &r110, T &r111,
                 T &r112, T &r113, T &r114, T &r115, T &r116, T &r117, T &r118, T &r119,
                 T &r120, T &r121, T &r122, T &r123, T &r124, T &r125, T &r126, T &r127,
                 uint32_t key[128]) {
    r0   ^= key[0  ];
    r1   ^= key[1  ];
    r2   ^= key[2  ];
    r3   ^= key[3  ];
    r4   ^= key[4  ];
    r5   ^= key[5  ];
    r6   ^= key[6  ];
    r7   ^= key[7  ];
    r8   ^= key[8  ];
    r9   ^= key[9  ];
    r10  ^= key[10 ];
    r11  ^= key[11 ];
    r12  ^= key[12 ];
    r13  ^= key[13 ];
    r14  ^= key[14 ];
    r15  ^= key[15 ];
    r16  ^= key[16 ];
    r17  ^= key[17 ];
    r18  ^= key[18 ];
    r19  ^= key[19 ];
    r20  ^= key[20 ];
    r21  ^= key[21 ];
    r22  ^= key[22 ];
    r23  ^= key[23 ];
    r24  ^= key[24 ];
    r25  ^= key[25 ];
    r26  ^= key[26 ];
    r27  ^= key[27 ];
    r28  ^= key[28 ];
    r29  ^= key[29 ];
    r30  ^= key[30 ];
    r31  ^= key[31 ];
    r32  ^= key[32 ];
    r33  ^= key[33 ];
    r34  ^= key[34 ];
    r35  ^= key[35 ];
    r36  ^= key[36 ];
    r37  ^= key[37 ];
    r38  ^= key[38 ];
    r39  ^= key[39 ];
    r40  ^= key[40 ];
    r41  ^= key[41 ];
    r42  ^= key[42 ];
    r43  ^= key[43 ];
    r44  ^= key[44 ];
    r45  ^= key[45 ];
    r46  ^= key[46 ];
    r47  ^= key[47 ];
    r48  ^= key[48 ];
    r49  ^= key[49 ];
    r50  ^= key[50 ];
    r51  ^= key[51 ];
    r52  ^= key[52 ];
    r53  ^= key[53 ];
    r54  ^= key[54 ];
    r55  ^= key[55 ];
    r56  ^= key[56 ];
    r57  ^= key[57 ];
    r58  ^= key[58 ];
    r59  ^= key[59 ];
    r60  ^= key[60 ];
    r61  ^= key[61 ];
    r62  ^= key[62 ];
    r63  ^= key[63 ];
    r64  ^= key[64 ];
    r65  ^= key[65 ];
    r66  ^= key[66 ];
    r67  ^= key[67 ];
    r68  ^= key[68 ];
    r69  ^= key[69 ];
    r70  ^= key[70 ];
    r71  ^= key[71 ];
    r72  ^= key[72 ];
    r73  ^= key[73 ];
    r74  ^= key[74 ];
    r75  ^= key[75 ];
    r76  ^= key[76 ];
    r77  ^= key[77 ];
    r78  ^= key[78 ];
    r79  ^= key[79 ];
    r80  ^= key[80 ];
    r81  ^= key[81 ];
    r82  ^= key[82 ];
    r83  ^= key[83 ];
    r84  ^= key[84 ];
    r85  ^= key[85 ];
    r86  ^= key[86 ];
    r87  ^= key[87 ];
    r88  ^= key[88 ];
    r89  ^= key[89 ];
    r90  ^= key[90 ];
    r91  ^= key[91 ];
    r92  ^= key[92 ];
    r93  ^= key[93 ];
    r94  ^= key[94 ];
    r95  ^= key[95 ];
    r96  ^= key[96 ];
    r97  ^= key[97 ];
    r98  ^= key[98 ];
    r99  ^= key[99 ];
    r100 ^= key[100];
    r101 ^= key[101];
    r102 ^= key[102];
    r103 ^= key[103];
    r104 ^= key[104];
    r105 ^= key[105];
    r106 ^= key[106];
    r107 ^= key[107];
    r108 ^= key[108];
    r109 ^= key[109];
    r110 ^= key[110];
    r111 ^= key[111];
    r112 ^= key[112];
    r113 ^= key[113];
    r114 ^= key[114];
    r115 ^= key[115];
    r116 ^= key[116];
    r117 ^= key[117];
    r118 ^= key[118];
    r119 ^= key[119];
    r120 ^= key[120];
    r121 ^= key[121];
    r122 ^= key[122];
    r123 ^= key[123];
    r124 ^= key[124];
    r125 ^= key[125];
    r126 ^= key[126];
    r127 ^= key[127];
}


//** With regiter as array
__device__ void addRoundKey32(uint32_t cipher[32], uint32_t key[32]) {
    cipher[0]   ^= key[0  ];
    cipher[1]   ^= key[1  ];
    cipher[2]   ^= key[2  ];
    cipher[3]   ^= key[3  ];
    cipher[4]   ^= key[4  ];
    cipher[5]   ^= key[5  ];
    cipher[6]   ^= key[6  ];
    cipher[7]   ^= key[7  ];
    cipher[8]   ^= key[8  ];
    cipher[9]   ^= key[9  ];
    cipher[10]  ^= key[10 ];
    cipher[11]  ^= key[11 ];
    cipher[12]  ^= key[12 ];
    cipher[13]  ^= key[13 ];
    cipher[14]  ^= key[14 ];
    cipher[15]  ^= key[15 ];
    cipher[16]  ^= key[16 ];
    cipher[17]  ^= key[17 ];
    cipher[18]  ^= key[18 ];
    cipher[19]  ^= key[19 ];
    cipher[20]  ^= key[20 ];
    cipher[21]  ^= key[21 ];
    cipher[22]  ^= key[22 ];
    cipher[23]  ^= key[23 ];
    cipher[24]  ^= key[24 ];
    cipher[25]  ^= key[25 ];
    cipher[26]  ^= key[26 ];
    cipher[27]  ^= key[27 ];
    cipher[28]  ^= key[28 ];
    cipher[29]  ^= key[29 ];
    cipher[30]  ^= key[30 ];
    cipher[31]  ^= key[31 ];
}


//__device__    void addRoundKey(uint32_t shared[128], uint32_t sm_key[128]) {
__device__ void addRoundKey(uint32_t cipher[128], uint32_t key[128]) {
    cipher[0]   ^= key[0  ];
    cipher[1]   ^= key[1  ];
    cipher[2]   ^= key[2  ];
    cipher[3]   ^= key[3  ];
    cipher[4]   ^= key[4  ];
    cipher[5]   ^= key[5  ];
    cipher[6]   ^= key[6  ];
    cipher[7]   ^= key[7  ];
    cipher[8]   ^= key[8  ];
    cipher[9]   ^= key[9  ];
    cipher[10]  ^= key[10 ];
    cipher[11]  ^= key[11 ];
    cipher[12]  ^= key[12 ];
    cipher[13]  ^= key[13 ];
    cipher[14]  ^= key[14 ];
    cipher[15]  ^= key[15 ];
    cipher[16]  ^= key[16 ];
    cipher[17]  ^= key[17 ];
    cipher[18]  ^= key[18 ];
    cipher[19]  ^= key[19 ];
    cipher[20]  ^= key[20 ];
    cipher[21]  ^= key[21 ];
    cipher[22]  ^= key[22 ];
    cipher[23]  ^= key[23 ];
    cipher[24]  ^= key[24 ];
    cipher[25]  ^= key[25 ];
    cipher[26]  ^= key[26 ];
    cipher[27]  ^= key[27 ];
    cipher[28]  ^= key[28 ];
    cipher[29]  ^= key[29 ];
    cipher[30]  ^= key[30 ];
    cipher[31]  ^= key[31 ];
    cipher[32]  ^= key[32 ];
    cipher[33]  ^= key[33 ];
    cipher[34]  ^= key[34 ];
    cipher[35]  ^= key[35 ];
    cipher[36]  ^= key[36 ];
    cipher[37]  ^= key[37 ];
    cipher[38]  ^= key[38 ];
    cipher[39]  ^= key[39 ];
    cipher[40]  ^= key[40 ];
    cipher[41]  ^= key[41 ];
    cipher[42]  ^= key[42 ];
    cipher[43]  ^= key[43 ];
    cipher[44]  ^= key[44 ];
    cipher[45]  ^= key[45 ];
    cipher[46]  ^= key[46 ];
    cipher[47]  ^= key[47 ];
    cipher[48]  ^= key[48 ];
    cipher[49]  ^= key[49 ];
    cipher[50]  ^= key[50 ];
    cipher[51]  ^= key[51 ];
    cipher[52]  ^= key[52 ];
    cipher[53]  ^= key[53 ];
    cipher[54]  ^= key[54 ];
    cipher[55]  ^= key[55 ];
    cipher[56]  ^= key[56 ];
    cipher[57]  ^= key[57 ];
    cipher[58]  ^= key[58 ];
    cipher[59]  ^= key[59 ];
    cipher[60]  ^= key[60 ];
    cipher[61]  ^= key[61 ];
    cipher[62]  ^= key[62 ];
    cipher[63]  ^= key[63 ];
    cipher[64]  ^= key[64 ];
    cipher[65]  ^= key[65 ];
    cipher[66]  ^= key[66 ];
    cipher[67]  ^= key[67 ];
    cipher[68]  ^= key[68 ];
    cipher[69]  ^= key[69 ];
    cipher[70]  ^= key[70 ];
    cipher[71]  ^= key[71 ];
    cipher[72]  ^= key[72 ];
    cipher[73]  ^= key[73 ];
    cipher[74]  ^= key[74 ];
    cipher[75]  ^= key[75 ];
    cipher[76]  ^= key[76 ];
    cipher[77]  ^= key[77 ];
    cipher[78]  ^= key[78 ];
    cipher[79]  ^= key[79 ];
    cipher[80]  ^= key[80 ];
    cipher[81]  ^= key[81 ];
    cipher[82]  ^= key[82 ];
    cipher[83]  ^= key[83 ];
    cipher[84]  ^= key[84 ];
    cipher[85]  ^= key[85 ];
    cipher[86]  ^= key[86 ];
    cipher[87]  ^= key[87 ];
    cipher[88]  ^= key[88 ];
    cipher[89]  ^= key[89 ];
    cipher[90]  ^= key[90 ];
    cipher[91]  ^= key[91 ];
    cipher[92]  ^= key[92 ];
    cipher[93]  ^= key[93 ];
    cipher[94]  ^= key[94 ];
    cipher[95]  ^= key[95 ];
    cipher[96]  ^= key[96 ];
    cipher[97]  ^= key[97 ];
    cipher[98]  ^= key[98 ];
    cipher[99]  ^= key[99 ];
    cipher[100] ^= key[100];
    cipher[101] ^= key[101];
    cipher[102] ^= key[102];
    cipher[103] ^= key[103];
    cipher[104] ^= key[104];
    cipher[105] ^= key[105];
    cipher[106] ^= key[106];
    cipher[107] ^= key[107];
    cipher[108] ^= key[108];
    cipher[109] ^= key[109];
    cipher[110] ^= key[110];
    cipher[111] ^= key[111];
    cipher[112] ^= key[112];
    cipher[113] ^= key[113];
    cipher[114] ^= key[114];
    cipher[115] ^= key[115];
    cipher[116] ^= key[116];
    cipher[117] ^= key[117];
    cipher[118] ^= key[118];
    cipher[119] ^= key[119];
    cipher[120] ^= key[120];
    cipher[121] ^= key[121];
    cipher[122] ^= key[122];
    cipher[123] ^= key[123];
    cipher[124] ^= key[124];
    cipher[125] ^= key[125];
    cipher[126] ^= key[126];
    cipher[127] ^= key[127];
}


#endif //_ADD_ROUND_KEY_H_