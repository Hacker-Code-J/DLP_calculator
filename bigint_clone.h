#include "setup.h"

#include <stdbool.h>

#ifndef BIGINTEGER_H
#define BIGINTEGER_H

// Define the size of a word in bits for the BigInteger implementation
#define WORD_BITLEN 32

// Conditional compilation based on the word size; this allows for flexibility in the data type of WORD
#if WORD_BITLEN == 8
typedef u8 WORD;        // If word length is 8 bits, define WORD as an unsigned 8-bit integer
#elif WORD_BITLEN == 64
typedef u64 WORD;       // If word length is 64 bits, define WORD as an unsigned 64-bit integer
#else
typedef u32 WORD;       // Default to defining WORD as an unsigned 32-bit integer
#endif

// Define a structure for representing big integers
typedef struct {
    bool sign;           // Sign bit: true for negative numbers, false for non-negative numbers
    int wordlen;         // The number of words that constitute the big integer
    WORD* val;           // Pointer to an array of words that represent the integer's value
} BINT;

void delete_bint(BINT** pptrBint);
void init_bint(BINT** pptrBint, int wordlen);

void SET_BINT_ZERO(BINT** pptrBint);
void SET_BINT_ONE(BINT** pptrBint);

void copyBINT(BINT** pptrBint_dst, const BINT* ptrBint_src);
void swapBINT(BINT** ptrBint1, BINT** ptrBint2);

void makeEven(BINT* ptrBint);
void matchSize(BINT* ptrBint1, BINT* ptrBint2);
void reset_bint(BINT* ptrBint);

WORD hexSubstringToWord(const char* str, int start, int length);
void strToBINT(BINT** pptrBint, const char* hexString);