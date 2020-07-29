#ifndef _SUBBYTES_H_
#define _SUBBYTES_H_

#include <stdio.h>
#include <sys/types.h>
//static int
 // inv-Trans1[8]= {0x98, 0xF3, 0xF2, 0x48, 0x09, 0x81, 0xA9, 0xFF};
 // Trans1[8]= {0x58, 0x2D, 0x9E, 0x0B, 0xDC, 0x04, 0x03, 0x24};
 /*Bitsliced Avalanche GF() -Sbox:*/

template <typename T>
__device__ __forceinline__ void Sbox(T &r0,T &r1,T &r2,T &r3,T &r4,T &r5,T &r6,T &r7){
    /*Changing Basic Matrix
    By : (Trans1)={0x58, 0x2D, 0x9E, 0x0B, 0xDC, 0x04, 0x03, 0x24}*/
    T y0 = (r0^r1^r2^r3^r6);
    T y1 = (r0^r5^r6);
    T y2 = (r0);
    T y3 = (r0^r1^r3^r4^r7);
    T y4 = (r0^r5^r6^r7);
    T y5 = (r0^r1^r5^r6);
    T y6 = (r0^r4^r5^r6);
    T y7 = (r0^r1^r2^r5^r6^r7);

    /* Sbox-Logic Circuit */

    /* Break Computation basis from GF(2^8) to GF(2^4) */

    /* 1- Square and Scale in GF(2^4)/GF(2^2) */
    /* 1-2 Scale in GF(2^2) */
    /* 1-3 Square in GF(2^2) */
    /* 1-4 Back to GF(2^4) */
    T v0=y0^y4^y6^y2;
    T v1=y1^y5^y7^y3;
    T m0=y1^y5;
    T m1=y0^y4;
    T c0=m1;
    T c1=m0^m1;
    T c2=v1;
    T c3=v0;
    T e=(y4^y5^y6^y7)&(y0^y1^y2^y3);
    T e0=((y6^y4)&(y0^y2))^e;
    T e1=((y5^y7)&(y1^y3))^e;
    T d1=e1;
    e1=e0;
    e0=d1^e0;

    /* 2- Multiply in GF(2^4) */
    /* 2-1 Break Computation basis from GF(2^4) to GF(2^2)*/
    /* 2-2 Scale in GF(2^2)*/
    /* 2-3 Multiply in GF(2^2)*/

    /* 2-4 Back to GF(2^4) */

    T p=(y7^y6)&(y2^y3);
    T d2=(y6&y2)^p^e0;
    T d3=(y7&y3)^p^e1;
    T q=(y5^y4)&(y1^y0);
    T d0=(y4&y0)^q^e0;
    d1=(y5&y1)^q^e1;
    v0=c2^d2;
    v1=c3^d3;
    m0=c0^d0;
    m1=c1^d1;
    d0=v1^m1;
    d1=v0^m0;
    c1=d0;
    c0=d1^d0;
    /* 3- Multiplicative Inverse in GF(2^4) */
    /* 3-1 Break Computation basis from GF(2^4) to GF(2^2)*/
    /* 3-2 Scale in GF(2^2)*/
    /* 3-3 Square in GF(2^2)*/
    /* 3-4 Multiply in GF(2^2)*/
    /* 3-5 Square in GF(2^2)*/
    /* 3-6 Back to GF(2^4) */
    T d=(v1^v0)&(m1^m0);
    d0=(v0&m0)^d;
    d1=(v1&m1)^d;
    e0=c1^d1;
    e1=c0^d0;
    p=(e1^e0)&(m1^m0);
    T p0=(e0&m0)^p;
    T p1=(e1&m1)^p;
    q=(e1^e0)&(v1^v0);
    T q0=(e0&v0)^q;
    T q1=(e1&v1)^q;
    T e3=p1;
    T e2=p0;
    e1=q1;
    e0=q0;
    v0=e2;
    v1=e3;
    m0=e0;
    m1=e1;
    /* 4- Multiply in GF(2^4) */
    /* 4-1 Break Computation basis from GF(2^4) to GF(2^2)*/
    /* 4-2 Scale in GF(2^2)*/
    /* 4-3 Multiply in GF(2^2)*/
    /* 4-4 Back to GF(2^4) */
    /* Back to GF(2^8) */

    T c=(v1^m1^v0^m0)&(y0^y1^y2^y3);

    c0=((v0^m0)&(y2^y0))^c;
    c1=((v1^m1)&(y3^y1))^c;
    T temp=c1;
    c1=c0;
    c0=temp^c0;
    p=(v1^v0)&(y3^y2);
    T p2=(v0&y2)^p^c0;
    T p3=(v1&y3)^p^c1;
    q=(m1^m0)&(y1^y0);
    p0=(m0&y0)^q^c0;
    p1=(m1&y1)^q^c1;
    d1=e1;
    d0=e0;
    c=(y4^y5^y6^y7)&(e0^e1^e2^e3);
    c0=((y6^y4)&(e2^e0))^c;
    c1=((y7^y5)&(e3^e1))^c;
    temp=c1;
    c1=c0;
    c0=temp^c0;
    e=(y7^y6)&(e2^e3);
    e0=(y6&e2)^e^c0;
    e1=(y7&e3)^e^c1;
    q=(y5^y4)&(d0^d1);
    q0=(y4&d0)^q^c0;
    q1=(y5&d1)^q^c1;

    /*Changing Basic Matrix
    By : inv-Trans1={0x98, 0xF3, 0xF2, 0x48, 0x09, 0x81, 0xA9, 0xFF}*/
    /* Memory Optimization Level
    r0=q0;r1=q1;
    r2=e0;r3=e1;
    r4=p0;r5=p1;r6=p2;r7=p3;
    */
    /*Adding 0x63 to the Registers:*/

    r0 = (q1^p0^p2^0xffffffff);
    r1 = (q1^p0^p1^0xffffffff);
    r2 = (q0^e0^e1^p1^p2);
    r3 = (e1^p0^p1^p2^p3);
    r4 = (e1^p1^p3);
    r5 = (q0^p2^0xffffffff);
    r6 = (e1^p3^0xffffffff);
    r7 = (e1^p1);
}


