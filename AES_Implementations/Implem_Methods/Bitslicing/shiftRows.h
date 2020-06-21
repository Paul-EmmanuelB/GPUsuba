#ifndef _SHIFT_ROWS_H_
#define _SHIFT_ROWS_H_

__device__ void shiftRows(uint32_t &r8  , uint32_t &r9  , uint32_t &r10 , uint32_t &r11 , uint32_t &r12 , uint32_t &r13 , uint32_t &r14 , uint32_t &r15 , uint32_t &r16 , uint32_t &r17 , uint32_t &r18 , uint32_t &r19 , uint32_t &r20 , uint32_t &r21 , uint32_t &r22 , uint32_t &r23 , uint32_t &r24 , uint32_t &r25 , uint32_t &r26 , uint32_t &r27 , uint32_t &r28 , uint32_t &r29 , uint32_t &r30 , uint32_t &r31 ,
                          uint32_t &r40 , uint32_t &r41 , uint32_t &r42 , uint32_t &r43 , uint32_t &r44 , uint32_t &r45 , uint32_t &r46 , uint32_t &r47 , uint32_t &r48 , uint32_t &r49 , uint32_t &r50 , uint32_t &r51 , uint32_t &r52 , uint32_t &r53 , uint32_t &r54 , uint32_t &r55 , uint32_t &r56 , uint32_t &r57 , uint32_t &r58 , uint32_t &r59 , uint32_t &r60 , uint32_t &r61 , uint32_t &r62 , uint32_t &r63 ,
                          uint32_t &r72 , uint32_t &r73 , uint32_t &r74 , uint32_t &r75 , uint32_t &r76 , uint32_t &r77 , uint32_t &r78 , uint32_t &r79 , uint32_t &r80 , uint32_t &r81 , uint32_t &r82 , uint32_t &r83 , uint32_t &r84 , uint32_t &r85 , uint32_t &r86 , uint32_t &r87 , uint32_t &r88 , uint32_t &r89 , uint32_t &r90 , uint32_t &r91 , uint32_t &r92 , uint32_t &r93 , uint32_t &r94 , uint32_t &r95 ,
                          uint32_t &r104, uint32_t &r105, uint32_t &r106, uint32_t &r107, uint32_t &r108, uint32_t &r109, uint32_t &r110, uint32_t &r111, uint32_t &r112, uint32_t &r113, uint32_t &r114, uint32_t &r115, uint32_t &r116, uint32_t &r117, uint32_t &r118, uint32_t &r119, uint32_t &r120, uint32_t &r121, uint32_t &r122, uint32_t &r123, uint32_t &r124, uint32_t &r125, uint32_t &r126, uint32_t &r127){
    
    uint32_t t0, t1, t2, t3, t4, t5, t6, t7;

    //First row
    t0 = r8 ;
    t1 = r9 ;
    t2 = r10;
    t3 = r11;
    t4 = r12;
    t5 = r13;
    t6 = r14;
    t7 = r15;

    r8  = r40;
    r9  = r41;
    r10 = r42;
    r11 = r43;
    r12 = r44;
    r13 = r45;
    r14 = r46;
    r15 = r47;

    r40 = r72;
    r41 = r73;
    r42 = r74;
    r43 = r75;
    r44 = r76;
    r45 = r77;
    r46 = r78;
    r47 = r79;

    r72 = r104;
    r73 = r105;
    r74 = r106;
    r75 = r107;
    r76 = r108;
    r77 = r109;
    r78 = r110;
    r79 = r111;

    r104 = t0;
    r105 = t1;
    r106 = t2;
    r107 = t3;
    r108 = t4;
    r109 = t5;
    r110 = t6;
    r111 = t7;

    //Second row
    t0 = r16;
    t1 = r17;
    t2 = r18;
    t3 = r19;
    t4 = r20;
    t5 = r21;
    t6 = r22;
    t7 = r23;

    r16 = r80;
    r17 = r81;
    r18 = r82;
    r19 = r83;
    r20 = r84;
    r21 = r85;
    r22 = r86;
    r23 = r87;

    r80 = t0;
    r81 = t1;
    r82 = t2;
    r83 = t3;
    r84 = t4;
    r85 = t5;
    r86 = t6;
    r87 = t7;

    t0 = r48;
    t1 = r49;
    t2 = r50;
    t3 = r51;
    t4 = r52;
    t5 = r53;
    t6 = r54;
    t7 = r55;

    r48 = r112;
    r49 = r113;
    r50 = r114;
    r51 = r115;
    r52 = r116;
    r53 = r117;
    r54 = r118;
    r55 = r119;

    r112 = t0;
    r113 = t1;
    r114 = t2;
    r115 = t3;
    r116 = t4;
    r117 = t5;
    r118 = t6;
    r119 = t7;

    //Last row
    t0 = r24;
    t1 = r25;
    t2 = r26;
    t3 = r27;
    t4 = r28;
    t5 = r29;
    t6 = r30;
    t7 = r31;

    r24 = r120;
    r25 = r121;
    r26 = r122;
    r27 = r123;
    r28 = r124;
    r29 = r125;
    r30 = r126;
    r31 = r127;

    r120 = r88;
    r121 = r89;
    r122 = r90;
    r123 = r91;
    r124 = r92;
    r125 = r93;
    r126 = r94;
    r127 = r95;

    r88 = r56;
    r89 = r57;
    r90 = r58;
    r91 = r59;
    r92 = r60;
    r93 = r61;
    r94 = r62;
    r95 = r63;

    r56 = t0;
    r57 = t1;
    r58 = t2;
    r59 = t3;
    r60 = t4;
    r61 = t5;
    r62 = t6;
    r63 = t7;
}


