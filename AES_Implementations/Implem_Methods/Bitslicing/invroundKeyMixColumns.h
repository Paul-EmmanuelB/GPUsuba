#ifndef _INV_KEY_MIXCOLUMNS_H_
#define _INV_KEY_MIXCOLUMNS_H_

__device__ void InvKeyMixcl(
int &r0 , int &r1 , int &r2 , int &r3 , int &r4 , int &r5 , int &r6 , int &r7,
int &r8 , int &r9 , int &r10, int &r11, int &r12, int &r13, int &r14, int &r15,
int &r16, int &r17, int &r18, int &r19, int &r20, int &r21, int &r22, int &r23,
int &r24, int &r25, int &r26, int &r27, int &r28, int &r29, int &r30, int &r31,
int  k0 , int  k1 , int  k2 , int  k3 , int  k4 , int  k5 , int  k6 , int  k7 ,
int  k8 , int  k9 , int  k10, int  k11, int  k12, int  k13, int  k14, int  k15,
int  k16, int  k17, int  k18, int  k19, int  k20, int  k21, int  k22, int  k23,
int  k24, int  k25, int  k26, int  k27, int  k28, int  k29, int  k30, int  k31){

    int o0  = 0 , o1  = 0 , o2  = 0, o3  = 0, o4  = 0, o5  = 0, o6  = 0, o7  = 0;
    int o8  = 0 , o9  = 0 , o10 = 0, o11 = 0, o12 = 0, o13 = 0, o14 = 0, o15 = 0;
    int o16 = 0 , o17 = 0 , o18 = 0, o19 = 0, o20 = 0, o21 = 0, o22 = 0, o23 = 0;
    int o24 = 0 , o25 = 0 , o26 = 0, o27 = 0, o28 = 0, o29 = 0, o30 = 0, o31 = 0;

    int a0,a1,a2,a3,a4,a5,a6,a7; //Mul2
    int b0,b1,b2,b3,b4,b5,b6,b7; //Mul4
    int c0,c1,c2,c3,c4,c5,c6,c7; //Mul8
    int tmp, tmp1, tmp2;

    /*
    Inverse-MixColumns in GF(2^8) with Primitive Polynomial
    g(x)=x^8+x^4+x^3+x+1
    Bitsliced Implementation 32-bit
    Function is implemented for a column of states [32 Regs * 32 bits]
    S_i Represents 8 Registers(each register include 32 bits)

    [0E 0B 0D 09] [ S0 ]
    [09 0E 0B 0D] * [ S4 ]
    [0D 09 0E 0B] [ S8 ]
    [0B 0D 09 0E] [ S12]

    */

    //----- First ROW -------//
    // Multiply by [0E 0B 0D 09] in GF(2^8)

    // int Mul2 = state[0]^state[4];

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

    //int Mul4 = state[0]^state[8];

    b0=r0^r16;
    b1=r1^r17;
    b2=r2^r18;
    b3=r3^r19;
    b4=r4^r20;
    b5=r5^r21;
    b6=r6^r22;
    b7=r7^r23;

    tmp=b7;
    tmp1=b6;
    b7=(b5);
    b6=(b4);
    b5=((b3)^tmp);
    b4=((b2)^tmp)^tmp1;
    b3=(b1)^tmp1;
    b2=((a0)^tmp);
    b1=(tmp)^tmp1;
    b0=tmp1;

    //int Mul8 = state[0]^state[4]^state[8]^state[12];
    // Changed
    c0=r0^r8^r16^r24;
    c1=r1^r9^r17^r25;
    c2=r2^r10^r18^r26;
    c3=r3^r11^r19^r27;
    c4=r4^r12^r20^r28;
    c5=r5^r13^r21^r29;
    c6=r6^r14^r22^r30;
    c7=r7^r15^r23^r31;

    tmp=c7;
    tmp1=c6;
    tmp2=c5;
    c7=(c4);
    c6=(((c3)^tmp));
    c5=(((c2)^tmp)^tmp1);
    c4=(((c1)^tmp1)^tmp)^tmp2;

    c3=(c0^tmp)^tmp2;
    c2=tmp^tmp1;
    c1=tmp1^tmp2;
    c0=tmp2;

    // o1=a^b^c^state[4]^state[8]^state[12];

    //Output of First Columns

    o0=a0^b0^c0^r8^r16^r24;
    o1=a1^b1^c1^r9^r17^r25;
    o2=a2^b2^c2^r10^r18^r26;
    o3=a3^b3^c3^r11^r19^r27;
    o4=a4^b4^c4^r12^r20^r28;
    o5=a5^b5^c5^r13^r21^r29;
    o6=a6^b6^c6^r14^r22^r30;
    o7=a7^b7^c7^r15^r23^r31;



    //----- Second ROW -------//
    //Multiply by [09 0E 0B 0D] in GF(2^8)

    // int Mul2 = state[4]^state[8];

    a0=r16^r8;
    a1=r17^r9;
    a2=r18^r10;
    a3=r19^r11;
    a4=r20^r12;
    a5=r21^r13;
    a6=r22^r14;
    a7=r23^r15;
    tmp=a7;
    a7=(a6);
    a6=(a5);
    a5=(a4);
    a4=(a3)^tmp;
    a3=(a2)^tmp;
    a2=(a1);
    a1=(a0)^tmp;
    a0=tmp;

    //int Mul4 = state[4]^state[12];

    b0=r8^r24;
    b1=r9^r25;
    b2=r10^r26;
    b3=r11^r27;
    b4=r12^r28;

    b5=r13^r29;
    b6=r14^r30;
    b7=r15^r31;

    tmp=b7;
    tmp1=b6;
    b7=(b5);
    b6=(b4);
    b5=((b3)^tmp);
    b4=((b2)^tmp)^tmp1;
    b3=(b1)^tmp1;
    b2=((a0)^tmp);
    b1=(tmp)^tmp1;
    b0=tmp1;

    //int Mul8 = state[0]^state[4]^state[8]^state[12];
    //changed
    c0=r0^r8^r16^r24;
    c1=r1^r9^r17^r25;
    c2=r2^r10^r18^r26;
    c3=r3^r11^r19^r27;
    c4=r4^r12^r20^r28;
    c5=r5^r13^r21^r29;
    c6=r6^r14^r22^r30;
    c7=r7^r15^r23^r31;

    tmp=c7;
    tmp1=c6;
    tmp2=c5;
    c7=(c4);
    c6=(((c3)^tmp));
    c5=(((c2)^tmp)^tmp1);
    c4=(((c1)^tmp1)^tmp)^tmp2;
    c3=(((c0)^tmp))^tmp2;
    c2=tmp^tmp1;
    c1=tmp1^tmp2;
    c0=tmp2;

    // o8=a^b^c^state[0]^state[8]^state[12];

    //Output of Second Columns

    o8=a0^b0^c0^r0^r16^r24;
    o9=a1^b1^c1^r1^r17^r25;
    o10=a2^b2^c2^r2^r18^r26;
    o11=a3^b3^c3^r3^r19^r27;
    o12=a4^b4^c4^r4^r20^r28;
    o13=a5^b5^c5^r5^r21^r29;
    o14=a6^b6^c6^r6^r22^r30;
    o15=a7^b7^c7^r7^r23^r31;


    //----- Third ROW -------//
    //Multiply by [0D 09 0E 0B] in GF(2^8)

    // int Mul2 = state[8]^state[12];

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

    //int Mul4 = state[0]^state[8];

    b0=r0^r16;
    b1=r1^r17;
    b2=r2^r18;
    b3=r3^r19;
    b4=r4^r20;
    b5=r5^r21;
    b6=r6^r22;
    b7=r7^r23;

    tmp=b7;
    tmp1=b6;
    b7=(b5);
    b6=(b4);
    b5=((b3)^tmp);
    b4=((b2)^tmp)^tmp1;
    b3=(b1)^tmp1;
    b2=((a0)^tmp);
    b1=(tmp)^tmp1;
    b0=tmp1;

    //int Mul8 = state[0]^state[4]^state[8]^state[12];

    c0=r0^r8^r16^r24;
    c1=r1^r9^r17^r25;
    c2=r2^r10^r18^r26;
    c3=r3^r11^r19^r27;
    c4=r4^r12^r20^r28;
    c5=r5^r13^r21^r29;
    c6=r6^r14^r22^r30;
    c7=r7^r15^r23^r31;

    tmp=c7;
    tmp1=c6;
    tmp2=c5;
    c7=(c4);
    c6=(((c3)^tmp));
    c5=(((c2)^tmp)^tmp1);
    c4=(((c1)^tmp1)^tmp)^tmp2;
    c3=(((c0)^tmp))^tmp2;
    c2=tmp^tmp1;
    c1=(tmp1)^tmp2;
    c0=tmp2;

    // o16=a^b^c^state[0]^state[4]^state[12];


    o16=a0^b0^c0^r0^r8^r24;
    o17=a1^b1^c1^r1^r9^r25;
    o18=a2^b2^c2^r2^r10^r26;
    o19=a3^b3^c3^r3^r11^r27;
    o20=a4^b4^c4^r4^r12^r28;
    o21=a5^b5^c5^r5^r13^r29;
    o22=a6^b6^c6^r6^r14^r30;
    o23=a7^b7^c7^r7^r15^r31;

    //----- Fourth ROW -------//
    //Multiply by [0B 0D 09 0E] in GF(2^8)

    // int Mul2 = state[0]^state[12];

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

    //int Mul4 = state[4]^state[12];

    b0=r8^r24;
    b1=r9^r25;
    b2=r10^r26;
    b3=r11^r27;
    b4=r12^r28;
    b5=r13^r29;
    b6=r14^r30;
    b7=r15^r31;

    tmp=b7;
    tmp1=b6;
    b7=(b5);
    b6=(b4);
    b5=((b3)^tmp);
    b4=((b2)^tmp)^tmp1;
    b3=(b1)^tmp1;
    b2=((a0)^tmp);
    b1=(tmp)^tmp1;
    b0=tmp1;

    //int Mul8 = state[0]^state[4]^state[8]^state[12];

    c0=r0^r8^r16^r24;
    c1=r1^r9^r17^r25;
    c2=r2^r10^r18^r26;
    c3=r3^r11^r19^r27;
    c4=r4^r12^r20^r28;
    c5=r5^r13^r21^r29;
    c6=r6^r14^r22^r30;
    c7=r7^r15^r23^r31;

    tmp=c7;
    tmp1=c6;
    tmp2=c5;
    c7=(c4);
    c6=(((c3)^tmp));
    c5=(((c2)^tmp)^tmp1);
    c4=(((c1)^tmp1)^tmp)^tmp2;
    c3=(((c0)^tmp))^tmp2;
    c2=tmp^tmp1;
    c1=tmp1^tmp2;
    c0=tmp2;

    // o24=a^b^c^state[0]^state[4]^state[8];


    o24=a0^b0^c0^r0^r8^r16;
    o25=a1^b1^c1^r1^r9^r17;
    o26=a2^b2^c2^r2^r10^r18;
    o27=a3^b3^c3^r3^r11^r19;
    o28=a4^b4^c4^r4^r12^r20;
    o29=a5^b5^c5^r5^r13^r21;
    o30=a6^b6^c6^r6^r14^r22;
    o31=a7^b7^c7^r7^r15^r23;


    //Final Assignments

    r0=o0^k0;
    r1=o1^k1;
    r2=o2^k2;
    r3=o3^k3;
    r4=o4^k4;
    r5=o5^k5;
    r6=o6^k6;
    r7=o7^k7;

    r8=o8^k8;
    r9=o9^k9;
    r10=o10^k10;
    r11=o11^k11;
    r12=o12^k12;
    r13=o13^k13;
    r14=o14^k14;
    r15=o15^k15;

    r16=o16^k16;
    r17=o17^k17;
    r18=o18^k18;
    r19=o19^k19;
    r20=o20^k20;
    r21=o21^k21;
    r22=o22^k22;
    r23=o23^k23;

    r24=o24^k24;
    r25=o25^k25;
    r26=o26^k26;
    r27=o27^k27;
    r28=o28^k28;
    r29=o29^k29;
    r30=o30^k30;
    r31=o31^k31;
}

#endif //_INV_KEY_MIXCOLUMNS_H_