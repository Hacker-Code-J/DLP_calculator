#include "measure.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MEASURE_TIME(start, end) ((double)(end - start) / CLOCKS_PER_SEC)

void performBINT_2ArgFn(void (*testFunc)(BINT**, BINT**), BINT** pptrX, BINT** pptrZ) {
    clock_t start = clock();
    testFunc(pptrX, pptrZ);
    clock_t end = clock();
    printf("%.3f\n", MEASURE_TIME(start, end)*1000);
}
void performTEST_2ArgFn(void (*testFunc1)(BINT**, BINT**), void (*testFunc2)(BINT**, BINT**)) {
    srand((u32)time(NULL));

    for (int idx = 0; idx < TEST_ITERATIONS; idx++) {
        int len = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;

        BINT *ptrX = NULL; BINT *ptrTmpX = NULL;
        BINT *ptrZ = NULL; BINT *ptrTmpZ = NULL;

        bool sgnX = rand() % 2;
        RANDOM_BINT(&ptrX, sgnX, len);
        copyBINT(&ptrTmpX, &ptrX);

        performBINT_2ArgFn(testFunc1, &ptrX, &ptrZ);
        performBINT_2ArgFn(testFunc2, &ptrTmpX, &ptrTmpZ);

        delete_bint(&ptrX);
        delete_bint(&ptrZ);
        delete_bint(&ptrTmpX);
        delete_bint(&ptrTmpZ);
    }
}

void performBINT_3ArgFn(void (*testFunc)(BINT**, BINT**, BINT**), BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    clock_t start = clock();
    testFunc(pptrX, pptrY, pptrZ);
    clock_t end = clock();
    printf("%.3f\n", MEASURE_TIME(start, end)*1000);
}
void performTEST_3ArgFn(void (*testFunc1)(BINT**, BINT**, BINT**), void (*testFunc2)(BINT**, BINT**, BINT**)) {
    srand((u32)time(NULL));

    for (int idx = 0; idx < TEST_ITERATIONS; idx++) {
        int len1 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;
        int len2 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;

        BINT *ptrX = NULL; BINT *ptrTmpX = NULL;
        BINT *ptrY = NULL; BINT *ptrTmpY = NULL;
        BINT *ptrZ = NULL; BINT *ptrTmpZ = NULL;
        
        // bool sgnX = rand() % 2;
        // bool sgnY = rand() % 2;
        bool sgnX = false;
        bool sgnY = false;
        RANDOM_BINT(&ptrX, sgnX, len1);
        RANDOM_BINT(&ptrY, sgnY, len2);
        copyBINT(&ptrTmpX, &ptrX);
        copyBINT(&ptrTmpY, &ptrY);

        performBINT_3ArgFn(testFunc1, &ptrX, &ptrY, &ptrZ);
        performBINT_3ArgFn(testFunc2, &ptrTmpX, &ptrTmpY, &ptrTmpZ);

        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrZ);
        delete_bint(&ptrTmpX);
        delete_bint(&ptrTmpY);
        delete_bint(&ptrTmpZ);
    }
}

