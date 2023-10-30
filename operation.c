#include <stdio.h>
#include <stdlib.h>

#include "setup.h"
#include "BigInteger.h"
#include "operation.h"

void NOT_BINT(BINT** pptrbint_dst, BINT** pptrbint_src) {
    BINT* ptrbint_dst = *pptrbint_dst;
    BINT* ptrbint_src = *pptrbint_src;
    for (int i = 0; i < ptrbint_src->wordlen; i++) {
        ptrbint_dst->val[i] = ~(ptrbint_src->val[i]);
    }
    ptrbint_dst->wordlen = ptrbint_src->wordlen; // The result size will be the same as the input
    // (*pptrZ)->sign = !(ptrX->sign);    // Negative if input is positive and vice versa
}

void AND_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ) {
    int min_len = ptrX->wordlen < ptrY->wordlen ? ptrX->wordlen : ptrY->wordlen;
    for (int i = 0; i < min_len; i++) {
        (*pptrZ)->val[i] = ptrX->val[i] & ptrY->val[i];
    }
    (*pptrZ)->wordlen = min_len; // The result size will be size of the smaller operand
    // ptrZ->sign = ptrX->sign && ptrY->sign; // Negative if both operands are negative
}

void OR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ) {
    int min_len = ptrX->wordlen < ptrY->wordlen ? ptrX->wordlen : ptrY->wordlen;
    for (int i = 0; i < min_len; i++) {
        (*pptrZ)->val[i] = ptrX->val[i] | ptrY->val[i];
    }
    (*pptrZ)->wordlen = min_len; // The result size will be size of the smaller operand
    // ptrZ->sign = ptrX->sign && ptrY->sign; // Negative if both operands are negative
}

void XOR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ) {
    int min_len = ptrX->wordlen < ptrY->wordlen ? ptrX->wordlen : ptrY->wordlen;
    for (int i = 0; i < min_len; i++) {
        (*pptrZ)->val[i] = ptrX->val[i] ^ ptrY->val[i];
    }
    (*pptrZ)->wordlen = min_len; 
    // Resulting sign depends on the signs of operands. You can decide on a rule, 
    // but for now, let's say it's negative if either of the operands is negative.
    // (*pptrZ)->sign = ptrX->sign || ptrY->sign; 
}

void add_carry(WORD x, WORD y, WORD k, WORD* ptrQ, WORD* ptrR) {
    if (WORD_BITLEN == 8 || WORD_BITLEN == 32) {
        u64 result = (u64)x + (u64)y + (u64)k;
        *ptrR = (WORD)result;
        *ptrQ = result >> WORD_BITLEN;
    } else if (WORD_BITLEN == 64) {
        const WORD HALF_MASK = (1ULL << 32) - 1;
        const u64 LOW_X = x & HALF_MASK;
        const u64 HIGH_X = x >> 32;
        const u64 LOW_Y = y & HALF_MASK;
        const u64 HIGH_Y = y >> 32;

        // Add the lower halves
        u64 low_result = LOW_X + LOW_Y + (k & HALF_MASK);

        // Check if there was a carry from the lower half
        u64 low_carry = (low_result > HALF_MASK) ? 1 : 0;

        // Add the upper halves
        u64 high_result = HIGH_X + HIGH_Y + (k >> 32) + low_carry;

        // Return results
        *ptrR = (high_result << 32) | (low_result & HALF_MASK);
        *ptrQ = (high_result >> 32);  // Carry from the higher half
    } else {
        fprintf(stderr, "Unsupported WORD size in 'add_carry'\n");
        exit(1);
    }
    // *ptrR = x + y;
	// *ptrQ = (*ptrR < x);
	// *ptrR += k;
	// *ptrQ += (*ptrR < (x+y));
}

