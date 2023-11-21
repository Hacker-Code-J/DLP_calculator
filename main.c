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
    double cpu_time_used1;
    // clock_t start2, end2;
    // double cpu_time_used2;
    int t = 10000;

    /**
     * if WORD_BITLEN = 32,
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

        // BINT* ptrQ = NULL;
        // BINT* ptrR = NULL;
/*************************** Random Input **************************************/
        // int len1 = (rand() % 0x010) + 0x010; //  512 ~ 1024 bits
        // int len2 = (rand() % 0x010) + 0x010; //  512 ~ 1024 bits
        // int len1 = (rand() % 0x020) + 0x020; // 1024 ~ 2048 bits
        // int len2 = (rand() % 0x020) + 0x020; // 1024 ~ 2048 bits
        // int len1 = (rand() % 0x020) + 0x040; // 2048 ~ 3072 bits
        // int len2 = (rand() % 0x020) + 0x040; // 2048 ~ 3072 bits
        // int len1 = (rand() % 0x090) + 0x060; // 3072 ~ 7680 bits
        // int len2 = (rand() % 0x090) + 0x060; // 3072 ~ 7680 bits
        // int len1 = (rand() % 0xf0) + 0x0f0; // 7680 ~ 15360 bits
        // int len2 = (rand() % 0xf0) + 0x0f0; // 7680 ~ 15360 bits
        
        int len1 = (rand() % 0x05) + 0x05;
        int len2 = (rand() % 0x05) + 0x05;
        
        // int len1 = 0x04;
        // int len2 = 0x08;
        
        RANDOM_BINT(&ptrX, false, len1);
        RANDOM_BINT(&ptrY, false, len2);
        // init_bint(&ptrZ, len1 + len2);
        // init_bint(&ptrQ, len1);
        // init_bint(&ptrR, len2);
        // printf("X(%x):", len1);print_bint_hex(ptrX);
        // printf("Y(%x):", len2);print_bint_hex(ptrY);
        
        // int sgn1 = rand() % 0x02;
        // int sgn2 = rand() % 0x02;
        // RANDOM_BINT(&ptrX, sgn1, len1);
        // RANDOM_BINT(&ptrY, sgn2, len2);
     
        // copyBINT(&ptrTmpX, &ptrX);      
        // copyBINT(&ptrTmpY, &ptrY);
/*******************************************************************************/       
/*************************** Non-Random Input **************************************/
        // const char* ptrTestX = "0xe6a29ab895e3d2fc3e8178be0d8b5dfb5482379e4e92abd9130f20265f81f22d0db7e698";
        // const char* ptrTestY = "0xa551847f73ed611236dbc3b634a5f3757f9cb9ecb09f3431733b477512951425dd971e61";
        // const char* ptrTestX = "0x44b7014fff077a1eb414a9dc3694ee5d";
        // const char* ptrTestY = "0x8e91703149a154b4b2a6faf2ce5b3f93";
        // const char* ptrTestX = "0xae035de48a0ca938";
        // const char* ptrTestY = "0x0000000010a85e3a";
        // const char* ptrTestX = "0x909440cbc3ff0daf2b328f80c9a99a8d924766b6eea3e28f0d839cef3bac21c5d141be14d4db1faa69e6e63c175326db59c3d45f965ff31e47efce76dbdf06bc6de3f5297239c42dcf93153bfa6307f92d756c78170350d114ee701f7223c74471bc65456931179499b2340035918db55e692846ef6161f57c4d572eb10bdf2c52332dd598655f987da3738033248d1779c6e5fae5149d4e9b4c4f85b9abff7d";
        // const char* ptrTestY = "0x7e2d7f3602a84bba5107d3c130b79587713d9688f013dae5dd0c243396a224482ad7eb485744e790d71c2ba94558f0e94d83ae0b36cec0aa75820e4e784d5de280a760c76dcb642ed6d3b85c0b6d3b414ce04b8c583e9b270ac1f195cdb682b93a95623ff591757ac84b20c9215bf6c0870cf13134da3574f33b13147ced56fba0d1c97ac82a5aff2ac5d361d4eacf56d816fcc0e7fa61dfbf26be814d853125099f20c900952cc9f1e9927e12b85a182f025f083a3a213ed9394f277c92053f9e8b9d37a4433773d7d924fa8958b927c724e7fc61e8efb2bdbd223cf479a267e75a00316c5d4fdd50cc4161";
        // strToBINT(&ptrX, ptrTestX);
        // strToBINT(&ptrY, ptrTestY);
        // int len1 = ptrX->wordlen;
        // int len2 = ptrY->wordlen; 
        // printf("X(%x):", len1);print_bint_hex(ptrX);
        // printf("Y(%x):", len2);print_bint_hex(ptrY);
