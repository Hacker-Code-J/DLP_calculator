#include "setup.h"
#include "BigInteger.h"

#include <stdio.h>
#include <stdlib.h>

// WORD arrOne[1] = {0x01};

// const BINT BINT_ZERO = {false, 0, NULL};
// const BINT BINT_ONE = {false, 1, arrOne};
// const BINT BINT_NEG_ONE = {true, 1, arrOne};

BINT* init_bint(BINT** pptrBint, int wordlen) { // ptrBint = *pptrBint
    // Allocate memory for BINT structure
    *pptrBint = (BINT*)malloc(sizeof(BINT));
    if(!(*pptrBint)) {
        fprintf(stderr, "Error: Unable to allocate memory for BINT.\n");
        exit(1);
    }
    // Allocate memory for val (array of WORD)
    (*pptrBint)->val = (WORD*)calloc(wordlen,sizeof(WORD));
    if (!(*pptrBint)->val) {
        free(*pptrBint); // freeing the already allocated BINT memory before exiting
        fprintf(stderr, "Error: Unable to allocate memory for BINT val.\n");
        exit(1);
    }
    // Initialize structure members
    (*pptrBint)->sign = false;
    (*pptrBint)->wordlen = wordlen;
    return *pptrBint;
}

void delete_bint(BINT** pptrBint) { // ptrBint = *pptrBint
    if (pptrBint && *pptrBint) {
        free((*pptrBint)->val); // Free the allocated memory for val
        free(*pptrBint); // Free the allocated memory for BINT structure
        *pptrBint = NULL; // Set the pointer to NULL after freeing memory to avoid using a dangling pointer
    }
}

#define SET_DATA(PB, V, WL, S) if(*(PB)) delete_bint(PB); *(PB)=calloc(1,sizeof(BINT));\
    (*PB)->val=calloc(1,sizeof(WORD)); *(*PB)->val=(V); (*PB)->wordlen=(WL); (*PB)->sign=(S)

void SET_BINT_ZERO(BINT** pptrBint) { SET_DATA(pptrBint, 0x00, 1, false); }
void SET_BINT_ONE(BINT** pptrBint) { SET_DATA(pptrBint, 0x01, 1, false); }

void copy_BINT(BINT** pptrBint_dst, BINT** pptrBint_src) {
    if(*pptrBint_dst != NULL)
        delete_bint(pptrBint_dst);

    *pptrBint_dst = (BINT*)calloc(1, sizeof(BINT));
    (*pptrBint_dst)->val = (WORD*)calloc((*pptrBint_src)->wordlen, sizeof(WORD));
    for(int i = 0; i < (*pptrBint_src)->wordlen; i++)
        (*pptrBint_dst)->val[i] = (*pptrBint_src)->val[i];
    
    (*pptrBint_dst)->wordlen = (*pptrBint_src)->wordlen;
    (*pptrBint_dst)->sign = (*pptrBint_src)->sign;
}


void makeEven(BINT** pptrBint) {
    // Check if wordlen is odd
    if ((*pptrBint)->wordlen % 2 == 1) {
        (*pptrBint)->wordlen++; // Increment wordlen to make it even

        // Reallocate memory for val
        (*pptrBint)->val = realloc((*pptrBint)->val, (*pptrBint)->wordlen * sizeof(WORD));
        if (!(*pptrBint)->val) {
            // Handle memory allocation failure, exit or return an error
            exit(1); 
        }

        // Fill the new WORD with 0
        (*pptrBint)->val[(*pptrBint)->wordlen - 1] = 0;
    }
}

/**
 * 
*/
// BINTQueue* createQueue() {
//     BINTQueue* q = (BINTQueue*)malloc(sizeof(BINTQueue));
//     if (!q) return NULL;  // Memory allocation failure

//     q->front = NULL;
//     q->rear = NULL;

//     return q;
// }

// void enqueue(BINTQueue* q, BINT data) {
//     if (!q) return;

//     Node* newNode = (Node*)malloc(sizeof(Node));
//     if (!newNode) return;  // Memory allocation failure

//     newNode->data = data;
//     newNode->next = NULL;

//     if (!q->rear) {
//         q->front = newNode;
//         q->rear = newNode;
//     } else {
//         q->rear->next = newNode;
//         q->rear = newNode;
//     }
// }

// BINT dequeue(BINTQueue* q) {
//     if (!q || !q->front) return BINT_ZERO;

//     Node* tempNode = q->front;
//     BINT data = tempNode->data;

//     q->front = q->front->next;

//     if (!q->front) {
//         q->rear = NULL;  // If the last node was dequeued, rear should also be NULL
//     }

//     free(tempNode);

//     return data;
// }