void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;
    BINT* ptrZ = *pptrZ;
    
    int n = ptrX->wordlen;
    int m = ptrY->wordlen;

    if(n < m) {
        printf("\nwordlen X = %d, wordlen Y = %d\n",ptrX->wordlen, ptrY->wordlen);
        fprintf(stderr, "Error: wordlen(X) < wordlen(Y) is not vaild in 'add_core_xyz'\n");
        exit(1);
    }

    int max_len = MAX(n, m);

    // Ensure ptrZ has enough allocated space
    if (ptrZ->wordlen < max_len + 1) {
        WORD* tmp = NULL;
        tmp = (WORD*)realloc(ptrZ->val, (max_len + 1) * sizeof(WORD));
        ptrZ->val = tmp;
        if (!ptrZ->val) {
            fprintf(stderr, "Error: Memory reallocation failed in 'add_core_xyz'\n");
            exit(1);
        }
        ptrZ->wordlen = max_len + 1;
    }

    WORD k = 0;
    WORD res = 0;
    WORD carry = 0;

    int i;
    for (i = 0; i < MIN(n, m); i++) {
        add_carry(ptrX->val[i], ptrY->val[i], k, &carry, &res);
        ptrZ->val[i] = res;
        k = carry;
    }

    // Continue with remaining values of the longer number and the carry
    while (i < max_len) {
        WORD val = (i < n) ? ptrX->val[i] : 0;
        res = val + k;
        carry = (res < k);
        ptrZ->val[i] = res;
        k = carry;
        i++;
    }

    if(k) {
        ptrZ->val[max_len] = k;
        ptrZ->wordlen = max_len+1;
    } else {
        ptrZ->wordlen = max_len;
    }

    refine_BINT(ptrZ);
}

// void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
//     BINT* ptrX = *pptrX;
//     BINT* ptrY = *pptrY;
//     BINT* ptrZ = *pptrZ;
    
//     int n = ptrX->wordlen;
//     int m = ptrY->wordlen;

//     // Ensure ptrZ has enough allocated space
//     if (ptrZ->wordlen < MAX(n, m) + 1) {
//         ptrZ->val = (WORD*)realloc(ptrZ->val, (MAX(n, m) + 1) * sizeof(WORD));
//         if (!ptrZ->val) {
//             fprintf(stderr, "Error: Memory reallocation failed in 'add_core_xyz'\n");
//             exit(1);
//         }
//         ptrZ->wordlen = MAX(n, m) + 1;
//     }

//     WORD k = 0;
//     WORD res = 0;
//     WORD carry = 0;

//     // Loop until the shorter of the two numbers ends
//     int i;
//     for (i = 0; i < m; i++) {
//         add_carry(ptrX->val[i], ptrY->val[i], k, &carry, &res);
//         ptrZ->val[i] = res;
//         k = carry;
//     }
//     // Continue adding any remaining X values with the carry, since Y is shorter
//     for (; i < n; i++) {
//         res = ptrX->val[i] + k;
//         carry = (res < k);
//         ptrZ->val[i] = res;
//         k = carry;
//     }

//     if(k) {
//         ptrZ->val[n] = k;
//         ptrZ->wordlen = n+1;
//     } else {
//         ptrZ->wordlen = n;
//     }

//     refine_BINT(ptrZ);
//     //return Z;
// }

void ADD(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "ADD");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "ADD");
    
    // If *pptrZ is NULL, allocate memory for it
    if (!*pptrZ) {
        *pptrZ = (BINT*) malloc(sizeof(BINT));
        if (!*pptrZ) {
            fprintf(stderr, "Error: Memory allocation failed in 'ADD'\n");
            exit(1);
        }
    }
    // CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "ADD");

    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;
    
    // Determine order based on magnitude, irrespective of sign.
    bool xGeqy = compare_abs_bint(pptrX, pptrY);
    
    // If signs are same, then addition is straightforward.
    if (!(ptrX->sign == ptrY->sign)) {
        if (ptrX->sign) { // If both are negative
            (*pptrZ)->sign = true;
        }
        add_core_xyz(xGeqy ? pptrX : pptrY, xGeqy ? pptrY : pptrX, pptrZ);
    } else { // If signs are different, then subtraction is needed.
        if (ptrX->sign) {
            (*pptrZ)->sign = !xGeqy; // X negative and Y positive
        } else {
            (*pptrZ)->sign = xGeqy; // X positive and Y negative
        }
        // sub_xyz(xGeqy ? ptrX : ptrY, xGeqy ? ptrY : ptrX, *pptrZ);
    }
}

