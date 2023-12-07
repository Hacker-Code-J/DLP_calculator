/**
 * @mainpage Big Integer Library Documentation
 *
 * @section intro Introduction
 * The Big Integer Library provides an implementation for handling very large integers 
 * that are beyond the capacity of standard C integer types. This library is useful in 
 * scenarios such as cryptographic computations, high precision calculations, and 
 * anywhere standard integer types are insufficient due to their size limitations.
 *
 * This documentation covers the usage, functionalities, and implementation details of 
 * the Big Integer Library. It is intended for developers who need to integrate large 
 * integer operations into their applications.
 *
 * @section usage Usage
 * To use this library, include 'bigint.h' in your C source files. The library 
 * provides a struct `BINT` to represent large integers and a set of functions 
 * for operations on these integers.
 *
 * Example of initializing a big integer:
 * @code
 * BINT myBigInt;
 * bigIntInit(&myBigInt, 0); // Initialize the big integer to zero
 * @endcode
 *
 * Example of arithmetic operation:
 * @code
 * BINT a, b, sum;
 * bigIntInit(&a, 123456789);
 * bigIntInit(&b, 987654321);
 * bigIntAdd(&a, &b, &sum); // Perform addition: sum = a + b
 * @endcode
 *
 * @section notes Implementation Notes
 * - The library uses dynamic memory allocation for managing the array of words in `BINT`.
 * - Care should be taken to free the allocated memory using the appropriate function 
 *   provided by the library.
 * - The library handles both positive and negative numbers, represented by the `sign` 
 *   field in the `BINT` struct.
 * - Error handling is implemented using specific return codes from functions.
 *
 * @section development Development
 * The source code and development resources for the Big Integer Library can be found on GitHub:
 * [Big Integer Library on GitHub](https://github.com/yourusername/bigintlibrary)
 *
 * @author Your Name
 * @date 2023-12-08
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "measure.h"

#define SET_BIT_LENGTHS(bit_op, rnd, fix) \
    do { \
        switch (bit_op) { \
            case 1: fix = u32_BIT_2048; rnd = 0; break;       /* Fixed 2048 bits */ \
            case 2: fix = u32_BIT_3072; rnd = 0; break;       /* Fixed 3072 bits */ \
            case 3: fix = u32_BIT_7680; rnd = 0; break;       /* Fixed 7680 bits */ \
            case 4: rnd = u32_BIT_1024; fix = u32_BIT_1024; break; /* Range: 1024 ~ 2048 bits */ \
            case 5: rnd = u32_BIT_2048; fix = u32_BIT_2048; break; /* Range: 2048 ~ 4096 bits */ \
            case 6: rnd = u32_BIT_2048; fix = u32_BIT_3072; break; /* Range: 3072 ~ 5120 bits */ \
            default: fix = u32_BIT_1024; rnd = 0;              /* Default to fixed 1024 bits */ \
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

