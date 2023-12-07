/***************************************************************************
 * Project Name: PUBAO (P.A.N.D.A.'s Unbounded Big Arithmetic Operations)
 * Team Name: P.A.N.D.A. (Programmers Aspiring to Navigate Digital Arithmetic)
 * 
 * File Description: This file contains the definitions and implementations
 *                   for large integer arithmetic.
 * 
 * Author(s): Ji Yong-Hyeon, Kim Ye-chan, Moon Ye-chan, Yoo Geun-oh
 * Date Created: 2023-09-21
 * Last Modified: 2023-11-22
 *
 * Note: This library aims to provide a robust and efficient solution for
 *       performing arithmetic on large integers beyond the limitations of
 *       standard data types.
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>

#include <flint/flint.h>
#include <flint/fmpz.h>

#include "setup.h"
#include "BigInteger.h"
#include "operation.h"

/**
 * if WORD_BITLEN = 32,
 * 0x010 -> ( 1 * 16 =  16) -> (16 *  32 =   512-bit)
 * 0x020 -> ( 2 * 16 =  32) -> (32 *  32 =  1024-bit)
 * 0x040 -> ( 4 * 16 =  64) -> (32 *  64 =  2048-bit)
 * 0x060 -> ( 6 * 16 =  96) -> (32 *  96 =  3072-bit)
 * 0x0f0 -> (15 * 16 = 240) -> (32 * 240 =  7680-bit)
 * 0x1e0 -> ( 1 * 256 + 14 * 16 = 480) -> (32 * 480 = 15360-bit)
*/

// Macro for bit length setting
#define SET_BIT_LENGTHS(bit_op, rnd, fix) \
    do { \
        switch (bit_op) { \
            case 1: rnd = 0x20; fix = 0x40; break; /* 2048 ~ 3072 bits */ \
            case 2: rnd = 0x90; fix = 0x60; break; /* 3072 ~ 7680 bits */ \
            case 3: rnd = 0xf0; fix = 0xf0; break; /* 7680 ~ 15360 bits */ \
            case 4: rnd = 0x00; fix = 0x020; break; /* 1024-bit */ \
            case 5: rnd = 0x00; fix = 0x040; break; /* 2048-bit */ \
            case 6: rnd = 0x00; fix = 0x060; break; /* 3072-bit */ \
            case 7: rnd = 0x00; fix = 0x0f0; break; /* 7680-bit */ \
            case 8: rnd = 0x00; fix = 0x1e0; break; /* 15360-bit */ \
            default: rnd = 0x20; fix = 0x20; /* 1024 ~ 2048 bits */ \
        } \
    } while(0)

// Macro for random BINT generation
#define RANDOMIZE_BINTS(ptrX, ptrY, rnd, fix) \
    do { \
        int len1 = (rand() % rnd) + fix; \
        int len2 = (rand() % rnd) + fix; \
        int sgnX = rand() % 0x02; \
        int sgnY = rand() % 0x02; \
        RANDOM_BINT(&ptrX, sgnX, len1); \
        RANDOM_BINT(&ptrY, sgnY, len2); \
    } while(0)

// Macro for positive random BINT generation
#define POSITIVE_RANDOMIZE_BINTS(ptrX, ptrY, rnd, fix) \
    do { \  
        int len1 = (rand() % rnd) + fix; \
        int len2 = (rand() % rnd) + fix; \
        RANDOM_BINT(&ptrX, false, len1); \
        RANDOM_BINT(&ptrY, false, len2); \
    } while(0)

