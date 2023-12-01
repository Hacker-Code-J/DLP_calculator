#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "arithmetic.h"

// Configuration Macros
#define TEST_ITERATIONS 10000
#define MIN_BIT_LENGTH 32 // 1024-bit
#define MAX_BIT_LENGTH 64 // 2048-bit

int main() {
    printf("Test\n");
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