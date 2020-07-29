#ifndef _SHIFT_ROWS_H_
#define _SHIFT_ROWS_H_

// Register version
template<typename T>
__device__ __forceinline__ void shiftFirstRow(T &r0 , T &r1 , T &r2 , T &r3 , T &r4 , T &r5 , T &r6 , T &r7 ,
                              T &r8 , T &r9 , T &r10, T &r11, T &r12, T &r13, T &r14, T &r15,
                              T &r16, T &r17, T &r18, T &r19, T &r20, T &r21, T &r22, T &r23,
                              T &r24, T &r25, T &r26, T &r27, T &r28, T &r29, T &r30, T &r31){    
    T t0, t1, t2, t3, t4, t5, t6, t7;

    //First row
    t0 = r0;
    t1 = r1;
    t2 = r2;
    t3 = r3;
    t4 = r4;
    t5 = r5;
    t6 = r6;
    t7 = r7;

    r0 = r8;
    r1 = r9;
    r2 = r10;
    r3 = r11;
    r4 = r12;
    r5 = r13;
    r6 = r14;
    r7 = r15;

    r8  = r16;
    r9  = r17;
    r10 = r18;
    r11 = r19;
    r12 = r20;
    r13 = r21;
    r14 = r22;
    r15 = r23;

    r16 = r24;
    r17 = r25;
    r18 = r26;
    r19 = r27;
    r20 = r28;
    r21 = r29;
    r22 = r30;
    r23 = r31;

    r24 = t0;
    r25 = t1;
    r26 = t2;
    r27 = t3;
    r28 = t4;
    r29 = t5;
    r30 = t6;
    r31 = t7;
}

template<typename T>
__device__ __forceinline__ void shiftSecondRow(T &r0 , T &r1 , T &r2 , T &r3 , T &r4 , T &r5 , T &r6 , T &r7 ,
                               T &r8 , T &r9 , T &r10, T &r11, T &r12, T &r13, T &r14, T &r15,
                               T &r16, T &r17, T &r18, T &r19, T &r20, T &r21, T &r22, T &r23,
                               T &r24, T &r25, T &r26, T &r27, T &r28, T &r29, T &r30, T &r31){
    T t0, t1, t2, t3, t4, t5, t6, t7;
    //Second row
    t0 = r0;
    t1 = r1;
    t2 = r2;
    t3 = r3;
    t4 = r4;
    t5 = r5;
    t6 = r6;
    t7 = r7;

    r0 = r8;
    r1 = r9;
    r2 = r10;
    r3 = r11;
    r4 = r12;
    r5 = r13;
    r6 = r14;
    r7 = r15;

    r8 = t0;
    r9 = t1;
    r10 = t2;
    r11 = t3;
    r12 = t4;
    r13 = t5;
    r14 = t6;
    r15 = t7;

    t0 = r16;
    t1 = r17;
    t2 = r18;
    t3 = r19;
    t4 = r20;
    t5 = r21;
    t6 = r22;
    t7 = r23;

    r16 = r24;
    r17 = r25;
    r18 = r26;
    r19 = r27;
    r20 = r28;
    r21 = r29;
    r22 = r30;
    r23 = r31;

    r24 = t0;
    r25 = t1;
    r26 = t2;
    r27 = t3;
    r28 = t4;
    r29 = t5;
    r30 = t6;
    r31 = t7;
}