// void ADD_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
//     BINT* X = *pptrX;
//     BINT* Y = *pptrY;
//     if(X->sign==false && Y->sign==false) {
//         if(compare_xy(X,Y) != -1)
//             add_core_xyz(X,Y,pptrZ);
//         else
//             add_core_xyz(Y,X,pptrZ);
//     } else if(X->sign==false && Y->sign==true) {
//         if(compare_xy(X,Y) != -1)
//             // sub_xyz(X,Y,Z);
//         else {
//             // Z->sign = true;
//             // sub_xyz(Y,X,Z);
//         }
//     } else if(X->sign==true && Y->sign==false) {
//         if(compare_xy(X,Y) != -1) {
//             // Z->sign = true;
//             // sub_xyz(X,Y,Z);
//         } else
//             // sub_xyz(Y,X,Z);
//     } else {
//         if(compare_xy(X,Y) != -1){
//             // Z->sign = true;
//             add_core_xyz(X,Y,pptrZ);
//         } else {
//             // Z->sign = true;
//             add_core_xyz(Y,X,pptrZ);
//         }
//     }
// }

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

void sub_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {//X>=Y>0
    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;
    BINT* ptrZ = *pptrZ;
    
    int n = ptrX->wordlen;
    int m = ptrY->wordlen;

    if(ptrY->sign == true) {
        fprintf(stderr, "Error: 'ptrY->sign' is negative 'sub_core_xyz'\n");
        exit(1);
    }
    if(n < m) {
        printf("\nwordlen X = %d, wordlen Y = %d\n",ptrX->wordlen, ptrY->wordlen);
        fprintf(stderr, "Error: wordlen(X) < wordlen(Y) is not vaild in 'sub_core_xyz'\n");
        exit(1);
    }

    WORD* tmp;
    tmp = (WORD*)realloc(ptrY->val,n*sizeof(WORD));
    ptrY->val = tmp;

    for(int i=m; i<n; i++)
        ptrY->val[i] = 0;

    WORD b = 0;
    WORD res, borrow;

    //
    for(int i=0; i<n; i++) {
        sub_xby(ptrX->val[i], b, ptrY->val[i], &res, &borrow);
        ptrZ->val[i] = res;
    }
    
    //refine_BINT(Z);
}

void SUB(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;
    BINT* ptrZ = *pptrZ;
    if(ptrX->sign==false && ptrY->sign==false) {
        if(compare_abs_bint(pptrX,pptrY))
            sub_core_xyz(pptrX,pptrY,pptrZ);
        else {
            ptrZ->sign = true;
            sub_core_xyz(pptrY,pptrX,pptrZ);
        }
    } else if(ptrX->sign==false && ptrY->sign==true) {
        if(compare_abs_bint(pptrX,pptrY))
            add_core_xyz(pptrX,pptrY,pptrZ);
        else
            add_core_xyz(pptrY,pptrX,pptrZ);
    } else if(ptrX->sign==true && ptrY->sign==false) {
        if(compare_abs_bint(pptrX,pptrY)) {
            ptrZ->sign = true;
            add_core_xyz(pptrX,pptrY,pptrZ);
        } else {
            ptrZ->sign = true;
            add_core_xyz(pptrY,pptrX,pptrZ);
        }
    } else {
        if(compare_abs_bint(pptrX,pptrY)){
            ptrZ->sign = true;
            sub_core_xyz(pptrX,pptrY,pptrZ);
        } else {
            sub_core_xyz(pptrY,pptrX,pptrZ);
        }
    }
}

