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

// Function prototypes for operations
void test_rand_OP(int cnt, int bit_op, void (*op_func)(BINT**, BINT**, BINT**), const char* op_symbol, int positive);
void ADD(BINT**, BINT**, BINT**);
void SUB(BINT**, BINT**, BINT**);
void mul_core_TxtBk(BINT**, BINT**, BINT**);
void MUL_Core_ImpTxtBk_xyz(BINT**, BINT**, BINT**);
void MUL_Core_Krtsb_xyz(BINT**, BINT**, BINT**);

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
            case 8: rnd = 0x00; fix = 0x1e0; break; /* 15360-s */ \
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

        int len1 = 0x08;
        int len2 = 0x07;
        
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

void test_flint_mul(int count) {
    struct timespec start, end;
    double elapsed_time;

    fmpz_t a, b, c;
    fmpz_init(a);
    fmpz_init(b);
    fmpz_init(c);

    flint_rand_t state;
    flint_randinit(state);

    // Testing multiplication 'count' times
    for (int i = 0; i < count; i++) {
        fmpz_randbits(a, state, 1024); // Generate a random 2048-bit number
        fmpz_randbits(b, state, 1024); // Generate another random 2048-bit number

        clock_gettime(CLOCK_MONOTONIC, &start);
        fmpz_mul(c, a, b); // Multiply a and b
        clock_gettime(CLOCK_MONOTONIC, &end);

        elapsed_time = (end.tv_sec - start.tv_sec) * 1e9;
        elapsed_time = (elapsed_time + (end.tv_nsec - start.tv_nsec)) * 1e-9; // Convert to seconds
        printf("%.6f\n", elapsed_time*1000);
    }

    fmpz_clear(a);
    fmpz_clear(b);
    fmpz_clear(c);
    flint_randclear(state);
}

// int main() {
//     /*
//      * bit_op
//      * 1: 2048 ~ 3072 bits
//      * 2: 3072 ~ 7680 bits
//      * 3: 7680 ~ 15360 bits
//      * default(0): 1024 ~ 2048 bits
//      * ===============================================================
//      * sgn_op
//      * 0: random sign
//      * 1: positive sign
//      * ===============================================================
//      * mul_op
//      * 1: Improved TextBook
//      * 2: Karatsuba
//      * default(0): TextBook
//      * ===============================================================
//      * squ_op
//      * 1: Karastsuba
//      * default(0): TextBook
//      * ===============================================================
//      * test_rand_ADD(int cnt, int bit_op, int sgn_op)
//      * test_rand_SUB(int cnt, int bit_op, int sgn_op)
//      * test_rand_MUL(int cnt, int bit_op, int sgn_op, int mul_op)
//      * test_rand_SQU(int cnt, int bit_op, int sgn_op, int squ_op)
//     */

//     int t = 1000;
//     int bit_op = 1;
//     int sgn_op = 1;

//     // Addition and Subtraction
//     // test_rand_ADD(t, bit_op, sgn_op);
//     // test_rand_SUB(t, bit_op, sgn_op);

//     // Multiplication
//     // test_rand_MUL(t, bit_op, sgn_op, 0); // TextBook
//     // test_rand_MUL(t, bit_op, sgn_op, 1); // Improved TextBook
//     // test_rand_MUL(t, bit_op, sgn_op, 2); // Kratsuba

//     // Squaring
//     // test_rand_SQU(t, bit_op, sgn_op, 0);
//     test_rand_SQU(t, bit_op, sgn_op, 1);

//     // Division
//     // test_rand_DIV(t, bit_op, sgn_op, 0); // Binary Long
//     // test_rand_DIV(t, bit_op, sgn_op, 1); // General Long


//     return 0;
// }

