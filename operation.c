#include <stdio.h>
#include <stdlib.h>

#include "setup.h"
#include "BigInteger.h"
#include "operation.h"

WORD One[1] = {0x01};

const BINT BINT_ZERO = {false, 0, NULL};
const BINT BINT_ONE = {false, 1, One};
const BINT BINT_NEG_ONE = {true, 1, One};

// AND operation
void AND_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ) {
    int min_len = ptrX->wordlen < ptrY->wordlen ? ptrX->wordlen : ptrY->wordlen;
    for (int i = 0; i < min_len; i++) {
        (*pptrZ)->val[i] = ptrX->val[i] & ptrY->val[i];
    }
    (*pptrZ)->wordlen = min_len; // The result size will be size of the smaller operand
    // ptrZ->sign = ptrX->sign && ptrY->sign; // Negative if both operands are negative
}

// OR operation
void OR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ) {
    int min_len = ptrX->wordlen < ptrY->wordlen ? ptrX->wordlen : ptrY->wordlen;
    for (int i = 0; i < min_len; i++) {
        (*pptrZ)->val[i] = ptrX->val[i] | ptrY->val[i];
    }
    (*pptrZ)->wordlen = min_len; // The result size will be size of the smaller operand
    // ptrZ->sign = ptrX->sign && ptrY->sign; // Negative if both operands are negative
}


void add_carry(WORD x, WORD y, WORD k, WORD* ptrQ, WORD* ptrR) {
	*ptrR = x + y;
	*ptrQ = (*ptrR < x);
	*ptrR += k;
	*ptrQ += (*ptrR < (x+y));
}

void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;
    BINT* ptrZ = *pptrZ;
    
    int n = ptrX->wordlen;
    int m = ptrY->wordlen;

    WORD k = 0;
    WORD res = 0;
    WORD carry = 0;

    // Loop until the shorter of the two numbers ends
    int i;
    for (i = 0; i < m; i++) {
        add_carry(ptrX->val[i], ptrY->val[i], k, &carry, &res);
        ptrZ->val[i] = res;
        k = carry;
    }
    // Continue adding any remaining X values with the carry, since Y is shorter
    for (; i < n; i++) {
        res = ptrX->val[i] + k;
        carry = (res < k);
        ptrZ->val[i] = res;
        k = carry;
    }

    if(k) {
        ptrZ->val[n] = k;
        ptrZ->wordlen = n+1;
    } else {
        ptrZ->wordlen = n;
    }

    refine_BINT(ptrZ);
    //return Z;
}

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

// void sub_xby(WORD x, WORD b, WORD y, WORD* res, WORD* borrow) {
//     /****************
//     *brrow = 0;
//     *res = x-borrow;
//     if(x < b)
//         *borrow = 1;
//     if(*res < y)
//         *borrow += 1;
//     *res = *res - y;
//     ****************/
//     //Optimize
//     *res = x-b;
//     *borrow = (x < b);
//     *borrow += (*res < y);
//     *res -= y;
    
//     //printf("%08x - %d - %08x = -%08x * W + %08x\n", x, b, y, *borrow, *res);
// }

// void sub_xyz(BINT* X, BINT* Y, BINT* Z) {//X>=Y>0
//     int n = X->wordlen;
//     int m = Y->wordlen;

//     WORD* tmp;
//     tmp = (WORD*)realloc(Y->val,n*sizeof(WORD));
//     Y->val = tmp;

//     for(int i=m; i<n; i++)
//         Y->val[i] = 0;

//     WORD b = 0;
//     WORD res, borrow;

//     //
//     for(int i=0; i<n; i++) {
//         sub_xby(X->val[i], b, Y->val[i], &res, &borrow);
//         Z->val[i] = res;
//     }
    
//     //refine_BINT(Z);
// }

