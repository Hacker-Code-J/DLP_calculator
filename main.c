/***************************************************************************
 * Project Name: PUBAO (P.A.N.D.A.'s Unbounded Big Arithmetic Operations)
 * Team Name: P.A.N.D.A. (Programmers Aspiring to Navigate Digital Arithmetic)
 * 
 * File Description: This file contains the definitions and implementations
 *                   for large integer arithmetic.
 * 
 * Author(s): Ji Yong-Hyeon, Kim Ye-chan, Moon Ye-chan, Yoo Geun-oh
 * Date Created: 2023-09-21
 * Last Modified: 2023-10-25
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

    // clock_t start, end;
    int t = 1;

    // printf("a: %08x\n b: %08x\n", a, b);
    // mul_xyz(a,b,c);
    // printf("%08x %08x\n", c[1], c[0]);

    int idx = 0;
    while(idx < t) {
        // printf("\n-----[Test %d]-----\n\n", idx+1);
        BINT* bint1 = NULL; 
        BINT* bint2 = NULL;
        BINT* bint3 = NULL;
        // int n = rand() % 0x4  + 1;
        // int m = rand() % 0x4  + 1;
        int n = 4;
        int m = 4;
        // int max = MAX(n, m);
        rand_bint(&bint1, false, n);
        rand_bint(&bint2, false, m);
        custom_printHex_xy(bint1, bint2, n+m);
        // SET_BINT_CUSTOM_ZERO(&bint2, 3);
        // bint3 = init_bint(&bint3, max);
        // bint3 = init_bint(&bint3, n+m);

        // printHex(bint1);printf("\n");
        // printHex(bint2);printf("\n");
        // mul_xyz(bint1->val[0], bint2->val[0], &bint3);
        // ADD(&bint1, &bint2, &bint3);
        MUL_Core_ImpTxtBk(&bint1, &bint2, &bint3);
        // mul_core_ImpTxtBk_xyz(&bint1, &bint2, &bint3);
        // mul_core_TxtBk_xyz(&bint1, &bint2, &bint3);
        // makeEven(&bint1); makeEven(&bint2);
        // printHex(bint1);printf("\n");
        // printHex(bint2);printf("\n");
        // custom_printHex(bint1, bint2, bint3, 2);
        // printHex(bint3);printf("\n");
        // printHex(bint3);printf("\n");

        // const char *hexData = "1A3F";  // Sample data
        // char binaryOutput[5 * strlen(hexData) + 1];  // Max potential output size (4 bits for each hex + null terminator)

        // hexToBinary(hexData, binaryOutput);
        // printf("Hex: %s\nBinary: %s\n", hexData, binaryOutput);

        /** SAGE
         * print(int(hex(0x06dbb859 * 0xa38fb144), 16) == int("0x0461bfdc618980a4", 16))
        */
        // printf("print(int(hex(");
        // printHex2(bint1);printf("*");printHex2(bint2);
        // printf("), 16) == int(\"");
        // printHex2(bint3);printf("\", 16))\n");

        delete_bint(&bint1);
        delete_bint(&bint2);
        delete_bint(&bint3);
        idx++;
    }

}