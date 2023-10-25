#include "setup.h"
#include "BigInteger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

WORD arrOne[1] = {1};

const BINT BINT_ZERO = {false, 0, NULL};
const BINT BINT_ONE = {false, 1, arrOne};
const BINT BINT_NEG_ONE = {true, 1, arrOne};

/**
 * 
*/
BINT* init_bint(BINT** bint_ptr, int wordlen) {
    // Allocate memory for BINT structure
    *bint_ptr = (BINT*)malloc(sizeof(BINT));
    if(!*bint_ptr) {
        fprintf(stderr, "Error: Unable to allocate memory for BINT.\n");
        exit(1);
    }
    // Allocate memory for val (array of WORD)
    (*bint_ptr)->val = (WORD*)calloc(wordlen,sizeof(WORD));
    if (!(*bint_ptr)->val) {
        free(*bint_ptr); // freeing the already allocated BINT memory before exiting
        fprintf(stderr, "Error: Unable to allocate memory for BINT val.\n");
        exit(1);
    }
    // Initialize structure members
    (*bint_ptr)->sign = false;
    (*bint_ptr)->wordlen = wordlen;
    return *bint_ptr;
}

/**
 * 
*/
void delete_bint(BINT** bint_ptr) {
    if (bint_ptr && *bint_ptr) {
        free((*bint_ptr)->val); // Free the allocated memory for val
        free(*bint_ptr); // Free the allocated memory for BINT structure
        *bint_ptr = NULL; // Set the pointer to NULL after freeing memory to avoid using a dangling pointer
    }
}

/******************************************************************/

void custom_printHex(BINT* X, BINT* Y, BINT* Z,  int option) {
    //option 0 = addition
    //option 1 = subtraction
    //option 2 = multiplication

    char opr;

    switch (option)
    {
    case 0:
        opr = '+';
        break;
    case 1:
        opr = '-';
        break;
    case 2:
        opr = '*';
        break;
    default:
        opr = '+';
        break;
    }

    int n = (X->wordlen >= Y->wordlen) ? X->wordlen:Y->wordlen;
    //print X 
    printf("  [%d] 0x ", X->sign);
    for(int i=0; i<(n-X->wordlen); i++)
        printf("%08x ", 0);
    for (int i=X->wordlen-1; i>=0; i--)
        printf("%08x ", X->val[i]);
    printf("\n");

    //print Y
    printf("%c [%d] 0x ", opr, Y->sign);
    for(int i=0; i<(n-Y->wordlen); i++)
        printf("%08x ", 0);
    for (int i=Y->wordlen-1; i>=0; i--)
        printf("%08x ", Y->val[i]);
    printf("\n");

    //print line
    printf("--------");
    for(int i=0; i<n; i++) {
        printf("-");
        printf("--------");
    } printf("\n");

    //print Z
    printf("  [%d] 0x ", Z->sign);
    for(int i=0; i<n-Z->wordlen; i++)
        printf("%08x ", 0);
    for (int i=Z->wordlen-1; i>=0; i--)
        printf("%08x ", Z->val[i]);
    printf("\n\n");
}

void custom_printHex_xy(BINT* X, BINT* Y, int n) {
    //print X 
    printf("x: [%d] 0x ",X->sign);
    for(int i=0; i<(n-X->wordlen); i++)
        printf("%08x ", 0);
    for (int i=X->wordlen-1; i>=0; i--)
        printf("%08x ", X->val[i]);
    printf("\n");

    //print Y
    printf("y: [%d] 0x ", Y->sign);
    for(int i=0; i<(n-Y->wordlen); i++)
        printf("%08x ", 0);
    for (int i=Y->wordlen-1; i>=0; i--)
        printf("%08x ", Y->val[i]);
    printf("\n\n");
}

void refine_BINT(BINT* X) {
    if(X == NULL) return;

    int new_wordlen = X->wordlen;
    while (new_wordlen > 1) { // at least one word needed
        if(X->val[new_wordlen-1] != 0)
            break;
        new_wordlen--;
    }
    if(X->wordlen != new_wordlen) {
        X->wordlen = new_wordlen;
        X->val = (WORD*)realloc(X->val, sizeof(WORD)*new_wordlen);
    }

    if((X->wordlen == 1) && (X->val[0] == 0))
        X->sign = false;
}