void performBINT_4ArgFn(void (*testFunc)(BINT**, BINT**, BINT**, BINT**), BINT** pptrX, BINT** pptrY, BINT** pptrM, BINT** pptrN) {
    clock_t start = clock();
    testFunc(pptrX, pptrY, pptrM, pptrN);
    clock_t end = clock();
    printf("%.3f\n", MEASURE_TIME(start, end)*1000);
}
void performTEST_4ArgFn(void (*testFunc1)(BINT**, BINT**, BINT**, BINT**), void (*testFunc2)(BINT**, BINT**, BINT**, BINT**)) {
    srand((u32)time(NULL));
    clock_t start1, start2, end1, end2;

    BINT *ptrX = NULL; BINT *ptrTmpX = NULL;
    BINT *ptrY = NULL; BINT *ptrTmpY = NULL;
    BINT *ptrM = NULL; BINT *ptrTmpM = NULL;
    BINT *ptrN = NULL; BINT *ptrTmpN = NULL;
    bool sgnX = false;
    bool sgnY = false;
    // int cnt = TEST_ITERATIONS;
    int cnt = 10000;
    for (int idx = 0; idx < cnt; idx++) {
        int len1 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;
        int len2 = len1 - 1;
        // int len2 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;
        
        // bool sgnX = rand() % 2;
        // bool sgnY = rand() % 2;
        RANDOM_BINT(&ptrX, sgnX, len1);
        RANDOM_BINT(&ptrY, sgnY, len2);
        copyBINT(&ptrTmpX, &ptrX);
        copyBINT(&ptrTmpY, &ptrY);

        // performBINT_4ArgFn(testFunc1, &ptrX, &ptrY, &ptrM, &ptrN);
        // performBINT_4ArgFn(testFunc2, &ptrTmpX, &ptrTmpY, &ptrTmpM, &ptrTmpN);

        start1 = clock();
        testFunc1(&ptrX, &ptrY, &ptrM, &ptrN);
        end1 = clock();

        start2 = clock();
        testFunc2(&ptrTmpX, &ptrTmpY, &ptrTmpM, &ptrTmpN);
        end2 = clock();
        printf("%.3f\n", MEASURE_TIME(start1, end1)*1000);
        printf("%.3f\n", MEASURE_TIME(start2, end2)*1000);

        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrM);
        delete_bint(&ptrN);
        delete_bint(&ptrTmpX);
        delete_bint(&ptrTmpY);
        delete_bint(&ptrTmpM);
        delete_bint(&ptrTmpN);
    }
}

void performTEST_MUL() {
    srand((u32)time(NULL));

    for (int idx = 0; idx < TEST_ITERATIONS; idx++) {
        int len1 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;
        int len2 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;

        BINT *ptrX = NULL; BINT *ptrTmpX = NULL; BINT *ptrTTmpX = NULL;
        BINT *ptrY = NULL; BINT *ptrTmpY = NULL; BINT *ptrTTmpY = NULL;
        BINT *ptrZ = NULL; BINT *ptrTmpZ = NULL; BINT *ptrTTmpZ = NULL;
        
        // bool sgnX = rand() % 2;
        // bool sgnY = rand() % 2;
        bool sgnX = false;
        bool sgnY = false;
        RANDOM_BINT(&ptrX, sgnX, len1);
        RANDOM_BINT(&ptrY, sgnY, len2);
        copyBINT(&ptrTmpX, &ptrX); copyBINT(&ptrTTmpX, &ptrX);
        copyBINT(&ptrTmpY, &ptrY); copyBINT(&ptrTTmpY, &ptrY);

        performBINT_3ArgFn(mul_core_TxtBk_xyz, &ptrX, &ptrY, &ptrZ);
        performBINT_3ArgFn(MUL_Core_ImpTxtBk_xyz, &ptrTmpX, &ptrTmpY, &ptrTmpZ);
        performBINT_3ArgFn(MUL_Core_Krtsb_xyz, &ptrTTmpX, &ptrTTmpY, &ptrTTmpZ);

        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrZ);
        delete_bint(&ptrTmpX);
        delete_bint(&ptrTmpY);
        delete_bint(&ptrTmpZ);
        delete_bint(&ptrTTmpX);
        delete_bint(&ptrTTmpY);
        delete_bint(&ptrTTmpZ);
    }
}

void performTEST_DIV(int cnt) {
    srand((u32)time(NULL));
    int i = cnt;
    performTEST_4ArgFn(DIV_Binary_Long, DIV_Long);
    // for (int idx = 0; idx < cnt; idx++) {
    //     // int len1 = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;
    //     // int len2 = len1 - 1;
        
    //     // BINT *ptrX = NULL; BINT *ptrTmpX = NULL;
    //     // BINT *ptrY = NULL; BINT *ptrTmpY = NULL;
    //     // BINT *ptrM = NULL; BINT *ptrTmpM = NULL;
    //     // BINT *ptrN = NULL; BINT *ptrTmpN = NULL;
        
    //     // // bool sgnX = rand() % 2;
    //     // // bool sgnY = rand() % 2;
    //     // bool sgnX = false;
    //     // bool sgnY = false;
    //     // RANDOM_BINT(&ptrX, sgnX, len1);
    //     // RANDOM_BINT(&ptrY, sgnY, len2);
    //     // copyBINT(&ptrTmpX, &ptrX);
    //     // copyBINT(&ptrTmpY, &ptrY);

    //     // performBINT_4ArgFn(DIV_Binary_Long, &ptrX, &ptrY, &ptrM, &ptrN);
    //     // performBINT_4ArgFn(DIV_Long, &ptrTmpX, &ptrTmpY, &ptrTmpM, &ptrTmpN);

    //     // delete_bint(&ptrX);
    //     // delete_bint(&ptrY);
    //     // delete_bint(&ptrM);
    //     // delete_bint(&ptrN);
    //     // delete_bint(&ptrTmpX);
    //     // delete_bint(&ptrTmpY);
    //     // delete_bint(&ptrTmpM);
    //     // delete_bint(&ptrTmpN);
    // }
}