/*
 *  Measuring Performance
*/
int main() {
    srand((unsigned int)time(NULL));

    clock_t start1, end1;
    double cpu_time_used1;
    clock_t start2, end2;
    double cpu_time_used2;
    // clock_t start3, end3;
    // double cpu_time_used3;
    int t = 10000;

    /**
     * if WORD_BITLEN = 32
     * 0x010 -> ( 1 * 16 =  16) -> (16 *  32 =   512-bit)
     * 0x020 -> ( 2 * 16 =  32) -> (32 *  32 =  1024-bit)
     * 0x040 -> ( 4 * 16 =  64) -> (32 *  64 =  2048-bit)
     * 0x060 -> ( 6 * 16 =  96) -> (32 *  96 =  3072-bit)
     * 0x0f0 -> (15 * 16 = 240) -> (32 * 240 =  7680-bit)
     * 
     * 0x1e0 -> ( 1 * 256 + 14 * 16 = 480) -> (32 * 480 = 15360-bit)
    */

    int idx = 0;
    while(idx < t) {
        BINT* ptrX = NULL;
        BINT* ptrY = NULL;
        BINT* ptrZ = NULL;
        BINT* ptrTmpX = NULL;
        BINT* ptrTmpY = NULL;
        BINT* ptrTmpZ = NULL;
        // BINT* ptrTTmpX = NULL;
        // BINT* ptrTTmpY = NULL;
        // BINT* ptrTTmpZ = NULL;
/*************************** Random Input **************************************/
        // int len1 = (rand() % 0x010) + 0x010; //  512 ~ 1024 bits
        // int len2 = (rand() % 0x010) + 0x010; //  512 ~ 1024 bits
        int len1 = (rand() % 0x020) + 0x020; // 1024 ~ 2048 bits
        int len2 = (rand() % 0x020) + 0x020; // 1024 ~ 2048 bits
        // int len1 = (rand() % 0x020) + 0x040; // 2048 ~ 3072 bits
        // int len2 = (rand() % 0x020) + 0x040; // 2048 ~ 3072 bits
        // int len1 = (rand() % 0x090) + 0x060; // 3072 ~ 7680 bits
        // int len2 = (rand() % 0x090) + 0x060; // 3072 ~ 7680 bits
        // int len1 = (rand() % 0xf0) + 0x0f0; // 7680 ~ 15360 bits
        // int len2 = (rand() % 0xf0) + 0x0f0; // 7680 ~ 15360 bits

        // int len1 = 0x020;
        // int len2 = 0x020;
      
        RANDOM_BINT(&ptrX, false, len1);
        RANDOM_BINT(&ptrY, false, len2);

        // int sgn1 = rand() % 0x02;
        // int sgn2 = rand() % 0x02;
        // RANDOM_BINT(&ptrX, false, len1);
        // RANDOM_BINT(&ptrY, false, len2);
        
        copyBINT(&ptrTmpX, &ptrX);      
        copyBINT(&ptrTmpY, &ptrX);
        // copyBINT(&ptrTTmpX, &ptrX);      
        // copyBINT(&ptrTTmpY, &ptrY);
        
        start1 = clock();
        // add_core_xyz(&ptrX, &ptrY, &ptrZ);
        // mul_core_TxtBk_xyz(&ptrTmpX,&ptrTmpY,&ptrTmpZ);
        // SQU_Txtbk_xz(&ptrX, &ptrZ);
        MUL_Core_Krtsb_xyz(&ptrTmpX,&ptrTmpY,&ptrTmpZ);
        end1 = clock();
        cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;

        start2 = clock();
        // ADD(&ptrTmpX,&ptrTmpY,&ptrTmpZ);
        // sub_core_xyz(&ptrTmpX,&ptrTmpY,&ptrTmpZ);
        // MUL_Core_ImpTxtBk_xyz(&ptrX,&ptrY,&ptrZ);
        // MUL_Core_ImpTxtBk_xyz(&ptrX,&ptrY,&ptrZ);
        // SQU_Txtbk_xz(&ptrX, &ptrZ);
        SQU_Krtsb_xz(&ptrX,&ptrZ);
        end2 = clock();
        cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;

        // start3 = clock();
        // Krtsb_FLAG_Test(&ptrX,&ptrY,&ptrZ, 0x08);
        // end3 = clock();
        // cpu_time_used3 = ((double) (end3 - start3)) / CLOCKS_PER_SEC;
        
        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrZ);
        delete_bint(&ptrTmpX);
        delete_bint(&ptrTmpY);
        delete_bint(&ptrTmpZ);
        // delete_bint(&ptrTTmpX);
        // delete_bint(&ptrTTmpY);
        // delete_bint(&ptrTTmpZ);
        printf("%.6f\n", cpu_time_used1);
        printf("%.6f\n", cpu_time_used2);
        // printf("%.6f\n", cpu_time_used3);
        // printf("%.6f\n", cpu_time_used1-cpu_time_used2);
        idx++;
    }
}










































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
//         int len1 = (rand() % 0x020) + 0x020; // 1024 ~ 2048 bits
//         int len2 = (rand() % 0x020) + 0x020; // 1024 ~ 2048 bits
//         // int len1 = (rand() % 0x020) + 0x040; // 2048 ~ 3072 bits
//         // int len2 = (rand() % 0x020) + 0x040; // 2048 ~ 3072 bits
//         // int len1 = (rand() % 0x090) + 0x060; // 3072 ~ 7680 bits
//         // int len2 = (rand() % 0x090) + 0x060; // 3072 ~ 7680 bits
//         // int len1 = (rand() % 0xf0) + 0x0f0; // 7680 ~ 15360 bits
//         // int len2 = (rand() % 0xf0) + 0x0f0; // 7680 ~ 15360 bits
        
