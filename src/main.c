#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "arithmetic.h"

// Define Macros for Bit Lengths based on 32-bit word units
#define BIT_1024 0x20  // 32 * 32 = 1024 bits
#define BIT_2048 0x40  // 64 * 32 = 2048 bits
#define BIT_3072 0x60  // 96 * 32 = 3072 bits
#define BIT_7680 0xF0  // 240 * 32 = 7680 bits

// Configuration Macros
#define TEST_ITERATIONS 10000
#define MIN_BIT_LENGTH 32 // 1024-bit
#define MAX_BIT_LENGTH 64 // 2048-bit

#define SET_BIT_LENGTHS(bit_op, rnd, fix) \
    do { \
        switch (bit_op) { \
            case 1: fix = BIT_2048; rnd = 0; break;       /* Fixed 2048 bits */ \
            case 2: fix = BIT_3072; rnd = 0; break;       /* Fixed 3072 bits */ \
            case 3: fix = BIT_7680; rnd = 0; break;       /* Fixed 7680 bits */ \
            case 4: rnd = BIT_1024; fix = BIT_1024; break; /* Range: 1024 ~ 2048 bits */ \
            case 5: rnd = BIT_2048; fix = BIT_2048; break; /* Range: 2048 ~ 4096 bits */ \
            case 6: rnd = BIT_2048; fix = BIT_3072; break; /* Range: 3072 ~ 5120 bits */ \
            default: fix = BIT_1024; rnd = 0;              /* Default to fixed 1024 bits */ \
        } \
    } while(0)

// Macro for BINT generation
#define GENERATE_BINTS(ptrX, ptrY, rnd, fix) \
    do { \
        int lenX, lenY; \
        if (rnd) { \
            lenX = (rand() % rnd) + fix; \
            lenY = (rand() % rnd) + fix; \
        } else { \
            lenX = lenY = fix; \
        } \
        int sgnX = rand() % 2; \
        int sgnY = rand() % 2; \
        RANDOM_BINT(&ptrX, sgnX, lenX); \
        RANDOM_BINT(&ptrY, sgnY, lenY); \
    } while(0)

// Macro for positive BINT generation
#define GENERATE_POSITIVE_BINTS(ptrX, ptrY, rnd, fix) \
    do { \
        int lenX, lenY; \
        if (rnd) { \
            lenX = (rand() % rnd) + fix; \
            lenY = (rand() % rnd) + fix; \
        } else { \
            lenX = lenY = fix; \
        } \
        RANDOM_BINT(&ptrX, false, lenX); \
        RANDOM_BINT(&ptrY, false, lenY); \
    } while(0)

#define PERFORM_OPERATION_AND_PRINT(OP, OP_SYMBOL, ptrX, ptrY, ptrZ) \
    do { \
        OP(&ptrX, &ptrY, &ptrZ); \
        printf("print("); \
        print_bint_hex_py(ptrX); \
        printf("%s", OP_SYMBOL); \
        print_bint_hex_py(ptrY); \
        printf(" == "); \
        print_bint_hex_py(ptrZ); \
        printf(")\n"); \
        delete_bint(&ptrX); \
        delete_bint(&ptrY); \
        delete_bint(&ptrZ); \
    } while(0)

void test_rand_OP(int cnt, int bit_op, void (*op_func)(BINT**, BINT**, BINT**), const char* op_symbol, int positive) {
    int idx = 0x00;
    while (idx < cnt) {
        BINT *ptrX = NULL, *ptrY = NULL, *ptrZ = NULL;
        int rnd, fix;

        SET_BIT_LENGTHS(bit_op, rnd, fix);
        if (positive) {
            GENERATE_POSITIVE_BINTS(ptrX, ptrY, rnd, fix);
        } else {
            GENERATE_BINTS(ptrX, ptrY, rnd, fix);
        }
        PERFORM_OPERATION_AND_PRINT(op_func, op_symbol, ptrX, ptrY, ptrZ);
        idx++;
    }
}
void test_rand_ADD(int cnt, int bit_op, int sgn_op) {
    test_rand_OP(cnt, bit_op, ADD, "+", sgn_op);
}
void test_rand_SUB(int cnt, int bit_op, int sgn_op) {
    test_rand_OP(cnt, bit_op, SUB, "-", sgn_op);
}
void test_rand_MUL(int cnt, int bit_op, int sgn_op, int mul_op) {
    void (*mul_funcs[])(BINT**, BINT**, BINT**) = {
        mul_core_TxtBk_xyz,           // Default multiplication function
        MUL_Core_ImpTxtBk_xyz,        // Multiplication function for mul_op == 1
        MUL_Core_Krtsb_xyz            // Multiplication function for mul_op == 2
    };

    // Ensure mul_op is within the valid range of the array
    int func_index = (mul_op >= 1 && mul_op <= 2) ? mul_op : 0;

    // Call the test function with the selected multiplication operation
    test_rand_OP(cnt, bit_op, mul_funcs[func_index], "*", sgn_op);
}