// void mul_xyc(WORD x, WORD y , WORD* C){
//     WORD A0,A1,B0,B1,T0,T1,C0,C1,T;
//     int w = sizeof(WORD)*8;//비트 단위 계산을 바이트 값을 비트값으로 변형;
//     WORD back_half_mask = (1<<(w/2))-1;//WORD에 따라 마스크값이 바뀔수 있게계산처리로 마스크값을 계산한다

//     A1 = x >> (w/2);//비트 쉬프트로 앞에 절반 남기기
//     A0 = x&back_half_mask;//뒤에 절반 남기기 and 연산자로 
//     B1 = y >> (w/2);
//     B0 = y&back_half_mask;
//     T1 = A1*B0;
//     T0 = A0*B1;
//     T0 = (T1+T0);//지금은 32비트 기준으로 만들고 있고 c언어 자체 연산이 mod32기준이라 마스크처리없이 가능하지만 추가로 32비트가 아닌 다른WORD길이를 사용할 경우 처리가 필요 
//     T1 = T0 < T1;
//     C1 = A1*B1;
//     C0 = A0*B0;
//     T = C0;
//     C0 = (C0 + (T0<<(w/2)));//지금은 32비트 기준으로 만들고 있고 c언어 자체 연산이 mod32기준이라 마스크처리없이 가능하지만 추가로 32비트가 아닌 다른WORD길이를 사용할 경우 처리가 필요 
//     C1 = C1 + (T1<<(w/2)) + (T0>>(w/2)) + (C0<T);
//     C[0]=C0;
//     C[1] = C1;
// }

// void mul_core_ImpTxtBk_xyz(BINT* ptrX, BINT* ptrY, BINT** pptrZ) {
//     makeEven(ptrX); makeEven(ptrY);
//     *pptrZ = init_bint(pptrZ, ptrX->wordlen + ptrY->wordlen);
//     SET_BINT_ZERO(pptrZ);

//     int idx1 = ptrX->wordlen;
//     int idx2 = ptrY->wordlen / 2; 

//     BINT* ptrTmp = NULL;
//     BINT* ptrT0 = NULL;
//     BINT* ptrT1 = NULL;
//     BINT* ptrTmp0 = NULL;
//     BINT* ptrTmp1 = NULL;

//     for(int i = 0; i < idx1; i++) {
//         ptrTmp = init_bint(&ptrTmp, ptrX->wordlen+1);
//         ptrT0 = init_bint(&ptrT0, ptrX->wordlen);
//         ptrT1 = init_bint(&ptrT1, ptrX->wordlen+1);
//         ptrTmp0 = init_bint(&ptrTmp0, 2);
//         ptrTmp1 = init_bint(&ptrTmp1, 2);
//         for(int j = 0; j < idx2; j++) {
//             mul_xyz(ptrX->val[2*j], ptrY->val[i], &ptrTmp0);
//             ptrT0->val[0] = ptrTmp->val[0];
//             ptrT0->val[1] = ptrTmp->val[1];
//             left_shift(&ptrT0, 2*WORD_BITLEN);

//             mul_xyz(ptrX->val[2*j+1], ptrY->val[i], &ptrTmp1);
//             ptrT1->val[0] = ptrTmp->val[0];
//             ptrT1->val[1] = ptrTmp->val[1];
//             left_shift(&ptrT1, 2*WORD_BITLEN);

//             if(j == idx2 - 1) {
//                 left_shift(&ptrT1, WORD_BITLEN);
//                 ptrT1->val[0] = 0x00;
//             }
//         }
//         add_core_xyz(ptrT1, ptrT0, &ptrTmp);
//         left_shift(&ptrTmp, i*WORD_BITLEN);
//         add_core_xyz(*pptrZ, ptrTmp, pptrZ);

//         delete_bint(&ptrTmp);
//         delete_bint(&ptrT0);
//         delete_bint(&ptrT1);
//         delete_bint(&ptrTmp0);
//         delete_bint(&ptrTmp1);
//     }
// }

// void mul_core_ImpTxtBk_xyz(BINT* ptrX, BINT* ptrY, BINT** pptrZ) {
//     makeEven(ptrX); makeEven(ptrY);
//     init_bint(pptrZ, ptrX->wordlen + ptrY->wordlen);
//     SET_BINT_ZERO(pptrZ);

