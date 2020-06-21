#ifndef _MIXCOLUMNS_H_
#define _MIXCOLUMNS_H_

__device__ void Mixcl( 
uint32_t &r0 , uint32_t &r1 , uint32_t &r2 , uint32_t &r3 , uint32_t &r4 , uint32_t &r5 , uint32_t &r6 , uint32_t &r7,
uint32_t &r8 , uint32_t &r9 , uint32_t &r10, uint32_t &r11, uint32_t &r12, uint32_t &r13, uint32_t &r14, uint32_t &r15,
uint32_t &r16, uint32_t &r17, uint32_t &r18, uint32_t &r19, uint32_t &r20, uint32_t &r21, uint32_t &r22, uint32_t &r23,
uint32_t &r24, uint32_t &r25, uint32_t &r26, uint32_t &r27, uint32_t &r28, uint32_t &r29, uint32_t &r30, uint32_t &r31){
    
    uint32_t o0  = 0 , o1  = 0 , o2  = 0, o3  = 0, o4  = 0, o5  = 0, o6  = 0, o7  = 0;
    uint32_t o8  = 0 , o9  = 0 , o10 = 0, o11 = 0, o12 = 0, o13 = 0, o14 = 0, o15 = 0;
    uint32_t o16 = 0 , o17 = 0 , o18 = 0, o19 = 0, o20 = 0, o21 = 0, o22 = 0, o23 = 0;
    uint32_t o24 = 0 , o25 = 0 , o26 = 0, o27 = 0, o28 = 0, o29 = 0, o30 = 0, o31 = 0;


    uint32_t a0,a1,a2,a3,a4,a5,a6,a7;//Mul2
    uint32_t tmp;
    /*
    Mix Columns in GF(2^8) with Primitive Polynomial g(x)=x^8+x^4+x^3+x+1
    Bitsliced Implementation 32-bit
    Function is implemented for a column of states [32 Regs * 32 bits]
    S_i Represents 8 Registers(each register include 32 bits)

    [02 03 01 01] [ S0 ]
    [01 02 03 01] * [ S4 ]
    [01 01 02 03] [ S8 ]
    [03 01 01 02] [ S12]

    */
    //----- First ROW -------//
    // Multiply by [02 03 01 01] in GF(2^8)
    // uint32_t Mul 2=state[0]^state[4];

    a0=r0^r8;
    a1=r1^r9;
    a2=r2^r10;
    a3=r3^r11;
    a4=r4^r12;
    a5=r5^r13;
    a6=r6^r14;
    a7=r7^r15;
    tmp=a7;
    a7=(a6);
    a6=(a5);
    a5=(a4);
    a4=(a3)^tmp;
    a3=(a2)^tmp;
    a2=(a1);
    a1=(a0)^tmp;
    a0=tmp;

    // o1=a^state[4]^state[8]^state[12];
    //Output of First Columns

    o0=a0^r8^r16^r24;
    o1=a1^r9^r17^r25;
    o2=a2^r10^r18^r26;
    o3=a3^r11^r19^r27;
    o4=a4^r12^r20^r28;
    o5=a5^r13^r21^r29;
    o6=a6^r14^r22^r30;
    o7=a7^r15^r23^r31;

    //----- Second ROW -------//
    //Multiply by [01 02 03 01] in GF(2^8)

    // uint32_t Mul2 = state[4]^state[8];
    a0=r8^r16;
    a1=r9^r17;
    a2=r10^r18;
    a3=r11^r19;
    a4=r12^r20;
    a5=r13^r21;
    a6=r14^r22;
    a7=r15^r23;
    tmp=a7;
    a7=(a6);
    a6=(a5);
    a5=(a4);
    a4=(a3)^tmp;
    a3=(a2)^tmp;
    a2=(a1);
    a1=(a0)^tmp;
    a0=tmp;

    // o1=a^state[0]^state[8]^state[12];
    //Output of Second Columns

    o8=a0^r0^r16^r24;
    o9=a1^r1^r17^r25;
    o10=a2^r2^r18^r26;
    o11=a3^r3^r19^r27;
    o12=a4^r4^r20^r28;
    o13=a5^r5^r21^r29;
    o14=a6^r6^r22^r30;
    o15=a7^r7^r23^r31;

    //----- Third ROW -------//
    //Multiply by [01 01 02 03] in GF(2^8)

    // uint32_t Mul2 = state[8]^state[12];

    a0=r16^r24;
    a1=r17^r25;
    a2=r18^r26;
    a3=r19^r27;
    a4=r20^r28;
    a5=r21^r29;
    a6=r22^r30;
    a7=r23^r31;
    tmp=a7;
    a7=(a6);
    a6=(a5);
    a5=(a4);
    a4=(a3)^tmp;
    a3=(a2)^tmp;
    a2=(a1);
    a1=(a0)^tmp;
    a0=tmp;

    // o1=a^state[0]^state[8]^state[12];
    // Output of the Third Column

    o16=a0^r8^r0^r24;
    o17=a1^r9^r1^r25;
    o18=a2^r10^r2^r26;
    o19=a3^r11^r3^r27;
    o20=a4^r12^r4^r28;
    o21=a5^r13^r5^r29;
    o22=a6^r14^r6^r30;
    o23=a7^r15^r7^r31;

    //----- Fourth ROW -------//
    //Multiply by [03 01 01 02] in GF(2^8)

    // uint32_t Mul2 = state[0]^state[12];
    a0=r0^r24;
    a1=r1^r25;
    a2=r2^r26;
    a3=r3^r27;
    a4=r4^r28;
    a5=r5^r29;
    a6=r6^r30;
    a7=r7^r31;
    tmp=a7;
    a7=(a6);
    a6=(a5);
    a5=(a4);
    a4=(a3)^tmp;
    a3=(a2)^tmp;
    a2=(a1);
    a1=(a0)^tmp;
    a0=tmp;
    // o1=a^state[4]^state[8]^state[0];
    // Output of the Last Column


    o24=a0^r8^r0^r16;
    o25=a1^r9^r1^r17;
    o26=a2^r10^r2^r18;
    o27=a3^r11^r3^r19;
    o28=a4^r12^r4^r20;
    o29=a5^r13^r5^r21;
    o30=a6^r14^r6^r22;
    o31=a7^r15^r7^r23;

    //Final Assignment

    r0=o0;
    r1=o1;
    r2=o2;
    r3=o3;
    r4=o4;
    r5=o5;
    r6=o6;
    r7=o7;

    r8=o8;
    r9=o9;
    r10=o10;
    r11=o11;
    r12=o12;
    r13=o13;
    r14=o14;
    r15=o15;

    r16=o16;
    r17=o17;
    r18=o18;
    r19=o19;
    r20=o20;
    r21=o21;
    r22=o22;
    r23=o23;

    r24=o24;
    r25=o25;
    r26=o26;
    r27=o27;
    r28=o28;
    r29=o29;
    r30=o30;
    r31=o31;

 }


 __device__ inline void Mixcl(uint32_t shared[]){
    
    uint32_t o0  = 0 , o1  = 0 , o2  = 0, o3  = 0, o4  = 0, o5  = 0, o6  = 0, o7  = 0;
    uint32_t o8  = 0 , o9  = 0 , o10 = 0, o11 = 0, o12 = 0, o13 = 0, o14 = 0, o15 = 0;
    uint32_t o16 = 0 , o17 = 0 , o18 = 0, o19 = 0, o20 = 0, o21 = 0, o22 = 0, o23 = 0;
    uint32_t o24 = 0 , o25 = 0 , o26 = 0, o27 = 0, o28 = 0, o29 = 0, o30 = 0, o31 = 0;


    uint32_t a0,a1,a2,a3,a4,a5,a6,a7;//Mul2
    uint32_t tmp;
    /*
    Mix Columns in GF(2^8) with Primitive Polynomial g(x)=x^8+x^4+x^3+x+1
    Bitsliced Implementation 32-bit
    Function is implemented for a column of states [32 Regs * 32 bits]
    S_i Represents 8 Registers(each register include 32 bits)

    [02 03 01 01] [ S0 ]
    [01 02 03 01] * [ S4 ]
    [01 01 02 03] [ S8 ]
    [03 01 01 02] [ S12]

    */
    //----- First ROW -------//
    // Multiply by [02 03 01 01] in GF(2^8)
    // uint32_t Mul 2=state[0]^state[4];

    a0=shared[0]^shared[8];
    a1=shared[1]^shared[9];
    a2=shared[2]^shared[10];
    a3=shared[3]^shared[11];
    a4=shared[4]^shared[12];
    a5=shared[5]^shared[13];
    a6=shared[6]^shared[14];
    a7=shared[7]^shared[15];
    tmp=a7;
    a7=(a6);
    a6=(a5);
    a5=(a4);
    a4=(a3)^tmp;
    a3=(a2)^tmp;
    a2=(a1);
    a1=(a0)^tmp;
    a0=tmp;

    // o1=a^state[4]^state[8]^state[12];
    //Output of First Columns

    o0=a0^shared[ 8]^shared[16]^shared[24];
    o1=a1^shared[ 9]^shared[17]^shared[25];
    o2=a2^shared[10]^shared[18]^shared[26];
    o3=a3^shared[11]^shared[19]^shared[27];
    o4=a4^shared[12]^shared[20]^shared[28];
    o5=a5^shared[13]^shared[21]^shared[29];
    o6=a6^shared[14]^shared[22]^shared[30];
    o7=a7^shared[15]^shared[23]^shared[31];

    //----- Second ROW -------//
    //Multiply by [01 02 03 01] in GF(2^8)

    // uint32_t Mul2 = state[4]^state[8];
    a0=shared[ 8]^shared[16];
    a1=shared[ 9]^shared[17];
    a2=shared[10]^shared[18];
    a3=shared[11]^shared[19];
    a4=shared[12]^shared[20];
    a5=shared[13]^shared[21];
    a6=shared[14]^shared[22];
    a7=shared[15]^shared[23];
    tmp=a7;
    a7=(a6);
    a6=(a5);
    a5=(a4);
    a4=(a3)^tmp;
    a3=(a2)^tmp;
    a2=(a1);
    a1=(a0)^tmp;
    a0=tmp;

    // o1=a^state[0]^state[8]^state[12];
    //Output of Second Columns

    o8 =a0^shared[0]^shared[16]^shared[24];
    o9 =a1^shared[1]^shared[17]^shared[25];
    o10=a2^shared[2]^shared[18]^shared[26];
    o11=a3^shared[3]^shared[19]^shared[27];
    o12=a4^shared[4]^shared[20]^shared[28];
    o13=a5^shared[5]^shared[21]^shared[29];
    o14=a6^shared[6]^shared[22]^shared[30];
    o15=a7^shared[7]^shared[23]^shared[31];

    //----- Third ROW -------//
    //Multiply by [01 01 02 03] in GF(2^8)

    // uint32_t Mul2 = state[8]^state[12];

    a0=shared[16]^shared[24];
    a1=shared[17]^shared[25];
    a2=shared[18]^shared[26];
    a3=shared[19]^shared[27];
    a4=shared[20]^shared[28];
    a5=shared[21]^shared[29];
    a6=shared[22]^shared[30];
    a7=shared[23]^shared[31];
    tmp=a7;
    a7=(a6);
    a6=(a5);
    a5=(a4);
    a4=(a3)^tmp;
    a3=(a2)^tmp;
    a2=(a1);
    a1=(a0)^tmp;
    a0=tmp;

    // o1=a^state[0]^state[8]^state[12];
    // Output of the Third Column

    o16=a0^shared[ 8]^shared[0]^shared[24];
    o17=a1^shared[ 9]^shared[1]^shared[25];
    o18=a2^shared[10]^shared[2]^shared[26];
    o19=a3^shared[11]^shared[3]^shared[27];
    o20=a4^shared[12]^shared[4]^shared[28];
    o21=a5^shared[13]^shared[5]^shared[29];
    o22=a6^shared[14]^shared[6]^shared[30];
    o23=a7^shared[15]^shared[7]^shared[31];

    //----- Fourth ROW -------//
    //Multiply by [03 01 01 02] in GF(2^8)

    // uint32_t Mul2 = state[0]^state[12];
    a0=shared[0]^shared[24];
    a1=shared[1]^shared[25];
    a2=shared[2]^shared[26];
    a3=shared[3]^shared[27];
    a4=shared[4]^shared[28];
    a5=shared[5]^shared[29];
    a6=shared[6]^shared[30];
    a7=shared[7]^shared[31];
    tmp=a7;
    a7=(a6);
    a6=(a5);
    a5=(a4);
    a4=(a3)^tmp;
    a3=(a2)^tmp;
    a2=(a1);
    a1=(a0)^tmp;
    a0=tmp;
    // o1=a^state[4]^state[8]^state[0];
    // Output of the Last Column


    o24=a0^shared[ 8]^shared[0]^shared[16];
    o25=a1^shared[ 9]^shared[1]^shared[17];
    o26=a2^shared[10]^shared[2]^shared[18];
    o27=a3^shared[11]^shared[3]^shared[19];
    o28=a4^shared[12]^shared[4]^shared[20];
    o29=a5^shared[13]^shared[5]^shared[21];
    o30=a6^shared[14]^shared[6]^shared[22];
    o31=a7^shared[15]^shared[7]^shared[23];

    //Final Assignment

    shared[0]=o0;
    shared[1]=o1;
    shared[2]=o2;
    shared[3]=o3;
    shared[4]=o4;
    shared[5]=o5;
    shared[6]=o6;
    shared[7]=o7;

    shared[8]=o8;
    shared[9]=o9;
    shared[10]=o10;
    shared[11]=o11;
    shared[12]=o12;
    shared[13]=o13;
    shared[14]=o14;
    shared[15]=o15;

    shared[16]=o16;
    shared[17]=o17;
    shared[18]=o18;
    shared[19]=o19;
    shared[20]=o20;
    shared[21]=o21;
    shared[22]=o22;
    shared[23]=o23;

    shared[24]=o24;
    shared[25]=o25;
    shared[26]=o26;
    shared[27]=o27;
    shared[28]=o28;
    shared[29]=o29;
    shared[30]=o30;
    shared[31]=o31;

 }


 #endif //_MIXCOLUMNS_H_