void test_rand_DIV(int cnt, int bit_op, int sgn_op) {
    srand((unsigned int)time(NULL));
    int idx = 0x00;
    while (idx < cnt) {
        BINT *ptrX = NULL, *ptrY = NULL;
        BINT* ptrQ = NULL;
        BINT* ptrR = NULL;
        int rnd, fix;

        int len1 = 0x10;
        int len2 = 0x0f;
        
        RANDOM_BINT(&ptrX, false, len1);
        RANDOM_BINT(&ptrY, false, len2);

        DIV_Binary_Long(&ptrX, &ptrY, &ptrQ, &ptrR);
        
        printf("print("); print_bint_hex_py(ptrQ);
        printf(" * "); print_bint_hex_py(ptrY);
        printf(" + "); print_bint_hex_py(ptrR);
        printf(" == "); print_bint_hex_py(ptrX);
        printf(")\n");

        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrQ);
        delete_bint(&ptrR);
        idx++;
    }
}

void test_rand_EEA(int cnt, int bit_op, int sgn_op) {
    // srand((unsigned int)time(NULL));
    // int idx = 0x00;
    // while (idx < cnt) {
    //     BINT* ptrX = NULL; BINT* ptrY = NULL;
    //     BINT* ptrS = NULL; BINT* ptrT = NULL;  
    //     BINT* ptrR = NULL;
    //     int rnd, fix;

    //     SET_BIT_LENGTHS(bit_op, rnd, fix);
    //     GENERATE_POSITIVE_BINTS(ptrX, ptrY, rnd, fix);

    //     EEA(&ptrX, &ptrY, &ptrS, &ptrT, &ptrR);
        
    //     printf("print(");
    //     print_bint_hex_py(ptrX);
    //     printf(" * ");
    //     print_bint_hex_py(ptrS);
    //     printf("+");
    //     print_bint_hex_py(ptrY);
    //     printf(" * ");
    //     print_bint_hex_py(ptrT);
    //     printf(" == ");
    //     print_bint_hex_py(ptrR);
    //     printf(")\n");

    //     delete_bint(&ptrX);
    //     delete_bint(&ptrY);
    //     delete_bint(&ptrT);
    //     delete_bint(&ptrS);
    //     delete_bint(&ptrR);
    //     idx++;
    // }
}

int main() {
    /**
     * bit_op
     * default(0): Fixed 1024 bits
     * 1: Fixed 2048 bits
     * 2: Fixed 3072 bits
     * 3: Fixed 7680 bits
     * 4: 1024 ~ 2048 bits
     * 5: 2048 ~ 4096 bits
     * 6: 3072 ~ 5120 bits
     * ===============================================================
     * sgn_op
     * 0: random sign
     * 1: positive
    */
    int bit_op = 4;
    int sgn_op = 1;

    // Addition and Subtraction
    // test_rand_ADD(TEST_ITERATIONS, bit_op, sgn_op);
    // test_rand_SUB(TEST_ITERATIONS, bit_op, sgn_op);

    // Multiplication
    // test_rand_MUL(TEST_ITERATIONS, bit_op, sgn_op, 0); // TextBook
    // test_rand_MUL(TEST_ITERATIONS, bit_op, sgn_op, 1); // Improved TextBook
    // test_rand_MUL(TEST_ITERATIONS, bit_op, sgn_op, 2); // Karatsuba

    // test_rand_DIV(TEST_ITERATIONS, bit_op, sgn_op);
    
    srand((unsigned int)time(NULL));
    int t = 10;

    int idx = 0;
    while(idx < t) {
        BINT* ptrX = NULL;
        BINT* ptrY = NULL;
        BINT* ptrT = NULL;  
        BINT* ptrS = NULL;
        BINT* ptrR = NULL;

        int len1 = (rand() % 0x020) + 0x020; // 1024 ~ 2048 bits
        int len2 = (rand() % 0x020) + 0x020; // 1024 ~ 2048 bits

        RANDOM_BINT(&ptrX, false, len1);
        RANDOM_BINT(&ptrY, false, len2);

        EEA(&ptrX, &ptrY, &ptrS, &ptrT, &ptrR);
        printf("print(hex(");
        print_bint_hex_py(ptrX);
        printf(" * ");
        print_bint_hex_py(ptrS);
        printf("+");
        print_bint_hex_py(ptrY);
        printf(" * ");
        print_bint_hex_py(ptrT);
        printf(") == hex(");
        print_bint_hex_py(ptrR);
        printf("))\n");

        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrT);
        delete_bint(&ptrS);
        delete_bint(&ptrR);
        idx++;
    }


