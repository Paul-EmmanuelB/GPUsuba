#ifndef _ADD_ROUND_KEY_H_
#define _ADD_ROUND_KEY_H_

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
                 uint32_t *sm_key) {
    r0   ^= sm_key[0  ];
    r1   ^= sm_key[1  ];
    r2   ^= sm_key[2  ];
    r3   ^= sm_key[3  ];
    r4   ^= sm_key[4  ];
    r5   ^= sm_key[5  ];
    r6   ^= sm_key[6  ];
    r7   ^= sm_key[7  ];
    r8   ^= sm_key[8  ];
    r9   ^= sm_key[9  ];
    r10  ^= sm_key[10 ];
    r11  ^= sm_key[11 ];
    r12  ^= sm_key[12 ];
    r13  ^= sm_key[13 ];
    r14  ^= sm_key[14 ];
    r15  ^= sm_key[15 ];
    r16  ^= sm_key[16 ];
    r17  ^= sm_key[17 ];
    r18  ^= sm_key[18 ];
    r19  ^= sm_key[19 ];
    r20  ^= sm_key[20 ];
    r21  ^= sm_key[21 ];
    r22  ^= sm_key[22 ];
    r23  ^= sm_key[23 ];
    r24  ^= sm_key[24 ];
    r25  ^= sm_key[25 ];
    r26  ^= sm_key[26 ];
    r27  ^= sm_key[27 ];
    r28  ^= sm_key[28 ];
    r29  ^= sm_key[29 ];
    r30  ^= sm_key[30 ];
    r31  ^= sm_key[31 ];
    r32  ^= sm_key[32 ];
    r33  ^= sm_key[33 ];
    r34  ^= sm_key[34 ];
    r35  ^= sm_key[35 ];
    r36  ^= sm_key[36 ];
    r37  ^= sm_key[37 ];
    r38  ^= sm_key[38 ];
    r39  ^= sm_key[39 ];
    r40  ^= sm_key[40 ];
    r41  ^= sm_key[41 ];
    r42  ^= sm_key[42 ];
    r43  ^= sm_key[43 ];
    r44  ^= sm_key[44 ];
    r45  ^= sm_key[45 ];
    r46  ^= sm_key[46 ];
    r47  ^= sm_key[47 ];
    r48  ^= sm_key[48 ];
    r49  ^= sm_key[49 ];
    r50  ^= sm_key[50 ];
    r51  ^= sm_key[51 ];
    r52  ^= sm_key[52 ];
    r53  ^= sm_key[53 ];
    r54  ^= sm_key[54 ];
    r55  ^= sm_key[55 ];
    r56  ^= sm_key[56 ];
    r57  ^= sm_key[57 ];
    r58  ^= sm_key[58 ];
    r59  ^= sm_key[59 ];
    r60  ^= sm_key[60 ];
    r61  ^= sm_key[61 ];
    r62  ^= sm_key[62 ];
    r63  ^= sm_key[63 ];
    r64  ^= sm_key[64 ];
    r65  ^= sm_key[65 ];
    r66  ^= sm_key[66 ];
    r67  ^= sm_key[67 ];
    r68  ^= sm_key[68 ];
    r69  ^= sm_key[69 ];
    r70  ^= sm_key[70 ];
    r71  ^= sm_key[71 ];
    r72  ^= sm_key[72 ];
    r73  ^= sm_key[73 ];
    r74  ^= sm_key[74 ];
    r75  ^= sm_key[75 ];
    r76  ^= sm_key[76 ];
    r77  ^= sm_key[77 ];
    r78  ^= sm_key[78 ];
    r79  ^= sm_key[79 ];
    r80  ^= sm_key[80 ];
    r81  ^= sm_key[81 ];
    r82  ^= sm_key[82 ];
    r83  ^= sm_key[83 ];
    r84  ^= sm_key[84 ];
    r85  ^= sm_key[85 ];
    r86  ^= sm_key[86 ];
    r87  ^= sm_key[87 ];
    r88  ^= sm_key[88 ];
    r89  ^= sm_key[89 ];
    r90  ^= sm_key[90 ];
    r91  ^= sm_key[91 ];
    r92  ^= sm_key[92 ];
    r93  ^= sm_key[93 ];
    r94  ^= sm_key[94 ];
    r95  ^= sm_key[95 ];
    r96  ^= sm_key[96 ];
    r97  ^= sm_key[97 ];
    r98  ^= sm_key[98 ];
    r99  ^= sm_key[99 ];
    r100 ^= sm_key[100];
    r101 ^= sm_key[101];
    r102 ^= sm_key[102];
    r103 ^= sm_key[103];
    r104 ^= sm_key[104];
    r105 ^= sm_key[105];
    r106 ^= sm_key[106];
    r107 ^= sm_key[107];
    r108 ^= sm_key[108];
    r109 ^= sm_key[109];
    r110 ^= sm_key[110];
    r111 ^= sm_key[111];
    r112 ^= sm_key[112];
    r113 ^= sm_key[113];
    r114 ^= sm_key[114];
    r115 ^= sm_key[115];
    r116 ^= sm_key[116];
    r117 ^= sm_key[117];
    r118 ^= sm_key[118];
    r119 ^= sm_key[119];
    r120 ^= sm_key[120];
    r121 ^= sm_key[121];
    r122 ^= sm_key[122];
    r123 ^= sm_key[123];
    r124 ^= sm_key[124];
    r125 ^= sm_key[125];
    r126 ^= sm_key[126];
    r127 ^= sm_key[127];
}