//Generate Random BINT

void rand_array(WORD* dst, int wordlen) {
    u8* p = (u8*)dst; //u8 = byte
    int cnt = wordlen * sizeof(WORD);
    while(cnt > 0) {
        *p = rand() & 0xFF;
        p++;
        cnt--;
    }
}

void rand_bint(BINT**x, int sign, int wordlen) {
    init_bint(x, wordlen);
    (*x)->sign = sign;
    rand_array((*x)->val, wordlen);

    refine_BINT(*x);
}

//Copy Arrary
void array_copy_x2y(WORD* X, WORD* Y, int wordlen) {
    for(int i=0; i<wordlen; i++)
        Y[i] = X[i];
}

//Assign Y<-X
void assgin_x2y(BINT* X, BINT** Y) {
    if(*Y)
        delete_bint(Y);
    init_bint(Y, X->wordlen);
    (*Y)->sign = X->sign;
    array_copy_x2y(X->val,(*Y)->val,X->wordlen);
}


//Compare
// X>Y return 1
// X<Y return -1
// X=Y return 0
int compare_xy(BINT* X, BINT* Y) {
    if (X->wordlen > Y->wordlen) return 1;
    if (X->wordlen < Y->wordlen) return -1;

    for (int i = X->wordlen - 1; i >= 0; i--) {
        if (X->val[i] > Y->val[i]) return 1;
        if (X->val[i] < Y->val[i]) return -1;
    }

    return 0;
}

//Author: Moon Ye-chan
int Get_bitlen(BINT* x){
    int bitlen =  (sizeof(x->val)*8);
    return bitlen;
}

int Get_sign(BINT* x){
    return x->sign;
}

void Flip_sign(BINT* x){
    if (x->sign == 0){
        x->sign = -1;
    }//양수면 음수로
    if (x->sign == -1){
        x->sign = 0;
    }//음수면 양수로 부호 바꾸기
}


//Author: Kim Ye-chan
void MUL_Shift(BINT* X, BINT* result , int N) {
    int q = N / 32;
    int r = N % 32;

    if (r == 0) {
        for (int i = X->wordlen-1; i>=0; i--) {
            result->val[i+q] = X->val[i];
            
        }
        for(int i=0; i<q; i++){
            result->val[i] = 0;
            
        }
        
    } 
    else {
        // r이 0이 아닌 경우 처리

        for (int i = X->wordlen; i>=0; i--) {
            if(i==X->wordlen){
                result->val[i] = (X->val[i-1] >> (32-r));                
                continue; 
            }
            else if(i < X->wordlen && i > 0){
                result->val[i] = (X->val[i-1] >> (32-r)) | (X->val[i] << r);
                continue;
            }

            else if(i == 0){
                result->val[i] = (X->val[i] << r);
                
            }
        }
        if (q > 0) {
            for (int i = result->wordlen-1; i>=0; i--) {
                result->val[i+q] = result->val[i];
            }
            for(int i=0; i<q; i++){
                result->val[i] = 0;
            }
        }         
    }
    for (int i = result->wordlen - 1 ; i >= 0  ; i--) {
    printf("%08x ", result->val[i]);
    }


} 

void DIV_Shift(BINT* X, BINT* result, int N) {
    int q = N / 32;
    int r = N % 32;

    if (r == 0) {
        for (int i = 0; i < X->wordlen - q; i++) {
            result->val[i] = X->val[i + q];
        }

    } else {
        for (int i = 0; i < X->wordlen; i++) {
            if (i < X->wordlen - 1) {
                result->val[i] = (X->val[i + 1] << (32 - r)) | (X->val[i] >> r);
            } else {
                result->val[i] = X->val[i] >> r;
            }
        }
        for (int i = 0; i < result->wordlen - q; i++) {
            result->val[i] = result->val[i + q];
        }
    }
    for (int i = result->wordlen - 1 ; i >= 0  ; i--) {
    printf("%08x ", result->val[i]);
    }
}