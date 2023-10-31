/***************************************************************************
 * Project Name: PUBAO (P.A.N.D.A.'s Unbounded Big Arithmetic Operations)
 * Team Name: P.A.N.D.A. (Programmers Aspiring to Navigate Digital Arithmetic)
 * 
 * File Description: This file contains the definitions and implementations
 *                   for large integer arithmetic.
 * 
 * Author(s): Ji Yong-Hyeon, Kim Ye-chan, Moon Ye-chan, Yoo Geun-oh
 * Date Created: 2023-09-21
 * Last Modified: 2023-11-01
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

#include "setup.h"
#include "BigInteger.h"
#include "operation.h"

int main() {
    srand((unsigned int)time(NULL));

    clock_t start1, end1;
    clock_t start2, end2;
    double cpu_time_used1, cpu_time_used2;
    int t = 1;

    BINT* bint1 = NULL;
    BINT* bint2 = NULL;
    BINT* bint3 = NULL;
    
    /**
     * if WORD_BITLEN = 32,
     * 0x020 -> ( 2 * 16 =  32) -> (32 *  32 =  1024-bit)
     * 0x040 -> ( 4 * 16 =  64) -> (32 *  64 =  2048-bit)
     * 0x060 -> ( 6 * 16 =  96) -> (32 *  96 =  3072-bit)
     * 0x0f0 -> (15 * 16 = 240) -> (32 * 240 =  7680-bit)
     * 
     * 0x1e0 -> ( 1 * 256 + 14 * 16 = 480) -> (32 * 480 = 15360-bit)
    */
    int idx = 0;
    while(idx < t) {
        // int n = (rand() % 0xa) + 0x1;
        // int m = (rand() % 0x20) + 0x10;
        // int n = 0x6;
        // int m = 0x6;
        // rand_bint(&bint1, false, n);
        // rand_bint(&bint2, false, n);
        
        const char* testBint1 = "0x15d44e89bdbd1d12";
        const char* testBint2 = "0xfef804d8d2177e7c";
        strToBINT(&bint1, testBint1);
        strToBINT(&bint2, testBint2);
        int n = bint1->wordlen;
        int m = bint2->wordlen;

        // if (bint1->wordlen < bint2->wordlen) swapBINT(&bint1,&bint2);
        // printf("Test[%d]---------------------------------------------------\n", idx+1);
        // printf("n m : %d %d\n", n, m);
        // custom_printHex_xy(bint1, bint2, n+m);

        // printHex(bint1);printf("\n");
        // printHex(bint2);printf("\n");
        
        start1 = clock();
        // add_core_xyz(&bint1,&bint2,&bint3);
        sub_core_xyz(&bint1,&bint2,&bint3);
        // mul_core_ImpTxtBk_test(&bint1, &bint2, &bint3);
        // MUL_Core_ImpTxtBk_xyz(&bint1, &bint2, &bint3);
        // mul_core_Krtsb_test(&bint1, &bint2, &bint3);
        // MUL_Core_Krtsb_xyz(&bint1, &bint2, &bint3);
        end1 = clock();
        cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;

        // start2 = clock();
        // MUL_Core_ImpTxtBk_xyz(&bint1, &bint2, &bint3);
        // end2 = clock();
        // cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;

        // custom_printHex(bint1, bint2, bint3, 0);
        custom_printHex(bint1, bint2, bint3, 1);
        // custom_printHex(bint1, bint2, bint3, 2);

        // const char *hexData = "1A3F";  // Sample data
        // char binaryOutput[5 * strlen(hexData) + 1];  // Max potential output size (4 bits for each hex + null terminator)

        // hexToBinary(hexData, binaryOutput);
        // printf("Hex: %s\nBinary: %s\n", hexData, binaryOutput);

        /** SAGE (ADD)
         * print(int(hex(0x00 + 0x00), 16) == int("0x00", 16))
        */ 
        // printf("print(int(hex(");
        // printHex2(bint1);printf(" + ");printHex2(bint2);
        // printf("), 16) == int(\"");
        // printHex2(bint3);printf("\", 16))\n");

        /** SAGE (SUB)
         * print(int(hex(0x00 - 0x00), 16) == int("0x00", 16))
        */ 
        // printf("print(int(hex(");
        // printHex2(bint1);printf(" - ");printHex2(bint2);
        // printf("), 16) == int(\"");
        // printHex2(bint3);printf("\", 16))\n");
        
        /** SAGE (SUB) abs ver (sub core test).
         * print(int(hex(abs(0x00 - 0x00)), 16) == int("0x00", 16))
        */
        // printf("print(int(hex(abs(");
        // if(bint1->sign) printf("-");
        // printHex2(bint1);printf(" - ");
        // if(bint2->sign) printf("-");
        // printHex2(bint2);
        // printf(")), 16) == int(\"");
        // if(bint3->sign) printf("-");
        // printHex2(bint3);printf("\", 16))\n");
        bool xGeqy = compare_abs_bint(&bint1, &bint2);
        printf("print(int(hex(");
        if(bint1->sign) printf("-");
        printHex2(bint1);printf(" - ");
        if(bint2->sign) printf("-");
        printHex2(bint2);
        printf("), 16) == int(\"");
        if(!xGeqy) printf("-");
        printHex2(bint3);printf("\", 16))\n");

        /** SAGE (MUL)
         * print(int(hex(0x00 * 0x00), 16) == int("0x00", 16))
        */ 
        // printf("print(int(hex(");
        // printHex2(bint1);printf(" * ");printHex2(bint2);
        // printf("), 16) == int(\"");
        // printHex2(bint3);printf("\", 16))\n");

        delete_bint(&bint1);
        delete_bint(&bint2);
        delete_bint(&bint3);
        // printf("%.8f\n", cpu_time_used);
        // printf("%.8f\n", cpu_time_used);
        idx++;
    }
}