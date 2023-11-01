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
    int t = 10000;

    BINT* ptrX = NULL;
    BINT* ptrY = NULL;
    BINT* ptrZ = NULL;

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
/*************************** Random Input **************************************/
        // int sgn1 = rand() % 0x02;
        // int sgn2 = rand() % 0x02;
        int len1 = (rand() % 0x060) + 0x040;
        int len2 = (rand() % 0x060) + 0x040;
        // int len1 = 0x6;
        // int len2 = 0x6;
        RANDOM_BINT(&ptrX, false, len1);
        RANDOM_BINT(&ptrY, false, len2);
        // RANDOM_BINT(&bint1, sgn1, len1);
        // RANDOM_BINT(&bint2, sgn2, len2);      
/*******************************************************************************/       
/*************************** Specific Input **************************************/
        // const char* testBint1 = "0xd07a7eb3448033bddeba770afea9c37ee1a1047b2e78b1ff";
        // const char* testBint2 = "0x296cfc6c8e42f584434cc1c56b20c1f0f1493ef22d5fb7e0";
        // strToBINT(&bint1, testBint1);
        // strToBINT(&bint2, testBint2);
        // int n = bint1->wordlen;
        // int m = bint2->wordlen;          
/*********************************************************************************/

        // if (bint1->wordlen < bint2->wordlen) swapBINT(&bint1,&bint2);
        // printf("Test[%d]---------------------------------------------------\n", idx+1);
        // printf("n m : %d %d\n", n, m);
        // custom_printHex_xy(bint1, bint2, MAX(n,m));
        // custom_printHex_xy(bint1, bint2, n+m);

        // printHex(bint1);printf("\n");
        // printHex(bint2);printf("\n");
        
        // exit_on_null_error(ptrX, "ptrX", "add_core_xyz");
        // exit_on_null_error(ptrY, "ptrY", "add_core_xyz");
        // int n = ptrX->wordlen; int m = ptrY->wordlen;

        // init_bint(&ptrZ, n+1);
        // CHECK_PTR_AND_DEREF(&ptrZ, "pptrZ", "add_core_xyz");
        // BINT* ptrZ = &ptrZ;

        // WORD res = 0x00;
        // WORD carry = 0x00;
        // // WORD* res = NULL;
        // // WORD k = 0x00;
        
        // int i;
        // for (i = 0; i < m; i++) {
        //     // printf("Before: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res);
        //     add_carry(ptrX->val[i], ptrY->val[i], &carry, &res);
        //     (*ptrZ)->val[i] = res;
        //     // printf("-After: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x, Z[%d]: %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res,i,ptrZ->val[i]);
        //     // k = carry;
        // }
        // for (i = m; i < n; i++) {
        //     // printf("Before: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res);    
        //     add_carry(ptrX->val[i], 0, &carry, &res);
        //     // printf("-After: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x, Z[%d]: %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res, i, ptrZ->val[i]);
        //     (*pptrZ)->val[i] = res;
        //     // k = carry;
        // }
        // if(carry) {
        //     (*pptrZ)->val[n] = carry;
        // } else {
        //     (*pptrZ)->wordlen = n;
        // }
        // refine_BINT(ptrX);
        // refine_BINT(ptrY);
        // refine_BINT(*pptrZ);

        start1 = clock();
        add_core_xyz(&ptrX,&ptrY,&ptrZ);
        // ADD(&bint1,&bint2,&bint3);
        // sub_core_xyz(&bint1,&bint2,&bint3);
        // SUB(&bint1,&bint2,&bint3);
        // mul_core_TxtBk_xyz(&bint1, &bint2, &bint3);
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
        // custom_printHex(bint1, bint2, bint3, 1);
        // custom_printHex(bint1, bint2, bint3, 2);

        // const char *hexData = "1A3F";  // Sample data
        // char binaryOutput[5 * strlen(hexData) + 1];  // Max potential output size (4 bits for each hex + null terminator)

        // hexToBinary(hexData, binaryOutput);
        // printf("Hex: %s\nBinary: %s\n", hexData, binaryOutput);

/*************************** Sage (or Python) Test **************************************/
        /** SAGE (ADD)
         * print(hex(0x00 + 0x00) == hex(0x00))
        */ 
        printf("print(hex(");
        if(ptrX->sign) printf("-");
        printHex2(ptrX);printf(" + ");
        if(ptrY->sign) printf("-");
        printHex2(ptrY);
        printf(") == hex(");
        if(ptrZ->sign) printf("-");
        printHex2(ptrZ);printf("))\n");
        
        /** SAGE (SUB).
         * print((hex(0x00 - 0x00) == hex(0x00))
        */
        // printf("print(hex(");
        // if(bint1->sign) printf("-");
        // printHex2(bint1);printf(" - ");
        // if(bint2->sign) printf("-");
        // printHex2(bint2);
        // printf(") == hex(");
        // if(bint3->sign) printf("-");
        // printHex2(bint3);printf("))\n");

        /** SAGE (MUL)
         * print(hex(0x00 * 0x00) == hex(0x00))
        */ 
        // printf("print(hex(");
        // if(bint1->sign) printf("-");
        // printHex2(bint1);printf(" * ");
        // if(bint2->sign) printf("-");
        // printHex2(bint2);
        // printf(") == hex(");
        // if(bint3->sign) printf("-");
        // printHex2(bint3);printf("))\n");
/****************************************************************************************/
 

        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrZ);
        // printf("%.8f\n", cpu_time_used);
        // printf("%.8f\n", cpu_time_used);
        idx++;
    }
}