//     init_bint(pptrZ, ptrX->wordlen + ptrY->wordlen);
//     (*pptrZ)->sign = ptrX->sign ^ ptrY->sign; // Result is negative if signs of input numbers are different

//     for (int j = 0; j < ptrY->wordlen; j++) {
//         BINT* T = NULL;
//         init_bint(&T, ptrX->wordlen + 1); // +1 for potential overflow

//         for (int k = 0; k < ptrX->wordlen; k++) {
//             WORD prod = ptrX->val[k] * ptrY->val[j];
//             T->val[k] = prod; // This will automatically split between lower and upper parts of prod

//             left_shift(&T, k * WORD_BITLEN);
            
//             BINT* newZ = NULL;
//             ADD(*pptrZ, T, &newZ);
//             // add_core_xyz(*pptrZ, T, &newZ);

//             delete_bint(pptrZ);
//             *pptrZ = newZ;
//         }

//         delete_bint(&T);
//     }
// }

// void mul_core_ImpTxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
//     BINT* ptrX = *pptrX;
//     BINT* ptrY = *pptrY;
//     makeEven(ptrX);
//     makeEven(ptrY);

//     init_bint(pptrZ, ptrX->wordlen + ptrY->wordlen);
//     SET_BINT_ZERO(pptrZ);

//     (*pptrZ)->sign = ptrX->sign ^ ptrY->sign; // Result is negative if signs of input numbers are different

//     for (int j = 0; j < ptrY->wordlen; j++) {
//         for (int k = 0; k < ptrX->wordlen; k++) {
//             // Calculating product
//             WORD lowerPart = ptrX->val[k] * ptrY->val[j];
//             WORD upperPart = (ptrX->val[k] * ptrY->val[j]) >> WORD_BITLEN;

//             // Constructing a temporary BINT for the product
//             BINT* T = NULL;
//             init_bint(&T, 2);
//             T->val[0] = lowerPart;
//             T->val[1] = upperPart;

//             // Shifting the temporary BINT
//             left_shift(&T, k * WORD_BITLEN + j * WORD_BITLEN);

//             // Adding the shifted product to the result
//             BINT* newZ = NULL;
//             ADD(*pptrZ, T, &newZ);
//             delete_bint(pptrZ);
//             *pptrZ = newZ;

//             delete_bint(&T);
//         }
//     }
// }

// void mul_core_ImpTxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
//     delete_bint(pptrZ);
//     SET_BINT_ZERO(pptrZ);

//     BINT* ptrX = *pptrX;
//     BINT* ptrY = *pptrY;
//     makeEven(ptrX);
//     makeEven(ptrY);

//     if(ptrX->sign != ptrY->sign)
//         (*pptrZ)->sign = true;
// }


void mul_xyz(WORD valX, WORD valY, BINT** pptrZ) {
	// if (!pptrZ || !*pptrZ || !(*pptrZ)->val) {
	// 	return;
	// }
    // If *pptrZ is NULL, allocate memory for it
    if (!*pptrZ) {
        *pptrZ = init_bint(pptrZ, 2);
        if (!*pptrZ) {
            fprintf(stderr, "Error: Memory allocation failed in 'ADD'\n");
            exit(1);
        }
    }
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_xyz");

    const int half_w = WORD_BITLEN / 2; // if w=32, half_w = 16 = 2^4
	const WORD MASK = (1 << half_w) - 1;
	
    // Split the WORDs into halves
	WORD X0 = valX & MASK;
	WORD X1 = valX >> half_w;
	WORD Y0 = valY & MASK;
	WORD Y1 = valY >> half_w;
	
    // Cross multiplication
	WORD T0 = X0 * Y1;
	WORD T1 = X1 * Y0;
	T0 = T0 + T1;
	T1 = T0 < T1; // overflow
	
    // Direct multiplication
	WORD Z0 = X0 * Y0;
	WORD Z1 = X1 * Y1;
	
    // Adjust for overflows
	WORD T = Z0;
	Z0 += (T0 << half_w);
	Z1 += (T1 << half_w) + (T0 >> half_w) + (Z0 < T);
	
    // Set results
	(*pptrZ)->val[0] = Z0;
	(*pptrZ)->val[1] = Z1;
}