// BINT peek(BINTQueue* q) {
//     if (!q || !q->front) return BINT_ZERO;
//     return q->front->data;
// }

// bool isempty(BINTQueue* q) {
//     return (q == NULL || q->front == NULL);
// }

// void freeQueue(BINTQueue* q) {
//     while (!isempty(q)) {
//         dequeue(q);
//     }
//     free(q);
// }

/**
 * 
*/
bool store_bint(const char* filename, BINT* b) {
    if(!filename || !b) return false;

    FILE* f = fopen(filename, "wb");
    if(!f) return false;

    fwrite(&b->sign, sizeof(b->sign), 1, f);
    fwrite(&b->wordlen, sizeof(b->wordlen), 1, f);
    fwrite(b->val, sizeof(WORD), b->wordlen, f);

    fclose(f);
    return true;
}

bool multi_store_bints(const char* filename, BINT** bint_array, int num_bints) {
    if(!filename || !bint_array || num_bints <= 0) return false;

    FILE* f = fopen(filename, "wb");
    if(!f) return false;

    fwrite(&num_bints, sizeof(int), 1, f); // Write number of BINTs

    for (int i = 0; i < num_bints; i++) {
        BINT* b = bint_array[i];
        fwrite(&b->sign, sizeof(b->sign), 1, f);
        fwrite(&b->wordlen, sizeof(b->wordlen), 1, f);
        fwrite(b->val, sizeof(WORD), b->wordlen, f);
    }

    fclose(f);
    return true;
}