// Macro for the operation and result printing
#define PERFORM_OPERATION_AND_PRINT(OP, OP_SYMBOL, ptrX, ptrY, ptrZ) \
    do { \
        OP(&ptrX, &ptrY, &ptrZ); \
        printf("print(hex("); \
        print_bint_hex_python(&ptrX); \
        printf("%s", OP_SYMBOL); \
        print_bint_hex_python(&ptrY); \
        printf(") == hex("); \
        print_bint_hex_python(&ptrZ); \
        printf("))\n"); \
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
            POSITIVE_RANDOMIZE_BINTS(ptrX, ptrY, rnd, fix);
        } else {
            RANDOMIZE_BINTS(ptrX, ptrY, rnd, fix);
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
void test_rand_SQU(int cnt, int bit_op, int sgn_op, int squ_op) {
    int idx = 0x00;
    while (idx < cnt) {
        BINT *ptrX = NULL, *ptrRes = NULL;
        int rnd, fix;

        SET_BIT_LENGTHS(bit_op, rnd, fix);
        int len = (rand() % rnd) + fix;
        int sgnX = false;
        if(!sgn_op) sgnX = rand()  % 0x02;
        RANDOM_BINT(&ptrX, sgnX, len);

        if(squ_op == 1) SQU_Krtsb_xz(&ptrX, &ptrRes);
        else SQU_Txtbk_xz(&ptrX, &ptrRes);

        printf("print(hex(");print_bint_hex_python(&ptrX);
        printf(" * ");print_bint_hex_python(&ptrX);
        printf(") == hex("); print_bint_hex_python(&ptrRes);
        printf("))\n");

        delete_bint(&ptrX);
        delete_bint(&ptrRes);
        idx++;
    }
}
void test_rand_DIV(int cnt, int bit_op, int sgn_op, int div_op) {
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
        // SET_BIT_LENGTHS(bit_op, rnd, fix);
        // if (sgn_op) {
        //     POSITIVE_RANDOMIZE_BINTS(ptrX, ptrY, rnd, fix);
        // } else {
        //     RANDOMIZE_BINTS(ptrX, ptrY, rnd, fix);
        // }

        if(div_op)
            DIV_long(&ptrX, &ptrY, &ptrQ, &ptrR);
        else
            DIV_Bianry_Long(&ptrX, &ptrY, &ptrQ, &ptrR);
        
        printf("print(hex("); print_bint_hex_python(&ptrQ);
        printf(" * "); print_bint_hex_python(&ptrY);
        printf(" + "); print_bint_hex_python(&ptrR);
        printf(") == hex("); print_bint_hex_python(&ptrX);
        printf("))\n");

        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrQ);
        delete_bint(&ptrR);
        idx++;
    }
}
// void test_rand_EXP(int cnt, int bit_op, int sgn_op, int exp_op) {
//     // test_rand_OP(cnt, bit_op, SUB, "-", sgn_op);
// }

// Configuration Macros
#define TEST_ITERATIONS 10000
#define MIN_BIT_LENGTH 32 // 1024-bit
#define MAX_BIT_LENGTH 64 // 2048-bit

// Timing Macro
#define MEASURE_TIME(start, end) ((double)(end - start) / CLOCKS_PER_SEC)

void performTest(void (*testFunc)(BINT**, BINT**, BINT**), BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    clock_t start = clock();
    testFunc(pptrX, pptrY, pptrZ);
    clock_t end = clock();
    printf("%.6f\n", MEASURE_TIME(start, end));
}

void performBINT(void (*testFunc1)(BINT**, BINT**, BINT**), void (*testFunc2)(BINT**, BINT**, BINT**)) {
    srand((unsigned int)time(NULL));

    for (int idx = 0; idx < TEST_ITERATIONS; idx++) {
        int len1 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;
        int len2 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;

        BINT *ptrX = NULL, *ptrY = NULL, *ptrZ = NULL;
        BINT* ptrTmpX = NULL;
        BINT* ptrTmpY = NULL;
        BINT* ptrTmpZ = NULL;
        RANDOM_BINT(&ptrX, 0, len1);
        RANDOM_BINT(&ptrY, 0, len2);
        copyBINT(&ptrTmpX, &ptrX);      
        copyBINT(&ptrTmpY, &ptrY);

        performTest(testFunc1, &ptrX, &ptrY, &ptrZ);
        performTest(testFunc2, &ptrTmpX, &ptrTmpY, &ptrTmpZ);

        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrZ);
        delete_bint(&ptrTmpX);
        delete_bint(&ptrTmpY);
        delete_bint(&ptrTmpZ);
    }
}

