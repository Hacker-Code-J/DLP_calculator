#include<stdio.h>
#include<stdlib.h>

#include "setup.h"
#include "BigInteger.h"
#include "operation.h"

void add_xyk(WORD x, WORD y, WORD k, WORD* res, WORD* carry) {
    /****************
    *carry = 0;
    *res = x+y;
    if(*res < x)
        *carry = 1;

    *res = *res + k;
    if(*res < k)
        *carry += 1;
    ****************/

    //Optimize
    *res = x + y;
    *carry = (*res < x);
    *res += k;
    *carry += (*res < (x+y));

    //printf("%08x + %08x + %d = %08x * W + %08x\n", x, y, k, *carry, *res);
}

void add_xyz(BINT* X, BINT* Y, BINT* Z) {
    int n = X->wordlen;
    int m = Y->wordlen;

    WORD k = 0;
    WORD res, carry;

    // Loop until the shorter of the two numbers ends
    int i;
    for (i=0; i<m; i++) {
        add_xyk(X->val[i], Y->val[i], k, &res, &carry);
        Z->val[i] = res;
        k = carry;
    }
    // Continue adding any remaining X values with the carry, since Y is shorter
    for (; i<n; i++) {
        add_xyk(X->val[i], 0, k, &res, &carry); // just adding X values since Y is shorter
        Z->val[i] = res;
        k = carry;
    }

    if(k) Z->val[n] = k;

    // Set the sign and word length of Z
    Z->wordlen = (k == 0) ? n : n+1;

    refine_BINT(Z);
    //return Z;
}

void ADD_xyz(BINT* X, BINT* Y, BINT* Z) {
    if(X->sign==false && Y->sign==false) {
        if(compare_xy(X,Y) != -1)
            add_xyz(X,Y,Z);
        else
            add_xyz(Y,X,Z);
    } else if(X->sign==false && Y->sign==true) {
        if(compare_xy(X,Y) != -1)
            sub_xyz(X,Y,Z);
        else {
            Z->sign = true;
            sub_xyz(Y,X,Z);
        }
    } else if(X->sign==true && Y->sign==false) {
        if(compare_xy(X,Y) != -1) {
            Z->sign = true;
            sub_xyz(X,Y,Z);
        } else
            sub_xyz(Y,X,Z);
    } else {
        if(compare_xy(X,Y) != -1){
            Z->sign = true;
            add_xyz(X,Y,Z);
        } else {
            Z->sign = true;
            add_xyz(Y,X,Z);
        }
    }
}

void sub_xby(WORD x, WORD b, WORD y, WORD* res, WORD* borrow) {
    /****************
    *brrow = 0;
    *res = x-borrow;
    if(x < b)
        *borrow = 1;
    if(*res < y)
        *borrow += 1;
    *res = *res - y;
    ****************/
    //Optimize
    *res = x-b;
    *borrow = (x < b);
    *borrow += (*res < y);
    *res -= y;
    
    //printf("%08x - %d - %08x = -%08x * W + %08x\n", x, b, y, *borrow, *res);
}

void sub_xyz(BINT* X, BINT* Y, BINT* Z) {//X>=Y>0
    int n = X->wordlen;
    int m = Y->wordlen;

    WORD* tmp;
    tmp = (WORD*)realloc(Y->val,n*sizeof(WORD));
    Y->val = tmp;

    for(int i=m; i<n; i++)
        Y->val[i] = 0;

    WORD b = 0;
    WORD res, borrow;

    //
    for(int i=0; i<n; i++) {
        sub_xby(X->val[i], b, Y->val[i], &res, &borrow);
        Z->val[i] = res;
    }
    
    //refine_BINT(Z);
}

void SUB_xyz(BINT* X, BINT* Y, BINT* Z) {
    if(X->sign==false && Y->sign==false) {
        if(compare_xy(X,Y) != -1)
            sub_xyz(X,Y,Z);
        else {
            Z->sign = true;
            sub_xyz(Y,X,Z);
        }
    } else if(X->sign==false && Y->sign==true) {
        if(compare_xy(X,Y) != -1)
            add_xyz(X,Y,Z);
        else
            add_xyz(Y,X,Z);
    } else if(X->sign==true && Y->sign==false) {
        if(compare_xy(X,Y) != -1) {
            Z->sign = true;
            add_xyz(X,Y,Z);
        } else {
            Z->sign = true;
            add_xyz(Y,X,Z);
        }
    } else {
        if(compare_xy(X,Y) != -1){
            Z->sign = true;
            sub_xyz(X,Y,Z);
        } else {
            sub_xyz(Y,X,Z);
        }
    }
}

void mul_xyc(WORD x, WORD y , WORD* C){
    WORD A0,A1,B0,B1,T0,T1,C0,C1,T;
    int w = sizeof(WORD)*8;//비트 단위 계산을 바이트 값을 비트값으로 변형;
    WORD back_half_mask = (1<<(w/2))-1;//WORD에 따라 마스크값이 바뀔수 있게계산처리로 마스크값을 계산한다

    A1 = x >> (w/2);//비트 쉬프트로 앞에 절반 남기기
    A0 = x&back_half_mask;//뒤에 절반 남기기 and 연산자로 
    B1 = y >> (w/2);
    B0 = y&back_half_mask;
    T1 = A1*B0;
    T0 = A0*B1;
    T0 = (T1+T0);//지금은 32비트 기준으로 만들고 있고 c언어 자체 연산이 mod32기준이라 마스크처리없이 가능하지만 추가로 32비트가 아닌 다른WORD길이를 사용할 경우 처리가 필요 
    T1 = T0 < T1;
    C1 = A1*B1;
    C0 = A0*B0;
    T = C0;
    C0 = (C0 + (T0<<(w/2)));//지금은 32비트 기준으로 만들고 있고 c언어 자체 연산이 mod32기준이라 마스크처리없이 가능하지만 추가로 32비트가 아닌 다른WORD길이를 사용할 경우 처리가 필요 
    C1 = C1 + (T1<<(w/2)) + (T0>>(w/2)) + (C0<T);
    C[0]=C0;
    C[1] = C1;
}

// void mul_xyc(WORD X, WORD Y, WORD* C) {
//     int half_w = sizeof(WORD)*4;
//     /**
//      * if WORD = u32, then
//      * (1<<(w/2))-1 = 0x00000001 << 16(=2^4) - 1
//      *              = 0x00010000 - 1
//      *              = 0x0000ffff
//     */
//     WORD MASK_R = (1 << half_w) - 1;
//     WORD MASK_L = MASK_R << half_w;

//     WORD X0 = X & MASK_R;
//     WORD X1 = X & MASK_L;
//     WORD Y0 = Y & MASK_R;
//     WORD Y1 = Y & MASK_L;
    
//     WORD T0 = X0 * Y1;
//     WORD T1 = X1 * Y0;

//     T0 = T0 + T1;
//     T1 = T0 < T1;

//     WORD C0 = X0 * Y0;
//     WORD C1 = X1 * Y1;

//     WORD T = C0;
//     C0 += (T0 << half_w);
//     C1 += (T1 << half_w) + (T0 >> half_w) + (C0 < T);
    
//     half_w *= 2;

//     *C = C0;
//     *(C+1) = C1;
// }