// void SUB_xyz(BINT* X, BINT* Y, BINT* Z) {
//     if(X->sign==false && Y->sign==false) {
//         if(compare_xy(X,Y) != -1)
//             sub_xyz(X,Y,Z);
//         else {
//             Z->sign = true;
//             sub_xyz(Y,X,Z);
//         }
//     } else if(X->sign==false && Y->sign==true) {
//         if(compare_xy(X,Y) != -1)
//             add_xyz(X,Y,Z);
//         else
//             add_xyz(Y,X,Z);
//     } else if(X->sign==true && Y->sign==false) {
//         if(compare_xy(X,Y) != -1) {
//             Z->sign = true;
//             add_xyz(X,Y,Z);
//         } else {
//             Z->sign = true;
//             add_xyz(Y,X,Z);
//         }
//     } else {
//         if(compare_xy(X,Y) != -1){
//             Z->sign = true;
//             sub_xyz(X,Y,Z);
//         } else {
//             sub_xyz(Y,X,Z);
//         }
//     }
// }

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
void mul_core_TxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "mul_core_TxtBk_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "mul_core_TxtBk_xyz");

    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;

    if (!*pptrZ) {
        *pptrZ = init_bint(pptrZ, ptrX->wordlen + ptrY->wordlen);
        if (!*pptrZ) {
            fprintf(stderr, "Error: Memory allocation failed in 'mul_core_TxtBk_xyz'\n");
            exit(1);
        }
    }
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_core_TxtBk_xyz");


    if(ptrX->sign != ptrY->sign)
        (*pptrZ)->sign = true;

    BINT* ptrTmp = NULL;
    BINT* ptrWordMUl = NULL;

    for(int i = 0; i < ptrX->wordlen; i++) {
        for(int j = 0; j < ptrY->wordlen; j++) {
            mul_xyz(ptrX->val[i], ptrX->val[j], &ptrWordMUl);
            left_shift(&ptrWordMUl, (i+j) * WORD_BITLEN);
            add_core_xyz(pptrZ, &ptrWordMUl, &ptrTmp);
            // ADD(pptrZ, &ptrWordMUl, &ptrTmp);
            copy_BINT(pptrZ, &ptrTmp);
        }
    }
    delete_bint(&ptrTmp);
    delete_bint(&ptrWordMUl);
    refine_BINT(*pptrZ);
}

void MUL_Core_ImpTxtBk(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "MUL_Core_ImpTxtBk");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "MUL_Core_ImpTxtBk");

    BINT* ptrX = *pptrX;
    BINT* ptrY = *pptrY;
    BINT* ptrZ = *pptrZ;

    makeEven(ptrX); makeEven(ptrY);

    if (!*pptrZ) {
        *pptrZ = init_bint(pptrZ, ptrX->wordlen + ptrY->wordlen);
        if (!*pptrZ) {
            fprintf(stderr, "Error: Memory allocation failed in 'mul_core_TxtBk_xyz'\n");
            exit(1);
        }
    }
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_ImpTxtBk");


    int n = ptrX->wordlen;
    int m = ptrY->wordlen;
    int p = n/2;
    int q = m/2;

    BINT* ptrT0 = init_bint(&ptrT0, 2*p);
    BINT* ptrT1 = init_bint(&ptrT1, 2*p+1);
    BINT* ptrTmp0 = init_bint(&ptrTmp0, 2*p);
    BINT* ptrTmp1 = init_bint(&ptrTmp1, 2*p+1);

    for(int j = 0; j < 2 * q; j++) {
        for(int k = 0; k < p; k++) {
            mul_xyz(ptrX->val[2*k], ptrY->val[j], &ptrT0);
            printf("--T0:");printHex2(ptrT0);printf("\n");
            if (!k) {
                copy_BINT(&ptrTmp0, &ptrT0);
                printf("Tmp0:");printHex2(ptrTmp0);printf("\n");
            } else {
                left_shift_word(&ptrTmp0, 2);
                refine_BINT_word(ptrTmp0, 2);
                OR_BINT(ptrT0, ptrTmp0, &ptrTmp0);
                printf("Tmp0:");printHex2(ptrTmp0);printf("\n");
            }
            // printf("print(int(hex(");
            // printf("0x%08x", ptrX->val[2*k]);printf("*");printf("0x%08x", ptrY->val[j]);
            // printf("), 16) == int(\"");
            // printHex2(ptrT0);printf("\", 16))\n");
        }
    }
    // Cleanup
    delete_bint(&ptrT0);
    delete_bint(&ptrT1);
    delete_bint(&ptrTmp0);
    delete_bint(&ptrTmp1);
}


        // BINT* T0 = (BINT*)malloc(sizeof(BINT));
        // T0->wordlen = p;
        // T0->val = (WORD*)calloc(p, sizeof(WORD));
        
        // BINT* T1 = (BINT*)malloc(sizeof(BINT));
        // T1->wordlen = p;
        // T1->val = (WORD*)calloc(p, sizeof(WORD));
        
        // for(int k = 0; k < p; k++) {
        //     T0->val[k] = X->val[2*k] * Y->val[j];
        //     T1->val[k] = X->val[2*k + 1] * Y->val[j];
        // }

        // BINT* T = (BINT*)malloc(sizeof(BINT));
        // T->wordlen = 2 * p;
        // T->val = (WORD*)calloc(T->wordlen, sizeof(WORD));
        // add_core_xyz(&T1, &T0, &T);

        // if(j) shift_left_word(&T, j);

        // BINT* newZ = (BINT*)malloc(sizeof(BINT));
        // newZ->wordlen = n + m;
        // newZ->val = (WORD*)calloc(newZ->wordlen, sizeof(WORD));
        // add_core_xyz(pptrZ, &T, &newZ);

        // free(Z->val);
        // free(Z);
        
        // *pptrZ = newZ;
        // free(T->val);
        // free(T);
        // free(T0->val);
        // free(T0);
        // free(T1->val);
        // free(T1);