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
    int t = 3;

    // printf("a: %08x\n b: %08x\n", a, b);
    // mul_xyz(a,b,c);
    // printf("%08x %08x\n", c[1], c[0]);

    while(t != 0) {
        BINT* bint1 = NULL; 
        BINT* bint2 = NULL;
        BINT* bint3 = NULL;
        // int n = rand() % 0x02  + 1;
        // int m = rand() % 0x02  + 1;
        int n = 3;
        int m = 1;
        int max = MAX(n, m);
        rand_bint(&bint1, false, n);
        rand_bint(&bint2, false, m);
        bint3 = init_bint(&bint3, 2*max);
        // add_xyz(bint1, bint1, bint2);
        mul_xyz(bint1->val[0], bint1->val[0], &bint3);
        // mul_core_ImpTxtBk_xyz(bint1, bint2, &bint3);

        // print(int(hex(0x06dbb859 * 0xa38fb144), 16) == int("0x0461bfdc618980a4", 16))
        // printf("print(int(hex(");
        // printHex2(bint1);printf("*");printHex2(bint2);
        // printf("), 16) == int(\"");
        // printHex2(bint3);printf("\", 16))\n");

        printHex(bint1);printf("\n");
        MUL_Shift(bint1, bint2, 32);
        printHex(bint2);printf("\n");

        // const char *hexData = "1A3F";  // Sample data
        // char binaryOutput[5 * strlen(hexData) + 1];  // Max potential output size (4 bits for each hex + null terminator)

        // hexToBinary(hexData, binaryOutput);
        // printf("Hex: %s\nBinary: %s\n", hexData, binaryOutput);

        delete_bint(&bint1);
        delete_bint(&bint2);
        delete_bint(&bint3);
        t--;
    }

}