/***********************************************************************************/

        // if (ptrX->wordlen < ptrY->wordlen) swapBINT(&ptrX,&ptrY);
        // printf("Test[%d]---------------------------------------------------\n", idx+1);
        // printf("len1 len2 : %d %d\n", len1, len2);
        // custom_printHex_xy(ptrX, ptrY, MAX(len1,len2));
        // custom_printHex_xy(ptrX, ptrY, MIN(len1,len2));
        // custom_printHex_xy(ptrX, ptrY, len1+len2);

        // print_bint_hex_split(ptrX);
        // print_bint_hex_split(ptrY);
        start1 = clock();
        // add_core_xyz(&ptrX,&ptrY,&ptrZ);
        // ADD(&ptrX,&ptrY,&ptrZ);
        // sub_core_xyz(&ptrX,&ptrY,&ptrZ);
        // SUB(&ptrX,&ptrY,&ptrZ);
        // mul_xyz(ptrX->val[0], ptrY->val[0], &ptrZ);  // len1 = 1 = len2
        // mul_core_TxtBk_xyz(&ptrX,&ptrY,&ptrZ);
        // mul_core_ImpTxtBk_test(&ptrX,&ptrY,&ptrZ);
        // MUL_Core_ImpTxtBk_xyz(&ptrX,&ptrY,&ptrZ);
        // mul_core_Krtsb_test(&ptrX,&ptrY,&ptrZ);
        Krtsb_FLAG_Test(&ptrX,&ptrY,&ptrZ, 0x8);
        // MUL_Core_Krtsb_xyz(&ptrX,&ptrY,&ptrZ);
        // DIV_Bianry_Long_Test(&ptrX, &ptrY, &ptrQ, &ptrR);
        // DIV_Bianry_Long(&ptrX, &ptrY, &ptrQ, &ptrR);
        // exp_Mongomery(&ptrX,&ptrY,&ptrZ);
        // bool* binaryX = HexToBinary(ptrX);
        // bool* binaryY = HexToBinary(ptrY);
        end1 = clock();
        cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;

        // start2 = clock();
        // MUL_Core_ImpTxtBk_xyz(&ptrTmpX,&ptrTmpY,&ptrTmpZ);
        // end2 = clock();
        // cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;

        // start3 = clock();
        // // MUL_Core_ImpTxtBk_xyz(&ptrTTmpX,&ptrTTmpY,&ptrTTmpZ);
        // Krtsb_FLAG_Test(&ptrTTmpX,&ptrTTmpY,&ptrTTmpZ, 3);
        // end3 = clock();
        // cpu_time_used3 = ((double) (end3 - start3)) / CLOCKS_PER_SEC;

        // custom_printHex(ptrX, ptrY, ptrZ, 0);
        // custom_printHex(ptrX, ptrY, ptrZ, 1);
        // custom_printHex(ptrX, ptrY, ptrZ, 2);
        // custom_printHex(ptrTmpX, ptrTmpY, ptrTmpZ, 2);

        // const char *hexData = "1A3F";  // Sample data
        // char binaryOutput[5 * strlen(hexData) + 1];  // Max potential output size (4 bits for each hex + null terminator)

        // hexToBinary(hexData, binaryOutput);
        // printf("Hex: %s\nBinary: %s\n", hexData, binaryOutput);

/*************************** Sage (or Python) Test **************************************/
        /** SAGE (ADD)
         * print(hex(0x00 + 0x00) == hex(0x00))
        */ 
        // printf("print(hex(");
        // if(ptrX->sign) printf("-");
        // printHex2(ptrX);printf(" + ");
        // if(ptrY->sign) printf("-");
        // printHex2(ptrY);
        // printf(") == hex(");
        // if(ptrZ->sign) printf("-");
        // printHex2(ptrZ);printf("))\n");
        
        /** SAGE (SUB).
         * print((hex(0x00 - 0x00) == hex(0x00))
        */
        // printf("print(hex(");
        // if(ptrX->sign) printf("-");
        // printHex2(ptrX);printf(" - ");
        // if(ptrY->sign) printf("-");
        // printHex2(ptrY);
        // printf(") == hex(");
        // if(ptrZ->sign) printf("-");
        // printHex2(ptrZ);printf("))\n");

        /** SAGE (MUL)
         * print(hex(0x00 * 0x00) == hex(0x00))
        */ 
        printf("print(hex(");
        print_bint_hex_python(&ptrX);
        printf(" * ");
        print_bint_hex_python(&ptrY);
        printf(") == hex(");
        print_bint_hex_python(&ptrZ);
        printf("))\n");
        
        /** SAGE (DIV)
         * print(hex(0x00 * 0x00 + 0x00) == hex(0x00))
        // */ 
        // printf("print(hex(");
        // print_bint_hex_python(&ptrQ);
        // printf(" * ");
        // print_bint_hex_python(&ptrY);
        // printf(" + ");
        // print_bint_hex_python(&ptrR);
        // printf(") == hex(");
        // print_bint_hex_python(&ptrX);
        // printf("))\n");

        /** SAGE (EXP)
         * print(hex(0x00 ** 0x00) == hex(0x00))
         * print(hex(power_mod(0x02, 0x03, 0x07)))
        */ 
        // printf("print(hex(");
        // print_bint_hex_python(&ptrX);
        // printf(" ** ");
        // print_bint_hex_python(&ptrY);
        // printf(") == hex(");
        // print_bint_hex_python(&ptrZ);
        // printf("))\n");