//__device__  inline  void addRoundKey(uint32_t shared[128], uint32_t sm_key[128]) {
__device__ void addRoundKey(uint32_t cipher[128], uint32_t sm_key[128]) {
    cipher[0]   ^= sm_key[0  ];
    cipher[1]   ^= sm_key[1  ];
    cipher[2]   ^= sm_key[2  ];
    cipher[3]   ^= sm_key[3  ];
    cipher[4]   ^= sm_key[4  ];
    cipher[5]   ^= sm_key[5  ];
    cipher[6]   ^= sm_key[6  ];
    cipher[7]   ^= sm_key[7  ];
    cipher[8]   ^= sm_key[8  ];
    cipher[9]   ^= sm_key[9  ];
    cipher[10]  ^= sm_key[10 ];
    cipher[11]  ^= sm_key[11 ];
    cipher[12]  ^= sm_key[12 ];
    cipher[13]  ^= sm_key[13 ];
    cipher[14]  ^= sm_key[14 ];
    cipher[15]  ^= sm_key[15 ];
    cipher[16]  ^= sm_key[16 ];
    cipher[17]  ^= sm_key[17 ];
    cipher[18]  ^= sm_key[18 ];
    cipher[19]  ^= sm_key[19 ];
    cipher[20]  ^= sm_key[20 ];
    cipher[21]  ^= sm_key[21 ];
    cipher[22]  ^= sm_key[22 ];
    cipher[23]  ^= sm_key[23 ];
    cipher[24]  ^= sm_key[24 ];
    cipher[25]  ^= sm_key[25 ];
    cipher[26]  ^= sm_key[26 ];
    cipher[27]  ^= sm_key[27 ];
    cipher[28]  ^= sm_key[28 ];
    cipher[29]  ^= sm_key[29 ];
    cipher[30]  ^= sm_key[30 ];
    cipher[31]  ^= sm_key[31 ];
    cipher[32]  ^= sm_key[32 ];
    cipher[33]  ^= sm_key[33 ];
    cipher[34]  ^= sm_key[34 ];
    cipher[35]  ^= sm_key[35 ];
    cipher[36]  ^= sm_key[36 ];
    cipher[37]  ^= sm_key[37 ];
    cipher[38]  ^= sm_key[38 ];
    cipher[39]  ^= sm_key[39 ];
    cipher[40]  ^= sm_key[40 ];
    cipher[41]  ^= sm_key[41 ];
    cipher[42]  ^= sm_key[42 ];
    cipher[43]  ^= sm_key[43 ];
    cipher[44]  ^= sm_key[44 ];
    cipher[45]  ^= sm_key[45 ];
    cipher[46]  ^= sm_key[46 ];
    cipher[47]  ^= sm_key[47 ];
    cipher[48]  ^= sm_key[48 ];
    cipher[49]  ^= sm_key[49 ];
    cipher[50]  ^= sm_key[50 ];
    cipher[51]  ^= sm_key[51 ];
    cipher[52]  ^= sm_key[52 ];
    cipher[53]  ^= sm_key[53 ];
    cipher[54]  ^= sm_key[54 ];
    cipher[55]  ^= sm_key[55 ];
    cipher[56]  ^= sm_key[56 ];
    cipher[57]  ^= sm_key[57 ];
    cipher[58]  ^= sm_key[58 ];
    cipher[59]  ^= sm_key[59 ];
    cipher[60]  ^= sm_key[60 ];
    cipher[61]  ^= sm_key[61 ];
    cipher[62]  ^= sm_key[62 ];
    cipher[63]  ^= sm_key[63 ];
    cipher[64]  ^= sm_key[64 ];
    cipher[65]  ^= sm_key[65 ];
    cipher[66]  ^= sm_key[66 ];
    cipher[67]  ^= sm_key[67 ];
    cipher[68]  ^= sm_key[68 ];
    cipher[69]  ^= sm_key[69 ];
    cipher[70]  ^= sm_key[70 ];
    cipher[71]  ^= sm_key[71 ];
    cipher[72]  ^= sm_key[72 ];
    cipher[73]  ^= sm_key[73 ];
    cipher[74]  ^= sm_key[74 ];
    cipher[75]  ^= sm_key[75 ];
    cipher[76]  ^= sm_key[76 ];
    cipher[77]  ^= sm_key[77 ];
    cipher[78]  ^= sm_key[78 ];
    cipher[79]  ^= sm_key[79 ];
    cipher[80]  ^= sm_key[80 ];
    cipher[81]  ^= sm_key[81 ];
    cipher[82]  ^= sm_key[82 ];
    cipher[83]  ^= sm_key[83 ];
    cipher[84]  ^= sm_key[84 ];
    cipher[85]  ^= sm_key[85 ];
    cipher[86]  ^= sm_key[86 ];
    cipher[87]  ^= sm_key[87 ];
    cipher[88]  ^= sm_key[88 ];
    cipher[89]  ^= sm_key[89 ];
    cipher[90]  ^= sm_key[90 ];
    cipher[91]  ^= sm_key[91 ];
    cipher[92]  ^= sm_key[92 ];
    cipher[93]  ^= sm_key[93 ];
    cipher[94]  ^= sm_key[94 ];
    cipher[95]  ^= sm_key[95 ];
    cipher[96]  ^= sm_key[96 ];
    cipher[97]  ^= sm_key[97 ];
    cipher[98]  ^= sm_key[98 ];
    cipher[99]  ^= sm_key[99 ];
    cipher[100] ^= sm_key[100];
    cipher[101] ^= sm_key[101];
    cipher[102] ^= sm_key[102];
    cipher[103] ^= sm_key[103];
    cipher[104] ^= sm_key[104];
    cipher[105] ^= sm_key[105];
    cipher[106] ^= sm_key[106];
    cipher[107] ^= sm_key[107];
    cipher[108] ^= sm_key[108];
    cipher[109] ^= sm_key[109];
    cipher[110] ^= sm_key[110];
    cipher[111] ^= sm_key[111];
    cipher[112] ^= sm_key[112];
    cipher[113] ^= sm_key[113];
    cipher[114] ^= sm_key[114];
    cipher[115] ^= sm_key[115];
    cipher[116] ^= sm_key[116];
    cipher[117] ^= sm_key[117];
    cipher[118] ^= sm_key[118];
    cipher[119] ^= sm_key[119];
    cipher[120] ^= sm_key[120];
    cipher[121] ^= sm_key[121];
    cipher[122] ^= sm_key[122];
    cipher[123] ^= sm_key[123];
    cipher[124] ^= sm_key[124];
    cipher[125] ^= sm_key[125];
    cipher[126] ^= sm_key[126];
    cipher[127] ^= sm_key[127];
}