//         // int len1 = (rand() % 0x05) + 0x05;
//         // int len2 = (rand() % 0x05) + 0x05;
        
//         // int len1 = 0x04;
//         // int len2 = 0x08;
        
//         // init_bint(&ptrZ, len1 + len2);
//         // init_bint(&ptrQ, len1);
//         // init_bint(&ptrR, len2);
//         // printf("X(%x):", len1);print_bint_hex(ptrX);
//         // printf("Y(%x):", len2);print_bint_hex(ptrY);
        
//         int sgn1 = rand() % 0x02;
//         int sgn2 = rand() % 0x02;
//         RANDOM_BINT(&ptrX, sgn1, len1);
//         RANDOM_BINT(&ptrY, sgn2, len2);
//         // RANDOM_BINT(&ptrX, false, len1);
//         // RANDOM_BINT(&ptrY, false, len2);
     
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
//         // const char* ptrTestX = "0x909440cbc3ff0daf2b328f80c9a99a8d924766b6eea3e28f0d839cef3bac21c5d141be14d4db1faa69e6e63c175326db59c3d45f965ff31e47efce76dbdf06bc6de3f5297239c42dcf93153bfa6307f92d756c78170350d114ee701f7223c74471bc65456931179499b2340035918db55e692846ef6161f57c4d572eb10bdf2c52332dd598655f987da3738033248d1779c6e5fae5149d4e9b4c4f85b9abff7d";
//         // const char* ptrTestY = "0x7e2d7f3602a84bba5107d3c130b79587713d9688f013dae5dd0c243396a224482ad7eb485744e790d71c2ba94558f0e94d83ae0b36cec0aa75820e4e784d5de280a760c76dcb642ed6d3b85c0b6d3b414ce04b8c583e9b270ac1f195cdb682b93a95623ff591757ac84b20c9215bf6c0870cf13134da3574f33b13147ced56fba0d1c97ac82a5aff2ac5d361d4eacf56d816fcc0e7fa61dfbf26be814d853125099f20c900952cc9f1e9927e12b85a182f025f083a3a213ed9394f277c92053f9e8b9d37a4433773d7d924fa8958b927c724e7fc61e8efb2bdbd223cf479a267e75a00316c5d4fdd50cc4161";
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
//         SUB(&ptrX,&ptrY,&ptrZ);
//         // mul_xyz(ptrX->val[0], ptrY->val[0], &ptrZ);  // len1 = 1 = len2
//         // mul_core_TxtBk_xyz(&ptrX,&ptrY,&ptrZ);
//         // mul_core_ImpTxtBk_test(&ptrX,&ptrY,&ptrZ);
//         // MUL_Core_ImpTxtBk_xyz(&ptrX,&ptrY,&ptrZ);
//         // mul_core_Krtsb_test(&ptrX,&ptrY,&ptrZ);
//         // Krtsb_FLAG_Test(&ptrX,&ptrY,&ptrZ, 0x8);
//         // MUL_Core_Krtsb_xyz(&ptrX,&ptrY,&ptrZ);
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
//         printf("print(hex(");
//         print_bint_hex_python(&ptrX);
//         printf(" - ");
//         print_bint_hex_python(&ptrY);
//         printf(") == hex(");
//         print_bint_hex_python(&ptrZ);
//         printf("))\n");