template<typename T>
__device__ __forceinline__ void shiftThirdRow(T &r0 , T &r1 , T &r2 , T &r3 , T &r4 , T &r5 , T &r6 , T &r7 ,
                              T &r8 , T &r9 , T &r10, T &r11, T &r12, T &r13, T &r14, T &r15,
                              T &r16, T &r17, T &r18, T &r19, T &r20, T &r21, T &r22, T &r23,
                              T &r24, T &r25, T &r26, T &r27, T &r28, T &r29, T &r30, T &r31){

    T t0, t1, t2, t3, t4, t5, t6, t7;
    //Last row
    t0 = r0;
    t1 = r1;
    t2 = r2;
    t3 = r3;
    t4 = r4;
    t5 = r5;
    t6 = r6;
    t7 = r7;

    r0 = r8;
    r1 = r9;
    r2 = r10;
    r3 = r11;
    r4 = r12;
    r5 = r13;
    r6 = r14;
    r7 = r15;

    r8  = r16;
    r9  = r17;
    r10 = r18;
    r11 = r19;
    r12 = r20;
    r13 = r21;
    r14 = r22;
    r15 = r23;

    r16 = r24;
    r17 = r25;
    r18 = r26;
    r19 = r27;
    r20 = r28;
    r21 = r29;
    r22 = r30;
    r23 = r31;

    r24 = t0;
    r25 = t1;
    r26 = t2;
    r27 = t3;
    r28 = t4;
    r29 = t5;
    r30 = t6;
    r31 = t7;
}