void test_rand_BarrettRed() {
    srand((u32)time(NULL));
    int idx = 0x00;

    // int cnt = TEST_INTERATOINS;
    int cnt = 1000;
    while(idx < cnt) {
        int len = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;

        BINT* ptrX = NULL;
        BINT* ptrN = NULL;
        BINT* ptrR = NULL;

        // bool sgnX = rand() % 2;
        // bool sgnY = rand() % 2;
        bool sgnX = false;
        bool sgnN = false;
        RANDOM_BINT(&ptrX, sgnX, 2*len);
        RANDOM_BINT(&ptrN, sgnN, len);

        Barrett_Reduction_TEST(&ptrX,&ptrN,&ptrR);

        printf("print(("); print_bint_hex_py(ptrX);
        printf(" %% "); print_bint_hex_py(ptrN);
        printf(") == "); print_bint_hex_py(ptrR);
        printf(")\n");

        delete_bint(&ptrX);
        delete_bint(&ptrN);
        delete_bint(&ptrR);
        idx++;
    }
}

void performFastRed() {
    srand((u32)time(NULL));
    clock_t start1, start2, end1, end2;
    
    // int cnt = TEST_ITERATIONS;
    int cnt = 1000;
    for (int idx = 0; idx < cnt; idx++) {
        int len = rand() % (MAX_BIT_LENGTH - MIN_BIT_LENGTH + 1) + MIN_BIT_LENGTH;

        BINT *ptrX = NULL; BINT *ptrTmpX = NULL;
        BINT *ptrY = NULL; BINT *ptrTmpY = NULL;
        BINT *ptrZ = NULL; BINT *ptrTmpZ = NULL;
        BINT *ptrN = NULL;

        // bool sgnX = rand() % 2;
        // bool sgnY = rand() % 2;
        bool sgnX = false;
        bool sgnY = false;
        RANDOM_BINT(&ptrX, sgnX, 2*len);
        RANDOM_BINT(&ptrY, sgnY, len);
        copyBINT(&ptrTmpX, &ptrX);
        copyBINT(&ptrTmpY, &ptrY);

        // Pre-computation
        BINT* W = NULL;
        BINT* temp = NULL;
        BINT* preT = NULL;
        BINT* Y = NULL;
        copyBINT(&Y,&ptrY);
        init_bint(&W,(ptrX)->wordlen+1);
        W->val[(ptrX)->wordlen] = WORD_ONE;
        DIV_Binary_Long(&W,&Y,&preT,&temp);

        start1 = clock();
        DIV_Binary_Long(&ptrX, &ptrY, &ptrZ, &ptrN);
        end1 = clock();
        start2 = clock();
        Barrett_Reduction(&ptrTmpX, &ptrTmpY, &ptrTmpZ, &preT);
        end2 = clock();

        printf("%.3f\n", MEASURE_TIME(start1, end1)*1000);
        printf("%.3f\n", MEASURE_TIME(start2, end2)*1000);

        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrZ);
        delete_bint(&ptrN);
        delete_bint(&ptrTmpX);
        delete_bint(&ptrTmpY);
        delete_bint(&ptrTmpZ);

        delete_bint(&W);
        delete_bint(&temp);
        delete_bint(&preT);
        delete_bint(&Y);
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