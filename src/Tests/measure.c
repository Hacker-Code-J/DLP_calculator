#include "measure.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MEASURE_TIME(start, end) ((double)(end - start) / CLOCKS_PER_SEC)

void performBINT_2Arg(void (*testFunc)(BINT**, BINT**), BINT** pptrX, BINT** pptrZ) {
    clock_t start = clock();
    testFunc(pptrX, pptrZ);
    clock_t end = clock();
    printf("%.3f\n", MEASURE_TIME(start, end)*1000);
}
void performTEST_2Arg(void (*testFunc1)(BINT**, BINT**), void (*testFunc2)(BINT**, BINT**)) {
    srand((unsigned int)time(NULL));

    for (int idx = 0; idx < TEST_ITERATIONS; idx++) {
        int len = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;

        BINT *ptrX = NULL;
        BINT *ptrZ = NULL;
        BINT *ptrTmpX = NULL;
        BINT *ptrTmpZ = NULL;
        bool sgnX = rand() % 2;
        RANDOM_BINT(&ptrX, sgnX, len);
        copyBINT(&ptrTmpX, &ptrX);

        performBINT_2Arg(testFunc1, &ptrX, &ptrZ);
        performBINT_2Arg(testFunc2, &ptrTmpX, &ptrTmpZ);

        delete_bint(&ptrX);
        delete_bint(&ptrZ);
        delete_bint(&ptrTmpX);
        delete_bint(&ptrTmpZ);
    }
}


// void performBINT_4Arg(void (*testFunc)(BINT**, BINT**, BINT**, BINT**), BINT** pptrX, BINT** pptrY, BINT** pptrQ, BINT** pptrR) {
//     clock_t start = clock();
//     testFunc(pptrX, pptrY, pptrQ, pptrR);
//     clock_t end = clock();
//     printf("%.3f\n", MEASURE_TIME(start, end)*1000);
// }

// void performTEST_4Arg(void (*testFunc1)(BINT**, BINT**, BINT**, BINT**), void (*testFunc2)(BINT**, BINT**, BINT**, BINT**)) {
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