int main() {
    /*
     * bit_op
     * 1: 2048 ~ 3072 bits
     * 2: 3072 ~ 7680 bits
     * 3: 7680 ~ 15360 bits
     * 4: 1024-bit
     * 5: 2048-bit
     * 5: 3072-bit
     * 5: 7680-bit
     * 5: 15360-bit            case 5: rnd = 0x00; fix = 0x040; break;
     * default(0): 1024 ~ 2048 bits
     * ===============================================================
     * sgn_op
     * 0: random sign
     * 1: positive sign
     * ===============================================================
     * mul_op
     * 1: Improved TextBook
     * 2: Karatsuba
     * default(0): TextBook
     * ===============================================================
     * squ_op
     * 1: Karastsuba
     * default(0): TextBook
     * ===============================================================
     * test_rand_ADD(int cnt, int bit_op, int sgn_op)
     * test_rand_SUB(int cnt, int bit_op, int sgn_op)
     * test_rand_MUL(int cnt, int bit_op, int sgn_op, int mul_op)
     * test_rand_SQU(int cnt, int bit_op, int sgn_op, int squ_op)
    */

    // performBINT(MUL_Core_ImpTxtBk_xyz, MUL_Core_Krtsb_xyz);

    int t = 1000;
    int bit_op = 3;
    int sgn_op = 1;

    // Addition and Subtraction
    // test_rand_ADD(t, bit_op, sgn_op);
    // test_rand_SUB(t, bit_op, sgn_op);

    // Multiplication
    // test_rand_MUL(t, bit_op, sgn_op, 0); // TextBook
    // test_rand_MUL(t, bit_op, sgn_op, 1); // Improved TextBook
    test_rand_MUL(t, bit_op, sgn_op, 2); // Karatsuba

    // Squaring
    // test_rand_SQU(t, bit_op, sgn_op, 0);
    // test_rand_SQU(t, bit_op, sgn_op, 1);

    // Division
    // test_rand_DIV(t, bit_op, sgn_op, 0); // Binary Long
    // test_rand_DIV(t, bit_op, sgn_op, 1); // General Long


    return 0;
}

/*
 *  Measuring Performance
*/
// int main() {
//     srand((unsigned int)time(NULL));

//     clock_t start1, end1;
//     double cpu_time_used1;
//     clock_t start2, end2;
//     double cpu_time_used2;
//     // clock_t start3, end3;
//     // double cpu_time_used3;
//     int t = 10000;

//     /**
//      * if WORD_BITLEN = 32
//      * 0x010 -> ( 1 * 16 =  16) -> (16 *  32 =   512-bit)
//      * 0x020 -> ( 2 * 16 =  32) -> (32 *  32 =  1024-bit)
//      * 0x040 -> ( 4 * 16 =  64) -> (32 *  64 =  2048-bit)
//      * 0x060 -> ( 6 * 16 =  96) -> (32 *  96 =  3072-bit)
//      * 0x0f0 -> (15 * 16 = 240) -> (32 * 240 =  7680-bit)
//      * 
//      * 0x1e0 -> ( 1 * 256 + 14 * 16 = 480) -> (32 * 480 = 15360-bit)
//     */