__device__ void Sbox(uint32_t *shared){
    /*Changing Basic Matrix
    By : (Trans1)={0x58, 0x2D, 0x9E, 0x0B, 0xDC, 0x04, 0x03, 0x24}*/
    uint32_t y0 = (shared[0]^shared[1]^shared[2]^shared[3]^shared[6]);
    uint32_t y1 = (shared[0]^shared[5]^shared[6]);
    uint32_t y2 = (shared[0]);
    uint32_t y3 = (shared[0]^shared[1]^shared[3]^shared[4]^shared[7]);
    uint32_t y4 = (shared[0]^shared[5]^shared[6]^shared[7]);
    uint32_t y5 = (shared[0]^shared[1]^shared[5]^shared[6]);
    uint32_t y6 = (shared[0]^shared[4]^shared[5]^shared[6]);
    uint32_t y7 = (shared[0]^shared[1]^shared[2]^shared[5]^shared[6]^shared[7]);

    /* Sbox-Logic Circuit */

    /* Break Computation basis from GF(2^8) to GF(2^4) */

    /* 1- Square and Scale in GF(2^4)/GF(2^2) */
    /* 1-2 Scale in GF(2^2) */
    /* 1-3 Square in GF(2^2) */
    /* 1-4 Back to GF(2^4) */
    uint32_t v0=y0^y4^y6^y2;
    uint32_t v1=y1^y5^y7^y3;
    uint32_t m0=y1^y5;
    uint32_t m1=y0^y4;
    uint32_t c0=m1;
    uint32_t c1=m0^m1;
    uint32_t c2=v1;
    uint32_t c3=v0;
    uint32_t e=(y4^y5^y6^y7)&(y0^y1^y2^y3);
    uint32_t e0=((y6^y4)&(y0^y2))^e;
    uint32_t e1=((y5^y7)&(y1^y3))^e;
    uint32_t d1=e1;
    e1=e0;
    e0=d1^e0;

    /* 2- Multiply in GF(2^4) */
    /* 2-1 Break Computation basis from GF(2^4) to GF(2^2)*/
    /* 2-2 Scale in GF(2^2)*/
    /* 2-3 Multiply in GF(2^2)*/

    /* 2-4 Back to GF(2^4) */

    uint32_t p=(y7^y6)&(y2^y3);
    uint32_t d2=(y6&y2)^p^e0;
    uint32_t d3=(y7&y3)^p^e1;
    uint32_t q=(y5^y4)&(y1^y0);
    uint32_t d0=(y4&y0)^q^e0;
    d1=(y5&y1)^q^e1;
    v0=c2^d2;
    v1=c3^d3;
    m0=c0^d0;
    m1=c1^d1;
    d0=v1^m1;
    d1=v0^m0;
    c1=d0;
    c0=d1^d0;
    /* 3- Multiplicative Inverse in GF(2^4) */
    /* 3-1 Break Computation basis from GF(2^4) to GF(2^2)*/
    /* 3-2 Scale in GF(2^2)*/
    /* 3-3 Square in GF(2^2)*/
    /* 3-4 Multiply in GF(2^2)*/
    /* 3-5 Square in GF(2^2)*/
    /* 3-6 Back to GF(2^4) */
    uint32_t d=(v1^v0)&(m1^m0);
    d0=(v0&m0)^d;
    d1=(v1&m1)^d;
    e0=c1^d1;
    e1=c0^d0;
    p=(e1^e0)&(m1^m0);
    uint32_t p0=(e0&m0)^p;
    uint32_t p1=(e1&m1)^p;
    q=(e1^e0)&(v1^v0);
    uint32_t q0=(e0&v0)^q;
    uint32_t q1=(e1&v1)^q;
    uint32_t e3=p1;
    uint32_t e2=p0;
    e1=q1;
    e0=q0;
    v0=e2;
    v1=e3;
    m0=e0;
    m1=e1;
    /* 4- Multiply in GF(2^4) */
    /* 4-1 Break Computation basis from GF(2^4) to GF(2^2)*/
    /* 4-2 Scale in GF(2^2)*/
    /* 4-3 Multiply in GF(2^2)*/
    /* 4-4 Back to GF(2^4) */
    /* Back to GF(2^8) */

    uint32_t c=(v1^m1^v0^m0)&(y0^y1^y2^y3);

    c0=((v0^m0)&(y2^y0))^c;
    c1=((v1^m1)&(y3^y1))^c;
    uint32_t temp=c1;
    c1=c0;
    c0=temp^c0;
    p=(v1^v0)&(y3^y2);
    uint32_t p2=(v0&y2)^p^c0;
    uint32_t p3=(v1&y3)^p^c1;
    q=(m1^m0)&(y1^y0);
    p0=(m0&y0)^q^c0;
    p1=(m1&y1)^q^c1;
    d1=e1;
    d0=e0;
    c=(y4^y5^y6^y7)&(e0^e1^e2^e3);
    c0=((y6^y4)&(e2^e0))^c;
    c1=((y7^y5)&(e3^e1))^c;
    temp=c1;
    c1=c0;
    c0=temp^c0;
    e=(y7^y6)&(e2^e3);
    e0=(y6&e2)^e^c0;
    e1=(y7&e3)^e^c1;
    q=(y5^y4)&(d0^d1);
    q0=(y4&d0)^q^c0;
    q1=(y5&d1)^q^c1;

    /*Changing Basic Matrix
    By : inv-Trans1={0x98, 0xF3, 0xF2, 0x48, 0x09, 0x81, 0xA9, 0xFF}*/
    /* Memory Optimization Level
    shared[0]=q0;shared[1]=q1;
    shared[2]=e0;shared[3]=e1;
    shared[4]=p0;shared[5]=p1;shared[6]=p2;shared[7]=p3;
    */
    /*Adding 0x63 to the Registers:*/

    shared[0] = (q1^p0^p2^0xffffffff);
    shared[1] = (q1^p0^p1^0xffffffff);
    shared[2] = (q0^e0^e1^p1^p2);
    shared[3] = (e1^p0^p1^p2^p3);
    shared[4] = (e1^p1^p3);
    shared[5] = (q0^p2^0xffffffff);
    shared[6] = (e1^p3^0xffffffff);
    shared[7] = (e1^p1);
}

#endif //_SUBBYTES_H_