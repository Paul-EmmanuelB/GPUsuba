#ifndef _ADD_ROUND_KEY_H_
#define _ADD_ROUND_KEY_H_

//__device__  inline  void addRoundKey(uint32_t shared[128], uint32_t sm_key[128]) {
__device__ void addRoundKey(uint32_t &cipher, uint32_t key) {
    cipher   ^= key;
}

__device__ inline void addRoundKey(uint32_t &r0 , uint32_t &r1 , uint32_t &r2 , uint32_t &r3 , uint32_t &r4 , uint32_t &r5 , uint32_t &r6 , uint32_t &r7 ,
                             uint32_t &r8 , uint32_t &r9 , uint32_t &r10, uint32_t &r11, uint32_t &r12, uint32_t &r13, uint32_t &r14, uint32_t &r15,
                             uint32_t &r16, uint32_t &r17, uint32_t &r18, uint32_t &r19, uint32_t &r20, uint32_t &r21, uint32_t &r22, uint32_t &r23,
                             uint32_t &r24, uint32_t &r25, uint32_t &r26, uint32_t &r27, uint32_t &r28, uint32_t &r29, uint32_t &r30, uint32_t &r31,
                             uint32_t key[32]){
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

__device__ inline void addRoundKey(uint32_t &r0 , uint32_t &r1 , uint32_t &r2 , uint32_t &r3 , uint32_t &r4 , uint32_t &r5 , uint32_t &r6 , uint32_t &r7 ,
                             uint32_t key[8]){
    r0   ^= key[0  ];
    r1   ^= key[1  ];
    r2   ^= key[2  ];
    r3   ^= key[3  ];
    r4   ^= key[4  ];
    r5   ^= key[5  ];
    r6   ^= key[6  ];
    r7   ^= key[7  ];
}

__device__  void addRoundKey(uint32_t &r0  , uint32_t &r1  , uint32_t &r2  , uint32_t &r3  , uint32_t &r4  , uint32_t &r5  , uint32_t &r6  , uint32_t &r7  ,
                 uint32_t &r8  , uint32_t &r9  , uint32_t &r10 , uint32_t &r11 , uint32_t &r12 , uint32_t &r13 , uint32_t &r14 , uint32_t &r15 ,
                 uint32_t &r16 , uint32_t &r17 , uint32_t &r18 , uint32_t &r19 , uint32_t &r20 , uint32_t &r21 , uint32_t &r22 , uint32_t &r23 ,
                 uint32_t &r24 , uint32_t &r25 , uint32_t &r26 , uint32_t &r27 , uint32_t &r28 , uint32_t &r29 , uint32_t &r30 , uint32_t &r31 ,
                 uint32_t &r32 , uint32_t &r33 , uint32_t &r34 , uint32_t &r35 , uint32_t &r36 , uint32_t &r37 , uint32_t &r38 , uint32_t &r39 ,
                 uint32_t &r40 , uint32_t &r41 , uint32_t &r42 , uint32_t &r43 , uint32_t &r44 , uint32_t &r45 , uint32_t &r46 , uint32_t &r47 ,
                 uint32_t &r48 , uint32_t &r49 , uint32_t &r50 , uint32_t &r51 , uint32_t &r52 , uint32_t &r53 , uint32_t &r54 , uint32_t &r55 ,
                 uint32_t &r56 , uint32_t &r57 , uint32_t &r58 , uint32_t &r59 , uint32_t &r60 , uint32_t &r61 , uint32_t &r62 , uint32_t &r63 ,
                 uint32_t &r64 , uint32_t &r65 , uint32_t &r66 , uint32_t &r67 , uint32_t &r68 , uint32_t &r69 , uint32_t &r70 , uint32_t &r71 ,
                 uint32_t &r72 , uint32_t &r73 , uint32_t &r74 , uint32_t &r75 , uint32_t &r76 , uint32_t &r77 , uint32_t &r78 , uint32_t &r79 ,
                 uint32_t &r80 , uint32_t &r81 , uint32_t &r82 , uint32_t &r83 , uint32_t &r84 , uint32_t &r85 , uint32_t &r86 , uint32_t &r87 ,
                 uint32_t &r88 , uint32_t &r89 , uint32_t &r90 , uint32_t &r91 , uint32_t &r92 , uint32_t &r93 , uint32_t &r94 , uint32_t &r95 ,
                 uint32_t &r96 , uint32_t &r97 , uint32_t &r98 , uint32_t &r99 , uint32_t &r100, uint32_t &r101, uint32_t &r102, uint32_t &r103,
                 uint32_t &r104, uint32_t &r105, uint32_t &r106, uint32_t &r107, uint32_t &r108, uint32_t &r109, uint32_t &r110, uint32_t &r111,
                 uint32_t &r112, uint32_t &r113, uint32_t &r114, uint32_t &r115, uint32_t &r116, uint32_t &r117, uint32_t &r118, uint32_t &r119,
                 uint32_t &r120, uint32_t &r121, uint32_t &r122, uint32_t &r123, uint32_t &r124, uint32_t &r125, uint32_t &r126, uint32_t &r127,
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


//** With arrays
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


//__device__  inline  void addRoundKey(uint32_t shared[128], uint32_t sm_key[128]) {
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