//     int idx = 0;
//     while(idx < t) {
//         BINT* ptrX = NULL;
//         BINT* ptrY = NULL;
//         BINT* ptrZ = NULL;
//         BINT* ptrTmpX = NULL;
//         BINT* ptrTmpY = NULL;
//         BINT* ptrTmpZ = NULL;
//         // BINT* ptrTTmpX = NULL;
//         // BINT* ptrTTmpY = NULL;
//         // BINT* ptrTTmpZ = NULL;
// /*************************** Random Input **************************************/
//         // int len1 = (rand() % 0x010) + 0x010; //  512 ~ 1024 bits
//         // int len2 = (rand() % 0x010) + 0x010; //  512 ~ 1024 bits
//         int len1 = (rand() % 0x020) + 0x020; // 1024 ~ 2048 bits
//         int len2 = (rand() % 0x020) + 0x020; // 1024 ~ 2048 bits
//         // int len1 = (rand() % 0x020) + 0x040; // 2048 ~ 3072 bits
//         // int len2 = (rand() % 0x020) + 0x040; // 2048 ~ 3072 bits
//         // int len1 = (rand() % 0x090) + 0x060; // 3072 ~ 7680 bits
//         // int len2 = (rand() % 0x090) + 0x060; // 3072 ~ 7680 bits
//         // int len1 = (rand() % 0xf0) + 0x0f0; // 7680 ~ 15360 bits
//         // int len2 = (rand() % 0xf0) + 0x0f0; // 7680 ~ 15360 bits

//         // int len1 = 0x020;
//         // int len2 = 0x020;
      
//         RANDOM_BINT(&ptrX, false, len1);
//         RANDOM_BINT(&ptrY, false, len2);

//         // int sgn1 = rand() % 0x02;
//         // int sgn2 = rand() % 0x02;
//         // RANDOM_BINT(&ptrX, false, len1);
//         // RANDOM_BINT(&ptrY, false, len2);
        
//         copyBINT(&ptrTmpX, &ptrX);      
//         copyBINT(&ptrTmpY, &ptrX);
//         // copyBINT(&ptrTTmpX, &ptrX);      
//         // copyBINT(&ptrTTmpY, &ptrY);
        
//         start1 = clock();
//         // add_core_xyz(&ptrX, &ptrY, &ptrZ);
//         // mul_core_TxtBk_xyz(&ptrTmpX,&ptrTmpY,&ptrTmpZ);
//         // SQU_Txtbk_xz(&ptrX, &ptrZ);
//         MUL_Core_Krtsb_xyz(&ptrTmpX,&ptrTmpY,&ptrTmpZ);
//         end1 = clock();
//         cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;

//         start2 = clock();
//         // ADD(&ptrTmpX,&ptrTmpY,&ptrTmpZ);
//         // sub_core_xyz(&ptrTmpX,&ptrTmpY,&ptrTmpZ);
//         // MUL_Core_ImpTxtBk_xyz(&ptrX,&ptrY,&ptrZ);
//         // MUL_Core_ImpTxtBk_xyz(&ptrX,&ptrY,&ptrZ);
//         // SQU_Txtbk_xz(&ptrX, &ptrZ);
//         SQU_Krtsb_xz(&ptrX,&ptrZ);
//         end2 = clock();
//         cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;

//         // start3 = clock();
//         // Krtsb_FLAG_Test(&ptrX,&ptrY,&ptrZ, 0x08);
//         // end3 = clock();
//         // cpu_time_used3 = ((double) (end3 - start3)) / CLOCKS_PER_SEC;
        
//         delete_bint(&ptrX);
//         delete_bint(&ptrY);
//         delete_bint(&ptrZ);
//         delete_bint(&ptrTmpX);
//         delete_bint(&ptrTmpY);
//         delete_bint(&ptrTmpZ);
//         // delete_bint(&ptrTTmpX);
//         // delete_bint(&ptrTTmpY);
//         // delete_bint(&ptrTTmpZ);
//         printf("%.6f\n", cpu_time_used1);
//         printf("%.6f\n", cpu_time_used2);
//         // printf("%.6f\n", cpu_time_used3);
//         // printf("%.6f\n", cpu_time_used1-cpu_time_used2);
//         idx++;
//     }
// }










