__device__ void addRoundKey32(uint32_t cipher[32], uint32_t sm_key[32]) {
    cipher[0]   ^= sm_key[0  ];
    cipher[1]   ^= sm_key[1  ];
    cipher[2]   ^= sm_key[2  ];
    cipher[3]   ^= sm_key[3  ];
    cipher[4]   ^= sm_key[4  ];
    cipher[5]   ^= sm_key[5  ];
    cipher[6]   ^= sm_key[6  ];
    cipher[7]   ^= sm_key[7  ];
    cipher[8]   ^= sm_key[8  ];
    cipher[9]   ^= sm_key[9  ];
    cipher[10]  ^= sm_key[10 ];
    cipher[11]  ^= sm_key[11 ];
    cipher[12]  ^= sm_key[12 ];
    cipher[13]  ^= sm_key[13 ];
    cipher[14]  ^= sm_key[14 ];
    cipher[15]  ^= sm_key[15 ];
    cipher[16]  ^= sm_key[16 ];
    cipher[17]  ^= sm_key[17 ];
    cipher[18]  ^= sm_key[18 ];
    cipher[19]  ^= sm_key[19 ];
    cipher[20]  ^= sm_key[20 ];
    cipher[21]  ^= sm_key[21 ];
    cipher[22]  ^= sm_key[22 ];
    cipher[23]  ^= sm_key[23 ];
    cipher[24]  ^= sm_key[24 ];
    cipher[25]  ^= sm_key[25 ];
    cipher[26]  ^= sm_key[26 ];
    cipher[27]  ^= sm_key[27 ];
    cipher[28]  ^= sm_key[28 ];
    cipher[29]  ^= sm_key[29 ];
    cipher[30]  ^= sm_key[30 ];
    cipher[31]  ^= sm_key[31 ];
}

//__device__  inline  void addRoundKey(uint32_t shared[128], uint32_t sm_key[128]) {
__device__ void addRoundKey(uint32_t &cipher, uint32_t key) {
    cipher   ^= key;
}


#endif //_ADD_ROUND_KEY_H_