//         /** SAGE (MUL)
//          * print(hex(0x00 * 0x00) == hex(0x00))
//         */ 
//         // printf("print(hex(");
//         // print_bint_hex_python(&ptrX);
//         // printf(" * ");
//         // print_bint_hex_python(&ptrY);
//         // printf(") == hex(");
//         // print_bint_hex_python(&ptrZ);
//         // printf("))\n");
        
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

/*
 *  Library Performance
*/
// int main() {
//     srand((unsigned int)time(NULL));
    
//     struct timespec start, end;
//     double elapsed_time;
    
//     // clock_t start2, end2;
//     // double cpu_time_used2;
//     // clock_t start3, end3;
//     // double cpu_time_used3;
//     int t = 10000;

//     test_flint_mul(t);

//     int idx = 0;
//     while(idx < t) {
//         BINT* ptrX = NULL;
//         BINT* ptrY = NULL;
//         BINT* ptrZ = NULL;
//         // BINT* ptrTmpX = NULL;
//         // BINT* ptrTmpY = NULL;
//         // BINT* ptrTmpZ = NULL;
//         // BINT* ptrTTmpX = NULL;
//         // BINT* ptrTTmpY = NULL;
//         // BINT* ptrTTmpZ = NULL;
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

//         int len1 = 0x20;
//         int len2 = 0x20;
        
//         RANDOM_BINT(&ptrX, false, len1);
//         RANDOM_BINT(&ptrY, false, len2);
        
//         // int sgn1 = rand() % 0x02;
//         // int sgn2 = rand() % 0x02;
//         // RANDOM_BINT(&ptrX, false, len1);
//         // RANDOM_BINT(&ptrY, false, len2);
     
//         // copyBINT(&ptrTmpX, &ptrX);      
//         // copyBINT(&ptrTmpY, &ptrY);
//         // copyBINT(&ptrTTmpX, &ptrX);      
//         // copyBINT(&ptrTTmpY, &ptrY);

//         clock_gettime(CLOCK_MONOTONIC, &start);
//         // MUL_Core_Krtsb_xyz(&ptrX, &ptrY, &ptrZ);
//         // MUL_Core_ImpTxtBk_xyz(&ptrX,&ptrX2,&ptrZ);
//         // SQU_Txtbk_xz(&ptrX, &ptrZ);
//         // mul_core_TxtBk_xyz(&ptrX,&ptrY,&ptrZ);
//         MUL_Core_Krtsb_xyz(&ptrX,&ptrY,&ptrZ);
//         clock_gettime(CLOCK_MONOTONIC, &end);  
//         elapsed_time = (end.tv_sec - start.tv_sec) * 1e9;
//         elapsed_time = (elapsed_time + (end.tv_nsec - start.tv_nsec)) * 1e-9; // Convert to seconds


//         // start2 = clock();
//         // // SQU_Txtbk_xz(&ptrTmpX,&ptrTmpZ);
//         // // SQU_Krtsb_xz(&ptrTmpX,&ptrTmpZ);
//         // MUL_Core_ImpTxtBk_xyz(&ptrTmpX,&ptrTmpY,&ptrTmpZ);
//         // end2 = clock();
//         // cpu_time_used2 = ((double) (end2 - start2));// / CLOCKS_PER_SEC;
        
//         // start3 = clock();
//         // // Krtsb_FLAG_Test(&ptrX,&ptrY,&ptrZ, 0x08);
//         // MUL_Core_Krtsb_xyz(&ptrTTmpX,&ptrTTmpY,&ptrTTmpZ);
//         // end3 = clock();
//         // cpu_time_used3 = ((double) (end3 - start3));// / CLOCKS_PER_SEC;

//         delete_bint(&ptrX);
//         delete_bint(&ptrY);
//         delete_bint(&ptrZ);
//         // delete_bint(&ptrTmpX);
//         // delete_bint(&ptrTmpY);
//         // delete_bint(&ptrTmpZ);
//         // delete_bint(&ptrTTmpX);
//         // delete_bint(&ptrTTmpY);
//         // delete_bint(&ptrTTmpZ);
//         printf("%.6f\n", elapsed_time*1000);
//         // printf("%.0f\n", cpu_time_used2);
//         // printf("%.0f\n", cpu_time_used3);
//         // printf("%.6f\n", cpu_time_used1-cpu_time_used2);
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