// int main() {
//     srand((unsigned int)time(NULL));

//     // clock_t start1, end1;
//     // double cpu_time_used1;
//     // clock_t start2, end2;
//     // double cpu_time_used2;
//     int t = 10000;

//     /**
//      * if WORD_BITLEN = 32,
//      * 0x010 -> ( 1 * 16 =  16) -> (16 *  32 =   512-bit)
//      * 0x020 -> ( 2 * 16 =  32) -> (32 *  32 =  1024-bit)
//      * 0x040 -> ( 4 * 16 =  64) -> (32 *  64 =  2048-bit)
//      * 0x060 -> ( 6 * 16 =  96) -> (32 *  96 =  3072-bit)
//      * 0x0f0 -> (15 * 16 = 240) -> (32 * 240 =  7680-bit)
//      * 0x1e0 -> ( 1 * 256 + 14 * 16 = 480) -> (32 * 480 = 15360-bit)
//     */
     
//     int idx = 0;
//     while(idx < t) {
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
        
//         // init_bint(&ptrZ, len1 + len2);
//         // init_bint(&ptrQ, len1);
//         // init_bint(&ptrR, len2);
//         // printf("X(%x):", len1);print_bint_hex(ptrX);
//         // printf("Y(%x):", len2);print_bint_hex(ptrY);
        
//         // int sgn1 = rand() % 0x02;
//         // int sgn2 = rand() % 0x02;
//         // RANDOM_BINT(&ptrX, sgn1, len1);
//         // RANDOM_BINT(&ptrY, sgn2, len2);
//         RANDOM_BINT(&ptrX, false, len1);
//         RANDOM_BINT(&ptrY, false, len2);
     
//         // copyBINT(&ptrTmpX, &ptrX);      
//         // copyBINT(&ptrTmpY, &ptrY);
// /*******************************************************************************/       
// /*************************** Non-Random Input **************************************/
//         // const char* ptrTestX = "0xe6a29ab895e3d2fc3e8178be0d8b5dfb5482379e4e92abd9130f20265f81f22d0db7e698";
//         // const char* ptrTestY = "0xa551847f73ed611236dbc3b634a5f3757f9cb9ecb09f3431733b477512951425dd971e61";
//         // const char* ptrTestX = "0x44b7014fff077a1eb414a9dc3694ee5d";
//         // const char* ptrTestY = "0x8e91703149a154b4b2a6faf2ce5b3f93";
//         // const char* ptrTestX = "0xae035de48a0ca938";
//         // const char* ptrTestY = "0x0000000010a85e3a";
//         // const char* ptrTestX = "0xb6023d19fc07299af27e5697a97efc48871297e9f656964ef383e6c9d8e34b9d0636ef6215ab445ad5d89b60f6eab19e1c4052a86313682aab027d3add15aea320441ebbd66cbf74e1b1af58";
//         // const char* ptrTestY = "0xf849d8af6eb2e1b9316a9335cfe29cde1555fa8346e01ba33c4e868766ed8249da26ceb5817726fb9bb199f1cd47e3910f1a68a3a3cd1db5e899051d125c7b25a5578e85dc7562a9e4520e3b38c9296c779b86eee483b0cb87a9a2edb724971f7b19233291109ba81a4512cc528ee689";
//         // strToBINT(&ptrX, ptrTestX);
//         // strToBINT(&ptrY, ptrTestY);
//         // int len1 = ptrX->wordlen;
//         // int len2 = ptrY->wordlen; 
//         // printf("X(%x):", len1);print_bint_hex(ptrX);
//         // printf("Y(%x):", len2);print_bint_hex(ptrY);
// /***********************************************************************************/

