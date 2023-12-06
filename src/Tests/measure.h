#include "../arithmetic.h"

#ifndef _MEASURE_H
#define _MEASURE_H

// Define Macros for Bit Lengths based on 8-bit word units
#define u8_BIT_1024 0x080  // 128 * 8 = 1024 bits
#define u8_BIT_2048 0x100  // 256 * 8 = 2048 bits
#define u8_BIT_3072 0x180  // 384 * 8 = 3072 bits
#define u8_BIT_7680 0x3C0  // 960 * 8 = 7680 bits

// Define Macros for Bit Lengths based on 32-bit word units
#define u32_BIT_1024 0x20  // 032 * 32 = 1024 bits
#define u32_BIT_2048 0x40  // 064 * 32 = 2048 bits
#define u32_BIT_3072 0x60  // 096 * 32 = 3072 bits
#define u32_BIT_7680 0xF0  // 240 * 32 = 7680 bits

// Define Macros for Bit Lengths based on 64-bit word units
#define u64_BIT_1024 0x10  // 016 * 64 = 1024 bits
#define u64_BIT_2048 0x20  // 032 * 64 = 2048 bits
#define u64_BIT_3072 0x30  // 048 * 64 = 3072 bits
#define u64_BIT_7680 0x78  // 120 * 64 = 7680 bits

// Configuration Macros
#define TEST_ITERATIONS 10000
#define MAX_BIT_LENGTH u32_BIT_3072
#define MIN_BIT_LENGTH u32_BIT_2048

void performBINT_2ArgFn(void (*testFunc)(BINT**, BINT**), BINT** pptrX, BINT** pptrZ);
void performTEST_2ArgFn(void (*testFunc1)(BINT**, BINT**), void (*testFunc2)(BINT**, BINT**));
void performBINT_3ArgFn(void (*testFunc)(BINT**, BINT**, BINT**), BINT** pptrX, BINT** pptrY, BINT** pptrZ);
void performTEST_3ArgFn(void (*testFunc1)(BINT**, BINT**, BINT**), void (*testFunc2)(BINT**, BINT**, BINT**));
void performBINT_4ArgFn(void (*testFunc)(BINT**, BINT**, BINT**, BINT**), BINT** pptrX, BINT** pptrY, BINT** pptrM, BINT** pptrN);
void performTEST_4ArgFn(void (*testFunc1)(BINT**, BINT**, BINT**, BINT**), void (*testFunc2)(BINT**, BINT**, BINT**, BINT**));

void correctTEST_ADD(int test_cnt);
void performTEST_ADD(int test_cnt);

void correctTEST_SUB(int test_cnt);
void performTEST_SUB(int test_cnt);

void correctTEST_TxtBk(int test_cnt);
void correctTEST_ImpTxtBk(int test_cnt);
void correctTEST_Krtsb(int test_cnt);

void performTEST_MUL();
void performTEST_DIV(int cnt);
void test_rand_BarrettRed();
void performFastRed();

#endif // _MEASURE_H