void test_rand_DIV(int cnt) {
    srand((unsigned int)time(NULL));
    int idx = 0x00;
    while (idx < cnt) {
        BINT *ptrX = NULL, *ptrY = NULL;
        BINT* ptrQ = NULL;
        BINT* ptrR = NULL;
        int len1 = (rand() % 0x10) + 0x10;
        int len2 = len1-1;
        
        RANDOM_BINT(&ptrX, false, len1);
        RANDOM_BINT(&ptrY, false, len2);

        // DIV_Binary_Long(&ptrX, &ptrY, &ptrQ, &ptrR);
        DIV_Long(&ptrX, &ptrY, &ptrQ, &ptrR);

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

void test_rand_EXP_MOD(int cnt) {
    srand((unsigned int)time(NULL));
    int idx = 0x00;
    while (idx < cnt) {
        BINT* ptrX = NULL;
        BINT* ptrY = NULL;
        BINT* ptrZ = NULL;
        BINT* ptrMod = NULL;

        int len1 = (rand() % 0x04) + 0x01;
        int len2 = (rand() % 0x04) + 0x01;
        int len3 = (rand() % 0x08) + 0x01;

        RANDOM_BINT(&ptrX, false, len1);
        RANDOM_BINT(&ptrY, false, len2);
        RANDOM_BINT(&ptrMod, false, len3);

        EXP_MOD_Montgomery(&ptrX,&ptrY,&ptrZ, ptrMod);
        // EXP_MOD_L2R(&ptrX,&ptrY,&ptrZ, ptrMod);
        // EXP_MOD_R2L(&ptrX,&ptrY,&ptrZ, ptrMod);
  
        printf("print(pow(");
        print_bint_hex_py(ptrX);
        printf(", ");
        print_bint_hex_py(ptrY);
        printf(", ");
        print_bint_hex_py(ptrMod);
        printf(") == ");
        print_bint_hex_py(ptrZ);
        printf(")\n");

        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrZ);
        delete_bint(&ptrMod);
        idx++;
    }
}

// #define MEASURE_TIME(start, end) ((double)(end - start) / CLOCKS_PER_SEC)

// void performBINT(void (*testFunc)(BINT**, BINT**, BINT**, BINT**), BINT** pptrX, BINT** pptrY, BINT** pptrQ, BINT** pptrR) {
//     clock_t start = clock();
//     testFunc(pptrX, pptrY, pptrQ, pptrR);
//     clock_t end = clock();
//     printf("%.6f\n", MEASURE_TIME(start, end));
// }

// void performTEST(void (*testFunc1)(BINT**, BINT**, BINT**, BINT**), void (*testFunc2)(BINT**, BINT**, BINT**, BINT**)) {
//     srand((unsigned int)time(NULL));

//     for (int idx = 0; idx < 500; idx++) {
//         int len1 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;
//         int len2 = len1 - 1;
//         // int len2 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;

//         BINT *ptrX = NULL, *ptrY = NULL;
//         BINT *ptrQ = NULL, *ptrR = NULL;
//         BINT *ptrTmpX = NULL, *ptrTmpY = NULL;
//         BINT *ptrTmpQ = NULL, *ptrTmpR = NULL;
//         RANDOM_BINT(&ptrX, 0, len1);
//         RANDOM_BINT(&ptrY, 0, len2);
//         copyBINT(&ptrTmpX, &ptrX);      
//         copyBINT(&ptrTmpY, &ptrY);

//         performBINT(testFunc1, &ptrX, &ptrY, &ptrQ, &ptrR);
//         performBINT(testFunc2, &ptrTmpX, &ptrTmpY, &ptrTmpQ, &ptrTmpR);

//         delete_bint(&ptrX);
//         delete_bint(&ptrY);
//         delete_bint(&ptrQ);
//         delete_bint(&ptrR);
//         delete_bint(&ptrTmpX);
//         delete_bint(&ptrTmpY);
//         delete_bint(&ptrTmpQ);
//         delete_bint(&ptrTmpR);
//     }
// }

int main() {
    // correctTEST_ADD(TEST_ITERATIONS);
    // correctTEST_SUB(TEST_ITERATIONS);

    // correctTEST_TxtBk(1000);
    // correctTEST_ImpTxtBk(1000);
    // correctTEST_Krtsb(1);
    
    // corretTEST_BinDIV(TEST_ITERATIONS);
    corretTEST_GenDIV(TEST_ITERATIONS);

    // corretTEST_MOD_L2R(TEST_ITERATIONS);
    // corretTEST_MOD_R2L(TEST_ITERATIONS);
    // corretTEST_MOD_Montgomery(TEST_ITERATIONS);

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
    // int bit_op = 4;
    // int sgn_op = 1;

    // Addition and Subtraction
    // test_rand_ADD(TEST_ITERATIONS, bit_op, sgn_op);
    // test_rand_SUB(TEST_ITERATIONS, bit_op, sgn_op);

    // Multiplication
    // test_rand_MUL(TEST_ITERATIONS, bit_op, sgn_op, 0); // TextBook
    // test_rand_MUL(100, bit_op, sgn_op, 1); // Improved TextBook
    // test_rand_MUL(TEST_ITERATIONS, bit_op, sgn_op, 2); // Karatsuba

    // test_rand_DIV(1000);
    // performTEST_2ArgFn(SQU_TxtBk_xz, SQU_Krtsb_xz);
    // performTEST_3ArgFn(MUL_Core_ImpTxtBk_xyz, mul_core_TxtBk_xyz);
    // performTEST_MUL();

    // test_rand_EXP_MOD(1000);
    // performBINT(DIV_Binary_Long, DIV_Long);
    // performTEST_4ArgFn(DIV_Binary_Long, DIV_Long);
    // performTEST_DIV(10);
    // test_rand_BarrettRed();
    // performFastRed();

    // srand((unsigned int)time(NULL));
    // int t = 1000;

    // int idx = 0;
    // while(idx < t) {
    //     BINT* ptrX = NULL;
    //     BINT* ptrY = NULL;
    //     BINT* ptrT = NULL;  
    //     BINT* ptrS = NULL;
    //     BINT* ptrR = NULL;

    //     int len1 = (rand() % 0x0f) + 0x01; // 1024 ~ 2048 bits
    //     int len2 = (rand() % 0x0f) + 0x01; // 1024 ~ 2048 bits

    //     // int sgn1 = rand() % 0x02;
    //     // int sgn2 = rand() % 0x02;
    //     // RANDOM_BINT(&ptrX, sgn1, len1);
    //     // RANDOM_BINT(&ptrY, sgn2, len2);
    //     RANDOM_BINT(&ptrX, false, len1);
    //     RANDOM_BINT(&ptrY, false, len2);

    //     EEA(&ptrX, &ptrY, &ptrS, &ptrT, &ptrR);

    //     printf("print(");
    //     print_bint_hex_py(ptrX);
    //     printf(" * ");
    //     print_bint_hex_py(ptrS);
    //     printf(" + ");
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

//         // int sgn1 = rand() % 0x02;
//         // int sgn2 = rand() % 0x02;
//         // RANDOM_BINT(&ptrX, sgn1, len1);
//         // RANDOM_BINT(&ptrY, sgn2, len2);
//         RANDOM_BINT(&ptrX, false, len1);
//         RANDOM_BINT(&ptrY, false, len2);
     
//         // MUL_Core_ImpTxtBk_xyz(&ptrX,&ptrY,&ptrZ);
//         MUL_Core_Krtsb_xyz(&ptrX,&ptrY,&ptrZ);
//         // SQU_TxtBk_xz(&ptrX,&ptrZ);
//         // SQU_Krtsb_xz(&ptrX,&ptrZ);

//         printf("print(");
//         print_bint_hex_py(ptrX);
//         printf(" * ");
//         print_bint_hex_py(ptrY);
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