//         // if (ptrX->wordlen < ptrY->wordlen) swapBINT(&ptrX,&ptrY);
//         // printf("Test[%d]---------------------------------------------------\n", idx+1);
//         // printf("len1 len2 : %d %d\n", len1, len2);
//         // custom_printHex_xy(ptrX, ptrY, MAX(len1,len2));
//         // custom_printHex_xy(ptrX, ptrY, MIN(len1,len2));
//         // custom_printHex_xy(ptrX, ptrY, len1+len2);

//         // print_bint_hex_split(ptrX);
//         // print_bint_hex_split(ptrY);
//         // start1 = clock();
//         // add_core_xyz(&ptrX,&ptrY,&ptrZ);
//         // ADD(&ptrX,&ptrY,&ptrZ);
//         // sub_core_xyz(&ptrX,&ptrY,&ptrZ);
//         // SUB(&ptrX,&ptrY,&ptrZ);
//         // mul_xyz(ptrX->val[0], ptrY->val[0], &ptrZ);  // len1 = 1 = len2
//         // mul_core_TxtBk_xyz(&ptrX,&ptrY,&ptrZ);
//         // mul_core_ImpTxtBk_test(&ptrX,&ptrY,&ptrZ);
//         // MUL_Core_ImpTxtBk_xyz(&ptrX,&ptrY,&ptrZ);
//         // mul_core_Krtsb_test(&ptrX,&ptrY,&ptrZ);
//         // Krtsb_FLAG_Test(&ptrX,&ptrY,&ptrZ, 0x8);
//         MUL_Core_Krtsb_xyz(&ptrX,&ptrY,&ptrZ);
//         // DIV_Bianry_Long_Test(&ptrX, &ptrY, &ptrQ, &ptrR);
//         // DIV_Bianry_Long(&ptrX, &ptrY, &ptrQ, &ptrR);
//         // exp_Mongomery(&ptrX,&ptrY,&ptrZ);
//         // bool* binaryX = HexToBinary(ptrX);
//         // bool* binaryY = HexToBinary(ptrY);
//         // end1 = clock();
//         // cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;

//         // start2 = clock();
//         // MUL_Core_ImpTxtBk_xyz(&ptrTmpX,&ptrTmpY,&ptrTmpZ);
//         // end2 = clock();
//         // cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;

//         // start3 = clock();
//         // // MUL_Core_ImpTxtBk_xyz(&ptrTTmpX,&ptrTTmpY,&ptrTTmpZ);
//         // Krtsb_FLAG_Test(&ptrTTmpX,&ptrTTmpY,&ptrTTmpZ, 3);
//         // end3 = clock();
//         // cpu_time_used3 = ((double) (end3 - start3)) / CLOCKS_PER_SEC;

//         // custom_printHex(ptrX, ptrY, ptrZ, 0);
//         // custom_printHex(ptrX, ptrY, ptrZ, 1);
//         // custom_printHex(ptrX, ptrY, ptrZ, 2);
//         // custom_printHex(ptrTmpX, ptrTmpY, ptrTmpZ, 2);

//         // const char *hexData = "1A3F";  // Sample data
//         // char binaryOutput[5 * strlen(hexData) + 1];  // Max potential output size (4 bits for each hex + null terminator)

//         // hexToBinary(hexData, binaryOutput);
//         // printf("Hex: %s\nBinary: %s\n", hexData, binaryOutput);

// /*************************** Sage (or Python) Test **************************************/
//         /** SAGE (ADD)
//          * print(hex(0x00 + 0x00) == hex(0x00))
//         */ 
//         // printf("print(hex(");
//         // print_bint_hex_python(&ptrX);
//         // printf(" + ");
//         // print_bint_hex_python(&ptrX);
//         // printf(") == hex(");
//         // print_bint_hex_python(&ptrX);
        
//         /** SAGE (SUB).
//          * print((hex(0x00 - 0x00) == hex(0x00))
//         */
//         // printf("print(hex(");
//         // print_bint_hex_python(&ptrX);
//         // printf(" - ");
//         // print_bint_hex_python(&ptrY);
//         // printf(") == hex(");
//         // print_bint_hex_python(&ptrZ);
//         // printf("))\n");