template<typename T>
__device__ __forceinline__ void shiftRows(T &r8  , T &r9  , T &r10 , T &r11 , T &r12 , T &r13 , T &r14 , T &r15 , T &r16 , T &r17 , T &r18 , T &r19 , T &r20 , T &r21 , T &r22 , T &r23 , T &r24 , T &r25 , T &r26 , T &r27 , T &r28 , T &r29 , T &r30 , T &r31 ,
                          T &r40 , T &r41 , T &r42 , T &r43 , T &r44 , T &r45 , T &r46 , T &r47 , T &r48 , T &r49 , T &r50 , T &r51 , T &r52 , T &r53 , T &r54 , T &r55 , T &r56 , T &r57 , T &r58 , T &r59 , T &r60 , T &r61 , T &r62 , T &r63 ,
                          T &r72 , T &r73 , T &r74 , T &r75 , T &r76 , T &r77 , T &r78 , T &r79 , T &r80 , T &r81 , T &r82 , T &r83 , T &r84 , T &r85 , T &r86 , T &r87 , T &r88 , T &r89 , T &r90 , T &r91 , T &r92 , T &r93 , T &r94 , T &r95 ,
                          T &r104, T &r105, T &r106, T &r107, T &r108, T &r109, T &r110, T &r111, T &r112, T &r113, T &r114, T &r115, T &r116, T &r117, T &r118, T &r119, T &r120, T &r121, T &r122, T &r123, T &r124, T &r125, T &r126, T &r127){
    
    T t0, t1, t2, t3, t4, t5, t6, t7;

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

// Array version
template<typename T>
__device__ void shiftRows(T cipher[128]){    
    T t0, t1, t2, t3, t4, t5, t6, t7;

    //First row
    t0 = cipher[8] ;
    t1 = cipher[9] ;
    t2 = cipher[10];
    t3 = cipher[11];
    t4 = cipher[12];
    t5 = cipher[13];
    t6 = cipher[14];
    t7 = cipher[15];

    cipher[8]  = cipher[40];
    cipher[9]  = cipher[41];
    cipher[10] = cipher[42];
    cipher[11] = cipher[43];
    cipher[12] = cipher[44];
    cipher[13] = cipher[45];
    cipher[14] = cipher[46];
    cipher[15] = cipher[47];

    cipher[40] = cipher[72];
    cipher[41] = cipher[73];
    cipher[42] = cipher[74];
    cipher[43] = cipher[75];
    cipher[44] = cipher[76];
    cipher[45] = cipher[77];
    cipher[46] = cipher[78];
    cipher[47] = cipher[79];

    cipher[72] = cipher[104];
    cipher[73] = cipher[105];
    cipher[74] = cipher[106];
    cipher[75] = cipher[107];
    cipher[76] = cipher[108];
    cipher[77] = cipher[109];
    cipher[78] = cipher[110];
    cipher[79] = cipher[111];

    cipher[104] = t0;
    cipher[105] = t1;
    cipher[106] = t2;
    cipher[107] = t3;
    cipher[108] = t4;
    cipher[109] = t5;
    cipher[110] = t6;
    cipher[111] = t7;

    //Second row
    t0 = cipher[16];
    t1 = cipher[17];
    t2 = cipher[18];
    t3 = cipher[19];
    t4 = cipher[20];
    t5 = cipher[21];
    t6 = cipher[22];
    t7 = cipher[23];

    cipher[16] = cipher[80];
    cipher[17] = cipher[81];
    cipher[18] = cipher[82];
    cipher[19] = cipher[83];
    cipher[20] = cipher[84];
    cipher[21] = cipher[85];
    cipher[22] = cipher[86];
    cipher[23] = cipher[87];

    cipher[80] = t0;
    cipher[81] = t1;
    cipher[82] = t2;
    cipher[83] = t3;
    cipher[84] = t4;
    cipher[85] = t5;
    cipher[86] = t6;
    cipher[87] = t7;

    t0 = cipher[48];
    t1 = cipher[49];
    t2 = cipher[50];
    t3 = cipher[51];
    t4 = cipher[52];
    t5 = cipher[53];
    t6 = cipher[54];
    t7 = cipher[55];

    cipher[48] = cipher[112];
    cipher[49] = cipher[113];
    cipher[50] = cipher[114];
    cipher[51] = cipher[115];
    cipher[52] = cipher[116];
    cipher[53] = cipher[117];
    cipher[54] = cipher[118];
    cipher[55] = cipher[119];

    cipher[112] = t0;
    cipher[113] = t1;
    cipher[114] = t2;
    cipher[115] = t3;
    cipher[116] = t4;
    cipher[117] = t5;
    cipher[118] = t6;
    cipher[119] = t7;

    //Last row
    t0 = cipher[24];
    t1 = cipher[25];
    t2 = cipher[26];
    t3 = cipher[27];
    t4 = cipher[28];
    t5 = cipher[29];
    t6 = cipher[30];
    t7 = cipher[31];

    cipher[24] = cipher[120];
    cipher[25] = cipher[121];
    cipher[26] = cipher[122];
    cipher[27] = cipher[123];
    cipher[28] = cipher[124];
    cipher[29] = cipher[125];
    cipher[30] = cipher[126];
    cipher[31] = cipher[127];

    cipher[120] = cipher[88];
    cipher[121] = cipher[89];
    cipher[122] = cipher[90];
    cipher[123] = cipher[91];
    cipher[124] = cipher[92];
    cipher[125] = cipher[93];
    cipher[126] = cipher[94];
    cipher[127] = cipher[95];

    cipher[88] = cipher[56];
    cipher[89] = cipher[57];
    cipher[90] = cipher[58];
    cipher[91] = cipher[59];
    cipher[92] = cipher[60];
    cipher[93] = cipher[61];
    cipher[94] = cipher[62];
    cipher[95] = cipher[63];

    cipher[56] = t0;
    cipher[57] = t1;
    cipher[58] = t2;
    cipher[59] = t3;
    cipher[60] = t4;
    cipher[61] = t5;
    cipher[62] = t6;
    cipher[63] = t7;
}


template<typename T>
__device__ void shiftFirstRow(T a8[8], T b40[8], T c72[8], T d104[8]){    
    T t0, t1, t2, t3, t4, t5, t6, t7;

    //First row
    t0 = a8[0];
    t1 = a8[1];
    t2 = a8[2];
    t3 = a8[3];
    t4 = a8[4];
    t5 = a8[5];
    t6 = a8[6];
    t7 = a8[7];

    a8[0] = b40[0];
    a8[1] = b40[1];
    a8[2] = b40[2];
    a8[3] = b40[3];
    a8[4] = b40[4];
    a8[5] = b40[5];
    a8[6] = b40[6];
    a8[7] = b40[7];

    b40[0] = c72[0];
    b40[1] = c72[1];
    b40[2] = c72[2];
    b40[3] = c72[3];
    b40[4] = c72[4];
    b40[5] = c72[5];
    b40[6] = c72[6];
    b40[7] = c72[7];

    c72[0] = d104[0];
    c72[1] = d104[1];
    c72[2] = d104[2];
    c72[3] = d104[3];
    c72[4] = d104[4];
    c72[5] = d104[5];
    c72[6] = d104[6];
    c72[7] = d104[7];

    d104[0] = t0;
    d104[1] = t1;
    d104[2] = t2;
    d104[3] = t3;
    d104[4] = t4;
    d104[5] = t5;
    d104[6] = t6;
    d104[7] = t7;
}

template<typename T>
__device__ void shiftSecondRow(T a16[8], T b80[8], T c48[8], T d112[8]){ 

    T t0, t1, t2, t3, t4, t5, t6, t7;
    //Second row
    t0 = a16[0];
    t1 = a16[1];
    t2 = a16[2];
    t3 = a16[3];
    t4 = a16[4];
    t5 = a16[5];
    t6 = a16[6];
    t7 = a16[7];

    a16[0] = b80[0];
    a16[1] = b80[1];
    a16[2] = b80[2];
    a16[3] = b80[3];
    a16[4] = b80[4];
    a16[5] = b80[5];
    a16[6] = b80[6];
    a16[7] = b80[7];

    b80[0] = t0;
    b80[1] = t1;
    b80[2] = t2;
    b80[3] = t3;
    b80[4] = t4;
    b80[5] = t5;
    b80[6] = t6;
    b80[7] = t7;

    t0 = c48[0];
    t1 = c48[1];
    t2 = c48[2];
    t3 = c48[3];
    t4 = c48[4];
    t5 = c48[5];
    t6 = c48[6];
    t7 = c48[7];

    c48[0] = d112[0];
    c48[1] = d112[1];
    c48[2] = d112[2];
    c48[3] = d112[3];
    c48[4] = d112[4];
    c48[5] = d112[5];
    c48[6] = d112[6];
    c48[7] = d112[7];

    d112[0] = t0;
    d112[1] = t1;
    d112[2] = t2;
    d112[3] = t3;
    d112[4] = t4;
    d112[5] = t5;
    d112[6] = t6;
    d112[7] = t7;
}

template<typename T>
__device__ void shiftThirdRow(T a24[8], T b120[8], T c88[8], T d56[8]){ 
    T t0, t1, t2, t3, t4, t5, t6, t7;
    //Last row
    t0 = a24[0];
    t1 = a24[1];
    t2 = a24[2];
    t3 = a24[3];
    t4 = a24[4];
    t5 = a24[5];
    t6 = a24[6];
    t7 = a24[7];

    a24[0] = b120[0];
    a24[1] = b120[1];
    a24[2] = b120[2];
    a24[3] = b120[3];
    a24[4] = b120[4];
    a24[5] = b120[5];
    a24[6] = b120[6];
    a24[7] = b120[7];

    b120[0] = c88[0];
    b120[1] = c88[1];
    b120[2] = c88[2];
    b120[3] = c88[3];
    b120[4] = c88[4];
    b120[5] = c88[5];
    b120[6] = c88[6];
    b120[7] = c88[7];

    c88[0] = d56[0];
    c88[1] = d56[1];
    c88[2] = d56[2];
    c88[3] = d56[3];
    c88[4] = d56[4];
    c88[5] = d56[5];
    c88[6] = d56[6];
    c88[7] = d56[7];

    d56[0] = t0;
    d56[1] = t1;
    d56[2] = t2;
    d56[3] = t3;
    d56[4] = t4;
    d56[5] = t5;
    d56[6] = t6;
    d56[7] = t7;
}

#endif //_SHIFT_ROWS_H_