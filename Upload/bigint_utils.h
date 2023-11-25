#include "config.h"

#include <stdbool.h>

#ifndef BIGINT_UTILS_H
#define BIGINT_UTILS_H

void exit_on_null_error(const void* ptr, const char* ptr_name, const char* function_name);

void delete_bint(BINT** pptrBint);
void init_bint(BINT** pptrBint, int wordlen);

void SET_BINT_ZERO(BINT** pptrBint);
void SET_BINT_ONE(BINT** pptrBint);

void copyBINT(BINT** pptrBint_dst, const BINT* ptrBint_src);
void swapBINT(BINT** ptrBint1, BINT** ptrBint2);

void makeEven(BINT* ptrBint);
void matchSize(BINT* ptrBint1, BINT* ptrBint2);
void resetBINT(BINT* ptrBint);
void refineBINT(BINT* ptrBint);

bool isZero(BINT* ptrBint);
bool isOne(BINT* ptrBint);

bool GET_BIT(BINT** pptrBint, int i_th);

void RANDOM_ARRARY(WORD* dst, int wordlen);
void RANDOM_BINT(BINT** pptrBint, bool sign, int wordlen);

bool compare_bint(BINT* pptrBint1, BINT* pptrBint2);
bool compare_abs_bint(BINT* pptrBint1, BINT* pptrBint2);

int BIT_LENGTH(BINT* pptrBint);
int BIT_LENGTH_NONZERO(BINT* pptrBint);

void left_shift_word(BINT** pptrBint, int shift_amount);
void right_shift_word(BINT** pptrBint, int shift_amount);
void left_shift_bit(BINT** pptrBint, int shift_amount);
void left_right_bit(BINT** pptrBint, int shift_amount);
void reduction(BINT** pptrBint, int pwOf2);

WORD hexSubstringToWord(const char* str, int start, int length);
void strToBINT(BINT** pptrBint, const char* hexString);

void HexDigitToBinary(WORD hex_digit, bool *binary, int start_index, int bits);
bool* HexToBinary(BINT* hex);
void PrintBinary(bool* binary, int length);

WORD BinaryToHexDigit(bool *binary, int start_index, int bits);
BINT* BinaryToHex(bool *binary, int length);

void print_bint_bin(const BINT* ptrBint);
void print_bint_hex(const BINT* ptrBint);
void print_bint_bin_split(const BINT* ptrBint);
void print_bint_hex_split(const BINT* ptrBint);
void print_bint_bin_py(const BINT* ptrBint);
void print_bint_hex_py(const BINT* ptrBint);

#endif // BIGINT_UTILS_H