//         /** SAGE (MUL)
//          * print(hex(0x00 * 0x00) == hex(0x00))
//         */ 
//         printf("print(");
//         print_bint_hex_python(&ptrX);
//         printf(" * ");
//         print_bint_hex_python(&ptrY);
//         printf(" == ");
//         print_bint_hex_python(&ptrZ);
//         printf(")\n");
        
//         /** SAGE (DIV)
//          * print(hex(0x00 * 0x00 + 0x00) == hex(0x00))
//         // */ 
//         // printf("print(hex(");
//         // print_bint_hex_python(&ptrQ);
//         // printf(" * ");
//         // print_bint_hex_python(&ptrY);
//         // printf(" + ");
//         // print_bint_hex_python(&ptrR);
//         // printf(") == hex(");
//         // print_bint_hex_python(&ptrX);
//         // printf("))\n");

//         /** SAGE (EXP)
//          * print(hex(0x00 ** 0x00) == hex(0x00))
//          * print(hex(power_mod(0x02, 0x03, 0x07)))
//         */ 
//         // printf("print(hex(");
//         // print_bint_hex_python(&ptrX);
//         // printf(" ** ");
//         // print_bint_hex_python(&ptrY);
//         // printf(") == hex(");
//         // print_bint_hex_python(&ptrZ);
//         // printf("))\n");
// /****************************************************************************************/
//         // printf("Result Q:");print_bint_hex(ptrQ);
//         // printf("Result R:");print_bint_hex(ptrR);


//         // printf("==================================================\n");
        
//         delete_bint(&ptrX);
//         delete_bint(&ptrY);
//         delete_bint(&ptrZ);
//         // delete_bint(&ptrTmpX);
//         // delete_bint(&ptrTmpY);
//         // delete_bint(&ptrTmpZ);
//         // delete_bint(&ptrQ);
//         // delete_bint(&ptrR);

//         // delete_bint(&ptrX2);
//         // delete_bint(&ptrY2);
//         // printf("%.6f\n", cpu_time_used1);
//         // printf("%.6f\n", cpu_time_used2);
//         idx++;
//     }
// }

// void test_flint_mul(int count) {
//     struct timespec start, end;
//     double elapsed_time;

//     fmpz_t a, b, c;
//     fmpz_init(a);
//     fmpz_init(b);
//     fmpz_init(c);

//     flint_rand_t state;
//     flint_randinit(state);

//     // Testing multiplication 'count' times
//     for (int i = 0; i < count; i++) {
//         fmpz_randbits(a, state, 3072); // Generate a random 3072-bit number
//         fmpz_randbits(b, state, 3072); // Generate another random 3072-bit number

//         clock_gettime(CLOCK_MONOTONIC, &start);
        
//         // fmpz_sub(c, a, b);
//         fmpz_add(c, a, b); // Multiply a and b
//         clock_gettime(CLOCK_MONOTONIC, &end);

//         elapsed_time = (end.tv_sec - start.tv_sec) * 1e9;
//         elapsed_time = (elapsed_time + (end.tv_nsec - start.tv_nsec)) * 1e-9; // Convert to seconds
//         printf("%.3f\n", elapsed_time*1000000);
//     }

//     fmpz_clear(a);
//     fmpz_clear(b);
//     fmpz_clear(c);
//     flint_randclear(state);
// }

// /*
//  *  Compare Performance with FLINT Library
// */
// int main() {
//     srand((unsigned int)time(NULL));
    
//     struct timespec start, end;
//     double elapsed_time;
    
//     int t = 10000;

//     test_flint_mul(t);

//     int idx = 0;
//     while(idx < t) {
//         BINT* ptrX = NULL;
//         BINT* ptrY = NULL;
//         BINT* ptrZ = NULL;

//         int len1 = 0x60;
//         int len2 = 0x60;
        