//     srand((unsigned int)time(NULL));

//     int idx = 0;
//     while(idx < TEST_ITERATIONS) {
//         BINT* ptrX = NULL;
//         BINT* ptrY = NULL;
//         BINT* ptrZ = NULL;

//         // BINT* ptrQ = NULL;
//         // BINT* ptrR = NULL;
// /*************************** Random Input **************************************/
//         // int len1 = (rand() % 0x010) + 0x010; //  512 ~ 1024 bits
//         // int len2 = (rand() % 0x010) + 0x010; //  512 ~ 1024 bits
//         // int len1 = (rand() % 0x020) + 0x020; // 1024 ~ 2048 bits
//         // int len2 = (rand() % 0x020) + 0x020; // 1024 ~ 2048 bits
//         // int len1 = (rand() % 0x020) + 0x040; // 2048 ~ 3072 bits
//         // int len2 = (rand() % 0x020) + 0x040; // 2048 ~ 3072 bits
//         // int len1 = (rand() % 0x090) + 0x060; // 3072 ~ 7680 bits
//         // int len2 = (rand() % 0x090) + 0x060; // 3072 ~ 7680 bits
//         // int len1 = (rand() % 0xf0) + 0x0f0; // 7680 ~ 15360 bits
//         // int len2 = (rand() % 0xf0) + 0x0f0; // 7680 ~ 15360 bits
        
//         // int len1 = (rand() % 0x05) + 0x05;
//         // int len2 = (rand() % 0x05) + 0x05;        

//         int len1 = 0x20;
//         int len2 = 0x20;

//         int sgn1 = rand() % 0x02;
//         int sgn2 = rand() % 0x02;
//         RANDOM_BINT(&ptrX, sgn1, len1);
//         RANDOM_BINT(&ptrY, sgn2, len2);
//         // RANDOM_BINT(&ptrX, false, len1);
//         // RANDOM_BINT(&ptrY, false, len2);
     
//         // MUL_Core_Krtsb_xyz(&ptrX,&ptrY,&ptrZ);
//         // SQU_TxtBk_xz(&ptrX,&ptrZ);
//         SQU_Krtsb_xz(&ptrX,&ptrZ);

//         printf("print(");
//         print_bint_hex_py(ptrX);
//         printf(" * ");
//         print_bint_hex_py(ptrX);
//         printf(" == ");
//         print_bint_hex_py(ptrZ);
//         printf(")\n");

//         delete_bint(&ptrX);
//         delete_bint(&ptrY);
//         delete_bint(&ptrZ);
//         idx++;
//     }

    return 0;
}

// Timing Macro
// #define MEASURE_TIME(start, end) ((double)(end - start) / CLOCKS_PER_SEC)

// void performTest(void (*testFunc)(BINT**, BINT**, BINT**), BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
//     clock_t start = clock();
//     testFunc(pptrX, pptrY, pptrZ);
//     clock_t end = clock();
//     printf("%.6f\n", MEASURE_TIME(start, end));
// }

// void performBINT(void (*testFunc1)(BINT**, BINT**, BINT**), void (*testFunc2)(BINT**, BINT**, BINT**)) {
//     srand((unsigned int)time(NULL));

//     for (int idx = 0; idx < TEST_ITERATIONS; idx++) {
//         int len1 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;
//         int len2 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;

//         BINT *ptrX = NULL, *ptrY = NULL, *ptrZ = NULL;
//         BINT* ptrTmpX = NULL;
//         BINT* ptrTmpY = NULL;
//         BINT* ptrTmpZ = NULL;
//         RANDOM_BINT(&ptrX, 0, len1);
//         RANDOM_BINT(&ptrY, 0, len2);
//         copyBINT(&ptrTmpX, &ptrX);      
//         copyBINT(&ptrTmpY, &ptrY);

//         performTest(testFunc1, &ptrX, &ptrY, &ptrZ);
//         performTest(testFunc2, &ptrTmpX, &ptrTmpY, &ptrTmpZ);

//         delete_bint(&ptrX);
//         delete_bint(&ptrY);
//         delete_bint(&ptrZ);
//         delete_bint(&ptrTmpX);
//         delete_bint(&ptrTmpY);
//         delete_bint(&ptrTmpZ);
//     }
// }