__device__ inline void shiftRows(uint32_t shared[128]){
    
    uint32_t t0, t1, t2, t3, t4, t5, t6, t7;

    //First row
    t0 = shared[8] ;
    t1 = shared[9] ;
    t2 = shared[10];
    t3 = shared[11];
    t4 = shared[12];
    t5 = shared[13];
    t6 = shared[14];
    t7 = shared[15];

    shared[8]  = shared[40];
    shared[9]  = shared[41];
    shared[10] = shared[42];
    shared[11] = shared[43];
    shared[12] = shared[44];
    shared[13] = shared[45];
    shared[14] = shared[46];
    shared[15] = shared[47];

    shared[40] = shared[72];
    shared[41] = shared[73];
    shared[42] = shared[74];
    shared[43] = shared[75];
    shared[44] = shared[76];
    shared[45] = shared[77];
    shared[46] = shared[78];
    shared[47] = shared[79];

    shared[72] = shared[104];
    shared[73] = shared[105];
    shared[74] = shared[106];
    shared[75] = shared[107];
    shared[76] = shared[108];
    shared[77] = shared[109];
    shared[78] = shared[110];
    shared[79] = shared[111];

    shared[104] = t0;
    shared[105] = t1;
    shared[106] = t2;
    shared[107] = t3;
    shared[108] = t4;
    shared[109] = t5;
    shared[110] = t6;
    shared[111] = t7;

    //Second row
    t0 = shared[16];
    t1 = shared[17];
    t2 = shared[18];
    t3 = shared[19];
    t4 = shared[20];
    t5 = shared[21];
    t6 = shared[22];
    t7 = shared[23];

    shared[16] = shared[80];
    shared[17] = shared[81];
    shared[18] = shared[82];
    shared[19] = shared[83];
    shared[20] = shared[84];
    shared[21] = shared[85];
    shared[22] = shared[86];
    shared[23] = shared[87];

    shared[80] = t0;
    shared[81] = t1;
    shared[82] = t2;
    shared[83] = t3;
    shared[84] = t4;
    shared[85] = t5;
    shared[86] = t6;
    shared[87] = t7;

    t0 = shared[48];
    t1 = shared[49];
    t2 = shared[50];
    t3 = shared[51];
    t4 = shared[52];
    t5 = shared[53];
    t6 = shared[54];
    t7 = shared[55];

    shared[48] = shared[112];
    shared[49] = shared[113];
    shared[50] = shared[114];
    shared[51] = shared[115];
    shared[52] = shared[116];
    shared[53] = shared[117];
    shared[54] = shared[118];
    shared[55] = shared[119];

    shared[112] = t0;
    shared[113] = t1;
    shared[114] = t2;
    shared[115] = t3;
    shared[116] = t4;
    shared[117] = t5;
    shared[118] = t6;
    shared[119] = t7;

    //Last row
    t0 = shared[24];
    t1 = shared[25];
    t2 = shared[26];
    t3 = shared[27];
    t4 = shared[28];
    t5 = shared[29];
    t6 = shared[30];
    t7 = shared[31];

    shared[24] = shared[120];
    shared[25] = shared[121];
    shared[26] = shared[122];
    shared[27] = shared[123];
    shared[28] = shared[124];
    shared[29] = shared[125];
    shared[30] = shared[126];
    shared[31] = shared[127];

    shared[120] = shared[88];
    shared[121] = shared[89];
    shared[122] = shared[90];
    shared[123] = shared[91];
    shared[124] = shared[92];
    shared[125] = shared[93];
    shared[126] = shared[94];
    shared[127] = shared[95];

    shared[88] = shared[56];
    shared[89] = shared[57];
    shared[90] = shared[58];
    shared[91] = shared[59];
    shared[92] = shared[60];
    shared[93] = shared[61];
    shared[94] = shared[62];
    shared[95] = shared[63];

    shared[56] = t0;
    shared[57] = t1;
    shared[58] = t2;
    shared[59] = t3;
    shared[60] = t4;
    shared[61] = t5;
    shared[62] = t6;
    shared[63] = t7;
}

#endif //_SHIFT_ROWS_H_