/****************************************************************************************/
        // printf("Result Q:");print_bint_hex(ptrQ);
        // printf("Result R:");print_bint_hex(ptrR);


        // printf("==================================================\n");
        
        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrZ);
        // delete_bint(&ptrTmpX);
        // delete_bint(&ptrTmpY);
        // delete_bint(&ptrTmpZ);
        // delete_bint(&ptrQ);
        // delete_bint(&ptrR);

        // delete_bint(&ptrX2);
        // delete_bint(&ptrY2);
        // printf("%.6f\n", cpu_time_used1);
        // printf("%.6f\n", cpu_time_used2);
        idx++;
    }
}

/* Measuring Performance*/
// int main() {
//     srand((unsigned int)time(NULL));

//     clock_t start1, end1;
//     double cpu_time_used1;
//     clock_t start2, end2;
//     double cpu_time_used2;
//     clock_t start3, end3;
//     double cpu_time_used3;
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
//         BINT* ptrTTmpX = NULL;
//         BINT* ptrTTmpY = NULL;
//         BINT* ptrTTmpZ = NULL;
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
        
//         // int len1 = (rand() % 0x08) + 0x3;
//         // int len2 = (rand() % 0x08) + 0x3;
        
//         int len1 = 0x0f0;
//         int len2 = 0x0f0;
        
//         // RANDOM_BINT(&ptrX, false, len1);
//         // RANDOM_BINT(&ptrY, false, len2);
        
//         // int sgn1 = rand() % 0x02;
//         // int sgn2 = rand() % 0x02;
//         RANDOM_BINT(&ptrX, false, len1);
//         RANDOM_BINT(&ptrY, false, len2);
     
//         copyBINT(&ptrTmpX, &ptrX);      
//         copyBINT(&ptrTmpY, &ptrY);
//         copyBINT(&ptrTTmpX, &ptrX);      
//         copyBINT(&ptrTTmpY, &ptrY);

//         start1 = clock();
//         mul_core_TxtBk_xyz(&ptrX,&ptrY,&ptrZ);
//         end1 = clock();
//         cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;

//         start2 = clock();
//         MUL_Core_ImpTxtBk_xyz(&ptrX,&ptrY,&ptrZ);
//         end2 = clock();
//         cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
        
//         start3 = clock();
//         Krtsb_FLAG_Test(&ptrX,&ptrY,&ptrZ, 0x08);
//         end3 = clock();
//         cpu_time_used3 = ((double) (end3 - start3)) / CLOCKS_PER_SEC;

//         delete_bint(&ptrX);
//         delete_bint(&ptrY);
//         delete_bint(&ptrZ);
//         delete_bint(&ptrTmpX);
//         delete_bint(&ptrTmpY);
//         delete_bint(&ptrTmpZ);
//         delete_bint(&ptrTTmpX);
//         delete_bint(&ptrTTmpY);
//         delete_bint(&ptrTTmpZ);
//         printf("%.6f\n", cpu_time_used1);
//         printf("%.6f\n", cpu_time_used2);
//         printf("%.6f\n", cpu_time_used3);
//         // printf("%.6f\n", cpu_time_used1-cpu_time_used2);
//         idx++;
//     }
// }

// /* FLAG TEST*/
// int main() {
//     srand((unsigned int)time(NULL));

//     clock_t start1, end1;
//     double cpu_time_used1;
//     clock_t start2, end2;
//     double cpu_time_used2;
//     int t = 10000;

//     /**
//      * if WORD_BITLEN = 32,
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
        
//         // int len1 = (rand() % 0x08) + 0x3;
//         // int len2 = (rand() % 0x08) + 0x3;
        
//         int len1 = 0x0f0;
//         int len2 = 0x0f0;
        
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
//         cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;

//         start2 = clock();
//         // FLAG = 2, 4, 8, 16, 32
//         Krtsb_FLAG_Test(&ptrX,&ptrY,&ptrZ, 0x20);
//         end2 = clock();
//         cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
        
//         delete_bint(&ptrX);
//         delete_bint(&ptrY);
//         delete_bint(&ptrZ);
//         delete_bint(&ptrTmpX);
//         delete_bint(&ptrTmpY);
//         delete_bint(&ptrTmpZ);
//         printf("%.6f\n", cpu_time_used1-cpu_time_used2);
//         idx++;
//     }
// }