//         RANDOM_BINT(&ptrX, false, len1);
//         RANDOM_BINT(&ptrY, false, len2);

//         clock_gettime(CLOCK_MONOTONIC, &start);
//         add_core_xyz(&ptrX,&ptrY,&ptrZ);
//         // MUL_Core_Krtsb_xyz(&ptrX,&ptrY,&ptrZ);
//         // SUB(&ptrX,&ptrY,&ptrZ);
//         clock_gettime(CLOCK_MONOTONIC, &end);  
//         elapsed_time = (end.tv_sec - start.tv_sec) * 1e9;
//         elapsed_time = (elapsed_time + (end.tv_nsec - start.tv_nsec)) * 1e-9; // Convert to seconds

//         delete_bint(&ptrX);
//         delete_bint(&ptrY);
//         delete_bint(&ptrZ);
//         printf("%.3f\n", elapsed_time*1000000);
//         idx++;
//     }
// }

/*
 *  FLAG TEST
*/
// int main() {
//     srand((unsigned int)time(NULL));

//     clock_t start1, end1;
//     double cpu_time_used1;
//     clock_t start2, end2;
//     double cpu_time_used2;
//     int t = 10000;
    
//     int idx = 0;
//     while(idx < t) {
//         BINT* ptrX = NULL;
//         BINT* ptrY = NULL;
//         BINT* ptrZ = NULL;
//         BINT* ptrTmpX = NULL;
//         BINT* ptrTmpY = NULL;
//         BINT* ptrTmpZ = NULL;
// /*************************** Random Input **************************************/
//         /**
//          * if WORD_BITLEN = 32,
//          * 0x010 -> ( 1 * 16 =  16) -> (16 *  32 =   512-bit)
//          * 0x020 -> ( 2 * 16 =  32) -> (32 *  32 =  1024-bit)
//          * 0x040 -> ( 4 * 16 =  64) -> (32 *  64 =  2048-bit)
//          * 0x060 -> ( 6 * 16 =  96) -> (32 *  96 =  3072-bit)
//          * 0x0f0 -> (15 * 16 = 240) -> (32 * 240 =  7680-bit)
//          * 0x1e0 -> ( 1 * 256 + 14 * 16 = 480) -> (32 * 480 = 15360-bit)
//         */
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
        
//         int len1 = 0x060;
//         int len2 = 0x060;
        
//         // RANDOM_BINT(&ptrX, false, len1);
//         // RANDOM_BINT(&ptrY, false, len2);
        
//         // int sgn1 = rand() % 0x02;
//         // int sgn2 = rand() % 0x02;
//         RANDOM_BINT(&ptrX, false, len1);
//         RANDOM_BINT(&ptrY, false, len2);
     
//         copyBINT(&ptrTmpX, &ptrX);      
//         copyBINT(&ptrTmpY, &ptrY);

//         start1 = clock();
//         MUL_Core_ImpTxtBk_xyz(&ptrX,&ptrY,&ptrZ);
//         end1 = clock();
//         // cpu_time_used1 = ((double) (end1 - start1));
//         cpu_time_used1 = ((double) (end1 - start1));// / CLOCKS_PER_SEC;

//         start2 = clock();
//         // FLAG = 2, 4, 8, 16, 32
//         Krtsb_FLAG_Test(&ptrTmpX,&ptrTmpY,&ptrTmpZ, 0x20);
//         end2 = clock();
//         cpu_time_used2 = ((double) (end2 - start2));// / CLOCKS_PER_SEC;

//         delete_bint(&ptrX);
//         delete_bint(&ptrY);
//         delete_bint(&ptrZ);
//         delete_bint(&ptrTmpX);
//         delete_bint(&ptrTmpY);
//         delete_bint(&ptrTmpZ);
//         printf("%.0f\n", (cpu_time_used1-cpu_time_used2));
//         idx++;
//     }
// }