BINT* load_bint(const char* filename) {
    if(!filename) return NULL;

    FILE* f = fopen(filename, "rb");
    if(!f) return NULL;

    BINT* b = NULL;

    // Read sign and wordlen first
    bool tmp_sign;
    int tmp_wordlen;

    if (fread(&tmp_sign, sizeof(tmp_sign), 1, f) != 1 ||
        fread(&tmp_wordlen, sizeof(tmp_wordlen), 1, f) != 1) {
        fclose(f);
        return NULL;
    }

    // Now, use init_bint with the read wordlen
    init_bint(&b, tmp_wordlen);

    if (!b || !b->val) {
        fclose(f);
        return NULL;
    }

    // Assign the read values to the BINT structure
    b->sign = tmp_sign;
    b->wordlen = tmp_wordlen;

    // Read the val array
    if (fread(b->val, sizeof(WORD), b->wordlen, f) != (long unsigned int)b->wordlen) {
        delete_bint(&b);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return b;
}

BINT** multi_load_bints(const char* filename, int* num_bints) {
    if(!filename) return NULL;

    FILE* f = fopen(filename, "rb");
    if(!f) return NULL;

    int tmp_num_bints;
    if (fread(&tmp_num_bints, sizeof(int), 1, f) != 1) {
        fclose(f);
        return NULL;
    }

    BINT** bint_array = (BINT**)malloc(tmp_num_bints * sizeof(BINT*));
    if (!bint_array) {
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < tmp_num_bints; i++) {
        bool tmp_sign;
        int tmp_wordlen;

        if (fread(&tmp_sign, sizeof(tmp_sign), 1, f) != 1 ||
            fread(&tmp_wordlen, sizeof(tmp_wordlen), 1, f) != 1) {
            for (int j = 0; j < i; j++) {
                delete_bint(&bint_array[j]);
            }
            free(bint_array);
            fclose(f);
            return NULL;
        }

        BINT* b = NULL;
        init_bint(&b, tmp_wordlen);
        if (!b || !b->val) {
            for (int j = 0; j < i; j++) {
                delete_bint(&bint_array[j]);
            }
            free(bint_array);
            fclose(f);
            return NULL;
        }
        b->sign = tmp_sign;
        if (fread(b->val, sizeof(WORD), b->wordlen, f) != (long unsigned int)b->wordlen) {
            delete_bint(&b);
            for (int j = 0; j < i; j++) {
                delete_bint(&bint_array[j]);
            }
            free(bint_array);
            fclose(f);
            return NULL;
        }
        bint_array[i] = b;
    }

    fclose(f);
    *num_bints = tmp_num_bints; // update the number of loaded BINTs
    return bint_array;
}



/**
 * 
*/
// void printSage(BINTQueue* queue,  int opt, int loop) {
//     char* opr;

//     switch (opt)
//     {
//     case 0:
//         opr = "add";
//         break;
//     case 1:
//         opr = "sub";
//         break;
//     case 2:
//         opr = "mul";
//         break;
//     default:
//         opr = NULL;
//         break;
//     }
    
//     printf("def hex_to_int(hex_str, sign):\n");
//     printf("    val = Integer(hex_str.replace(' ', '').replace('0x', ''), 16)\n");
//     printf("    return -val if sign == \"[1]\" else val\n\n");

//     printf("def int_to_hex(val):\n");
//     printf("    sign = \"[0]\" if val >= 0 else \"[1]\"\n");
//     printf("    abs_val = abs(val)\n");
//     printf("    hex_str = hex(abs_val)[2:]\n");
//     printf("    hex_str = ' '.join([hex_str[i:i+8] for i in range(0, len(hex_str), 8)])\n");
//     printf("    return f\"{sign} 0x {hex_str}\"\n\n");

//     printf("def check_operation(x_sign, x_hex, y_sign, y_hex, expected_sign, expected_hex, operation):\n");
//     printf("    x_int = hex_to_int(x_hex, x_sign)\n");
//     printf("    y_int = hex_to_int(y_hex, y_sign)\n\n");

//     printf("    if operation == \"add\":\n");
//     printf("        result_int = x_int + y_int\n");
//     printf("    elif operation == \"sub\":\n");
//     printf("        result_int = x_int - y_int\n");
//     printf("    elif operation == \"mul\":\n");
//     printf("        result_int = x_int * y_int\n");
//     printf("    else:\n");
//     printf("        raise ValueError(f\"Invalid operation: {operation}\")\n\n");

//     printf("    result_hex = int_to_hex(result_int)\n");
//     printf("    return result_hex.replace(' ', '').lower() == (expected_sign + ' ' + expected_hex).replace(' ', '').lower()\n\n");

//     printf("values = [\n");
//     while(loop != 0) {
//         BINT X = dequeue(queue);
//         BINT Y = dequeue(queue);
//         BINT Z = dequeue(queue);
//         int n = MAX(X.wordlen, Y.wordlen);
//         printf("    (\"[%d]\", ", X.sign);
//         printf("\"0x");
//         for(int i=0; i<(n-X.wordlen); i++)
//             printf(" %08x", 0);
//         for (int i=X.wordlen-1; i>=0; i--)
//             printf(" %08x", X.val[i]);
//         printf("\", ");
//         printf("\"[%d]\", \"0x", Y.sign);
//         for(int i=0; i<(n-Y.wordlen); i++)
//             printf(" %08x", 0);
//         for (int i=Y.wordlen-1; i>=0; i--)
//             printf(" %08x", Y.val[i]);
//         printf("\", ");
//         printf("\"[%d]\", \"0x", Z.sign);
//         for(int i=0; i<n-Z.wordlen; i++)
//             printf(" %08x", 0);
//         for (int i=Z.wordlen-1; i>=0; i--)
//             printf(" %08x", Z.val[i]);
//         printf("\", \"%03s\"),\n", opr);
//         loop--;
//     }
//     printf("]\n\n");

//     printf("for idx, (x_sign, x, y_sign, y, expected_sign, expected, op) in enumerate(values):\n");
//     printf("    is_correct = check_operation(x_sign, x, y_sign, y, expected_sign, expected, op)\n");
//     printf("    print(is_correct)\n");
// }

/******************************************************************/
void printHex(BINT* X) {
    printf("[%d] 0x ",X->sign);
    for (int i=X->wordlen-1; i>=0; i--)
        printf("%08x ", X->val[i]);
    printf("\n");
}

void printHex2(BINT* X) {
    //printf("[%d] 0x ",X->sign);
    printf("0x");
    for (int i=X->wordlen-1; i>=0; i--) {

        printf("%08x", X->val[i]);
    }
    //printf("\n");
}

void custom_printHex(BINT* X, BINT* Y, BINT* Z,  int opt) {
    //option 0 = addition
    //option 1 = subtraction
    //option 2 = multiplication

    char opr;

    switch (opt)
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

// void Flip_sign(BINT* x){
//     if (x->sign == 0){
//         x->sign = -1;
//     }//양수면 음수로
//     if (x->sign == -1){
//         x->sign = 0;
//     }//음수면 양수로 부호 바꾸기
// }

/**
 * 
*/


// //Author: Kim Ye-chan
// void MUL_Shift(BINT* X, BINT* result , int N) {
//     int q = N / 32;
//     int r = N % 32;

//     if (r == 0) {
//         for (int i = X->wordlen-1; i>=0; i--) {
//             result->val[i+q] = X->val[i];
            
//         }
//         for(int i=0; i<q; i++){
//             result->val[i] = 0;
            
//         }
        
//     } 
//     else {
//         // r이 0이 아닌 경우 처리

//         for (int i = X->wordlen; i>=0; i--) {
//             if(i==X->wordlen){
//                 result->val[i] = (X->val[i-1] >> (32-r));                
//                 continue; 
//             }
//             else if(i < X->wordlen && i > 0){
//                 result->val[i] = (X->val[i-1] >> (32-r)) | (X->val[i] << r);
//                 continue;
//             }

//             else if(i == 0){
//                 result->val[i] = (X->val[i] << r);
                
//             }
//         }
//         if (q > 0) {
//             for (int i = result->wordlen-1; i>=0; i--) {
//                 result->val[i+q] = result->val[i];
//             }
//             for(int i=0; i<q; i++){
//                 result->val[i] = 0;
//             }
//         }         
//     }
//     for (int i = result->wordlen - 1 ; i >= 0  ; i--) {
//     printf("%08x ", result->val[i]);
//     }


// } 

// void DIV_Shift(BINT* X, BINT* result, int N) {
//     int q = N / 32;
//     int r = N % 32;

//     if (r == 0) {
//         for (int i = 0; i < X->wordlen - q; i++) {
//             result->val[i] = X->val[i + q];
//         }

//     } else {
//         for (int i = 0; i < X->wordlen; i++) {
//             if (i < X->wordlen - 1) {
//                 result->val[i] = (X->val[i + 1] << (32 - r)) | (X->val[i] >> r);
//             } else {
//                 result->val[i] = X->val[i] >> r;
//             }
//         }
//         for (int i = 0; i < result->wordlen - q; i++) {
//             result->val[i] = result->val[i + q];
//         }
//     }
//     for (int i = result->wordlen - 1 ; i >= 0  ; i--) {
//     printf("%08x ", result->val[i]);
//     }
// }

void shift_MUL(BINT** pptrBint, int N) {
    const int q = N / WORD_BITLEN;
    const int r = N % WORD_BITLEN;

    if(r == 0) {
#if WORD_BITLEN == 8
    (*pptrBint)->val = (WORD*)realloc((*pptrBint)->val, (*pptrBint)->wordlen + q);
#elif WORD_BITLEN == 64
    (*pptrBint)->val = (WORD*)realloc((*pptrBint)->val, 8 * ((*pptrBint)->wordlen + q));
#else
    (*pptrBint)->val = (WORD*)realloc((*pptrBint)->val, 4 * ((*pptrBint)->wordlen + q));
#endif    
        (*pptrBint)->wordlen += q;
        for (int i = ((*pptrBint)->wordlen) - q - 1; i >= 0; i--)
            (*pptrBint)->val[1+q] = (*pptrBint)->val[i];
        for (int i = 0; i < q; i++)
            (*pptrBint)->val[i] = 0x00;

    } else {
#if WORD_BITLEN == 8
    (*pptrBint)->val = (WORD*)realloc((*pptrBint)->val, (*pptrBint)->wordlen + q + 1);
#elif WORD_BITLEN == 64
    (*pptrBint)->val = (WORD*)realloc((*pptrBint)->val, 8 * ((*pptrBint)->wordlen + q + 1));
#else
    (*pptrBint)->val = (WORD*)realloc((*pptrBint)->val, 4 * ((*pptrBint)->wordlen + q + 1));
#endif
        (*pptrBint)->wordlen += (q + 1);
        for (int i = (*pptrBint)->wordlen-2; i > q-1; i--)
            (*pptrBint)->val[i] = (*pptrBint)->val[i-q];
        for (int i = 0; i < q; i++)
            (*pptrBint)->val[i] = 0x00;
        
        (*pptrBint)->val[(*pptrBint)->wordlen - 1] = (*pptrBint)->val[(*pptrBint)->wordlen - 2] >> (WORD_BITLEN - r);
        for(int i = (*pptrBint)->wordlen-2; i > q; i--)
            (*pptrBint)->val[i] = ((*pptrBint)->val[i] << q) | ((*pptrBint)->val[i-1] >> (WORD_BITLEN-r));
        (*pptrBint)->val[q] = (*pptrBint)->val[q] << r;
        
        if ((*pptrBint)->val[(*pptrBint)->wordlen -1] == 0x00) {
#if WORD_BITLEN == 8
    (*pptrBint)->val = (WORD*)realloc((*pptrBint)->val, (*pptrBint)->wordlen);
    (*pptrBint)->wordlen -= 1;
#elif WORD_BITLEN == 64
    (*pptrBint)->val = (WORD*)realloc((*pptrBint)->val, 8 * ((*pptrBint)->wordlen));
    (*pptrBint)->wordlen -= 1;
#else
    (*pptrBint)->val = (WORD*)realloc((*pptrBint)->val, 4 * ((*pptrBint)->wordlen ));
    (*pptrBint)->wordlen -= 1;
#endif
        }
    }
}
void shift_DIV(BINT** pptrBint, int shift) {

}