void mul_core_ImpTxtBk_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "MUL_Core_ImpTxtBk");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "MUL_Core_ImpTxtBk");

    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;
    
    int n = ptrX->wordlen;
    int m = ptrX->wordlen;

    matchSize(ptrX, ptrY);
    makeEven(ptrX); makeEven(ptrY);

    delete_bint(pptrZ);
    *pptrZ = init_bint(pptrZ, n+m);
    if (!*pptrZ) {
        fprintf(stderr, "Error: Memory allocation failed in 'mul_core_TxtBk_xyz'\n");
        exit(1);
    }
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_ImpTxtBk");
    BINT* ptrZ = *pptrZ;

    int p = ptrX->wordlen / 2;
    int q = ptrY->wordlen / 2;

    BINT* ptrT = init_bint(&ptrT, n+m);
    BINT* ptrT0 = init_bint(&ptrT0, 2*p);
    BINT* ptrT1 = init_bint(&ptrT1, 2*p);
    BINT* ptrTmp0 = init_bint(&ptrTmp0, 2*p);
    BINT* ptrTmp1 = init_bint(&ptrTmp1, 2*p);

    for(int j = 0; j < 2 * q; j++) {
        for(int k = 0; k < p; k++) {
            reset_bint(ptrTmp0);
            reset_bint(ptrTmp1);
           
            mul_xyz(ptrX->val[2*k], ptrY->val[j], &ptrTmp0);
            mul_xyz(ptrX->val[2*k+1], ptrY->val[j], &ptrTmp1);
            // printf("\nx[%d]*y[%d]=Tmp0: ",2*k,j);printHex2(ptrTmp0);printf("\n");
            // printf("x[%d]*y[%d]=Tmp1: ",2*k+1,j);printHex2(ptrTmp1);printf("\n");
            if (!k) {
                copy_BINT(&ptrT0, &ptrTmp0);
                copy_BINT(&ptrT1, &ptrTmp1);
                // printf("--T0: ");printHex2(ptrT0);printf("\n");
                // printf("--T1: ");printHex2(ptrT1);printf("\n");
            } else {
                left_shift_word(&ptrTmp0, 2*k);
                refine_BINT_word(ptrTmp0, 2*k);
                OR_BINT(ptrTmp0, ptrT0, &ptrT0);

                left_shift_word(&ptrTmp1, 2*k);
                refine_BINT_word(ptrTmp1, 2*k);
                OR_BINT(ptrTmp1, ptrT1, &ptrT1);
                // printf("--T0: ");printHex2(ptrT0);printf("\n");
                // printf("--T1: ");printHex2(ptrT1);printf("\n");
            }
        }
        left_shift_word(&ptrT1, 1);
        
        // printf("\nResult:-------------------------------------------------\n");
        // printf("--T0: ");printHex2(ptrT0);printf("\n");
        // printf("--T1: ");printHex2(ptrT1);printf("\n\n");

        if (ptrT1->wordlen > ptrT0->wordlen)
            add_core_xyz(&ptrT1, &ptrT0, &ptrT);
        else
            add_core_xyz(&ptrT0, &ptrT1, &ptrT);

        // printf("\n\nprint(int(hex(");
        // printHex2(ptrT1);printf(" + ");printHex2(ptrT0);
        // printf("), 16) == int(\"");
        // printHex2(ptrT);printf("\", 16))\n\n");
        
        left_shift_word(&ptrT, j);
        // printHex2(ptrT);

        // printf("\n\nprint(int(hex(");
        // printHex2(ptrT);printf(" + ");printHex2(*pptrZ);
        // printf("), 16) == int(\"");
        if (ptrZ->wordlen > ptrT->wordlen) // wordlen Z >= wordlen T
            add_core_xyz(pptrZ, &ptrT, pptrZ);
        else 
            add_core_xyz(&ptrT, pptrZ, pptrZ);
        // printHex2(*pptrZ);printf("\", 16))\n\n");

    }
    // Cleanup
    delete_bint(&ptrT);
    delete_bint(&ptrT0);
    delete_bint(&ptrT1);
    delete_bint(&ptrTmp0);
    delete_bint(&ptrTmp1);
}
void MUL_Core_ImpTxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "MUL_Core_ImpTxtBk");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "MUL_Core_ImpTxtBk");

    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;
    
    int n = ptrX->wordlen;
    int m = ptrX->wordlen;

    matchSize(ptrX, ptrY);
    makeEven(ptrX); makeEven(ptrY);

    delete_bint(pptrZ);
    *pptrZ = init_bint(pptrZ, n+m);
    if (!*pptrZ) {
        fprintf(stderr, "Error: Memory allocation failed in 'mul_core_TxtBk_xyz'\n");
        exit(1);
    }
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_ImpTxtBk");
    BINT* ptrZ = *pptrZ;

    int p = ptrX->wordlen / 2;
    int q = ptrY->wordlen / 2;

    BINT* ptrT = init_bint(&ptrT, n+m);
    BINT* ptrT0 = init_bint(&ptrT0, 2*p);
    BINT* ptrT1 = init_bint(&ptrT1, 2*p);
    BINT* ptrTmp0 = init_bint(&ptrTmp0, 2*p);
    BINT* ptrTmp1 = init_bint(&ptrTmp1, 2*p);

    for(int j = 0; j < 2 * q; j++) {
        for(int k = 0; k < p; k++) {
            reset_bint(ptrTmp0);
            reset_bint(ptrTmp1);
            mul_xyz(ptrX->val[2*k], ptrY->val[j], &ptrTmp0);
            mul_xyz(ptrX->val[2*k+1], ptrY->val[j], &ptrTmp1);
            if (!k) {
                copy_BINT(&ptrT0, &ptrTmp0);
                copy_BINT(&ptrT1, &ptrTmp1);
            } else {
                left_shift_word(&ptrTmp0, 2*k);
                refine_BINT_word(ptrTmp0, 2*k);
                OR_BINT(ptrTmp0, ptrT0, &ptrT0);

                left_shift_word(&ptrTmp1, 2*k);
                refine_BINT_word(ptrTmp1, 2*k);
                OR_BINT(ptrTmp1, ptrT1, &ptrT1);
            }
        }
        left_shift_word(&ptrT1, 1);
        if (ptrT1->wordlen > ptrT0->wordlen)
            add_core_xyz(&ptrT1, &ptrT0, &ptrT);
        else
            add_core_xyz(&ptrT0, &ptrT1, &ptrT);
        left_shift_word(&ptrT, j);
        if (ptrZ->wordlen > ptrT->wordlen) // wordlen Z >= wordlen T
            add_core_xyz(pptrZ, &ptrT, pptrZ);
        else 
            add_core_xyz(&ptrT, pptrZ, pptrZ);
    }
    delete_bint(&ptrT);
    delete_bint(&ptrT0);
    delete_bint(&ptrT1);
    delete_bint(&ptrTmp0);
    delete_bint(&ptrTmp1);
}

void mul_core_Krtsb_test(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "MUL_Core_ImpTxtBk");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "MUL_Core_ImpTxtBk");

    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;
    
    int n = ptrX->wordlen;
    int m = ptrX->wordlen;

    matchSize(ptrX, ptrY);
    makeEven(ptrX); makeEven(ptrY);

    delete_bint(pptrZ);
    *pptrZ = init_bint(pptrZ, n+m);
    if (!*pptrZ) {
        fprintf(stderr, "Error: Memory allocation failed in 'mul_core_TxtBk_xyz'\n");
        exit(1);
    }
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_ImpTxtBk");
    BINT* ptrZ = *pptrZ;
}