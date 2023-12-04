#include "../utils.h"

#ifndef _MEASURE_H
#define _MEASURE_H

// Configuration Macros
#define TEST_ITERATIONS 10000
#define MAX_BIT_LENGTH 64
#define MIN_BIT_LENGTH 32

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

void performBINT_2Arg(void (*testFunc)(BINT**, BINT**), BINT** pptrX, BINT** pptrZ);
void performTEST_2Arg(void (*testFunc1)(BINT**, BINT**), void (*testFunc2)(BINT**, BINT**));
void performBINT_3Arg(void (*testFunc)(BINT**, BINT**, BINT**), BINT** pptrX, BINT** pptrY, BINT** pptrZ);
void performTEST_3Arg(void (*testFunc1)(BINT**, BINT**, BINT**), void (*testFunc2)(BINT**, BINT**, BINT**));
void performBINT_4Arg(void (*testFunc)(BINT**, BINT**, BINT**, BINT**), BINT** pptrX, BINT** pptrY, BINT** pptrQ, BINT** pptrR);
void performTEST_4Arg(void (*testFunc1)(BINT**, BINT**, BINT**, BINT**), void (*testFunc2)(BINT**, BINT**, BINT**, BINT**));

#endif // _MEASURE_H