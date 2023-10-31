/*****************************************************************************************************************
 * BINT bint;
 * BINT* ptrBint = &bint;
 * BINT** pptrBint = &ptrBint;
 * +---------------+
 * |   pptrBint    |
 * |---------------|
 * |  [address]----|-----> +-----------+
 * +---------------+       |  ptrBint  |
 *                         |-----------|
 *                         | [address]-|-----> +----------------------+
 *                         +-----------+       |        bint          |
 *                                             |----------------------|
 *                                             | sign:    false(+)    |
 *                                             | wordlen: n           |
 *                                             | val:     [address]---|----> +---------+---------+---+-----------+
 *                                             +----------------------+      | WORD[0] | WORD[1] |...| WORD[n-1] |
 *                                                                           +---------+---------+---+-----------+
*****************************************************************************************************************/
#include "setup.h"

#include <stdbool.h>

#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#define WORD_BITLEN 32

// Supports operations in 8-bit, 16-bit, 32-bit, and 64-bit units.
#if WORD_BITLEN == 8
typedef u8 WORD;
#elif WORD_BITLEN == 64
typedef u64 WORD:
#else
typedef u32 WORD;
#endif

/**
 * Represents a large integer in binary format.
 */
typedef struct {
    bool sign;      // The sign of the number: false if 0 or positive, true if negative.
    int wordlen;    // The number of WORDs representing the value of the BINT.
    WORD* val;      // Pointer to the array of WORDs representing the value.
} BINT;

/**
 * allocate memory for the integer
 * @param bint_ptr  the point of BINT
 * @param wordlen   the initial size of the array
 * @return the integer pointer
 */
BINT* init_bint(BINT** pptrBint, int wordlen);

/**
 * Deletes the binary integer object pointed by the given pointer, and sets the pointer to NULL.
 * @param pptrBint Double pointer to the binary integer to be deleted.
 */
void delete_bint(BINT** pptrBint);

/**
 * Sets the value of the binary integer object to zero.
 * @param pptrBint Double pointer to the binary integer whose value is to be set.
 */
void SET_BINT_ZERO(BINT** pptrBint);
/**
 * Sets the value of the binary integer object to one.
 * @param pptrBint Double pointer to the binary integer whose value is to be set.
 */
void SET_BINT_ONE(BINT** pptrBint);
/**
 * Initializes the binary integer object with custom zero value based on the number of words.
 * @param pptrBint  Double pointer to the binary integer to be initialized.
 * @param num_words Number of words for custom zero initialization.
 */
void SET_BINT_CUSTOM_ZERO(BINT** pptrBint, int num_words);

/**
 * Copies the value from the source binary integer object to the destination binary integer object.
 * @param pptrBint_dst Double pointer to the destination binary integer object.
 * @param pptrBint_src Double pointer to the source binary integer object.
 */
void copyBINT(BINT** pptrBint_dst, BINT** pptrBint_src);

void swapBINT(BINT** ptrbint1, BINT** ptrbint2);

/**
 * Modifies the binary integer to be an even number, if it isn't already.
 * @param ptrBint Pointer to the binary integer to be modified.
 */
void makeEven(BINT* ptrBint);

/**
 * Adjusts the sizes of two binary integer objects to be the same, typically by padding the smaller one.
 * @param ptrBint1 Pointer to the first binary integer object.
 * @param ptrBint2 Pointer to the second binary integer object.
 */
void matchSize(BINT* ptrBint1, BINT* ptrBint2);

/**
 * Resets the provided BINT structure, setting its value to zero and cleaning any resources.
 * @param ptrBint Pointer to the BINT that is to be reset.
 */
void reset_bint(BINT* ptrBint);

/**
 * Determines if the binary integer object is one.
 * @param bint Constant pointer to the binary integer object.
 * @return true if the binary integer is one, false otherwise.
 */
bool isZero(BINT* ptrbint);
/**
 * Determines if the binary integer object is one.
 * @param bint Constant pointer to the binary integer object.
 * @return true if the binary integer is one, false otherwise.
 */
bool isOne(BINT* ptrbint);

WORD hexSubstringToWord(const char* str, int start, int length);
void strToBINT(BINT** pptrBint, const char* hexString);

/**
 * Converts a hex character to its binary representation.
 * @param hex        The hex character to be converted.
 * @param output     Pointer to the output char array to store the binary representation.
 */
void hexCharToBinary(char hex, char* output);
/**
 * Converts a hex string to its binary representation.
 * @param hex           Pointer to the hex string to be converted.
 * @param binaryOutput  Pointer to the output char array to store the binary representation.
 */
void hexToBinary(const char* hex, char* binaryOutput);

/**
 * Stores a BINT structure to a file.
 * @param filename The name of the file where the BINT should be stored.
 * @param b        Pointer to the BINT to be stored.
 * @return True if the operation was successful, false otherwise.
 */
bool store_bint(const char* filename, BINT* b);
/**
 * Stores multiple BINT structures to a file.
 * @param filename  The name of the file where the BINTs should be stored.
 * @param bint_array Pointer to an array of BINT pointers to be stored.
 * @param num_bints The number of BINT structures in the array.
 * @return True if the operation was successful, false otherwise.
 */
bool multi_store_bints(const char* filename, BINT** bint_array, int num_bints);
/**
 * Loads a BINT structure from a file.
 * @param filename The name of the file from which the BINT should be loaded.
 * @return A pointer to the loaded BINT structure or NULL if there was an error.
 */
BINT* load_bint(const char* filename);
/**
 * Loads multiple BINT structures from a file.
 * @param filename  The name of the file from which the BINTs should be loaded.
 * @param num_bints Pointer to an integer where the number of loaded BINTs will be stored.
 * @return A pointer to an array of BINT pointers containing the loaded BINTs or NULL if there was an error.
 */
BINT** multi_load_bints(const char* filename, int* num_bints);


// typedef struct Node {
//     BINT data;
//     struct Node* next;
// } Node;

// typedef struct {
//     Node* front;
//     Node* rear;
// } BINTQueue;

// BINTQueue* createQueue();
// void enqueue(BINTQueue* q, BINT data);
// BINT dequeue(BINTQueue* q);
// BINT peek(BINTQueue* q);
// bool isempty(BINTQueue* q);
// void freeQueue(BINTQueue* q);

///////////////////////////////////////////////////////////////////////////////////////

void printHex(BINT* X);
void printHex2(BINT* X);


/**********************************************************************
* x: [sgn] 0x ######## ######## 
* y: [sgn] 0x ######## ######## 
**********************************************************************/
void custom_printHex_xy(BINT* X, BINT* Y, int n);

/**********************************************************************
*   [sgn] 0x ######## ######## 
* @ [sgn] 0x ######## ######## 
* ----------------------------
*   [sgn] 0x ######## ########
*
* @:option - 0: '+' | 1: '-' | 2: '*'
**********************************************************************/
void custom_printHex(BINT* X, BINT* Y, BINT* Z,  int option);

void refine_BINT(BINT* X);
void refine_BINT_word(BINT* ptrX, int num_words);

//Generate Random BINT

void rand_array(WORD* dst, int wordlen);
void rand_bint(BINT**x, int sign, int wordlen);

//Copy Arrary
void array_copy_x2y(WORD* X, WORD* Y, int wordlen);

//Assign Y<-X
void assgin_x2y(BINT* X, BINT** Y);

//Compare
// Return values:
//  1 if X >= Y
//  0 if X < Y
bool compare_abs_bint(BINT** pptrX, BINT** pptrY);
int compare_bint(const BINT* a, const BINT* b);

//Author: Moon Ye-chan
int Get_bitlen(BINT* x);
int Get_sign(BINT* x);
void Flip_sign(BINT* x);

int BIT_LENGTH(BINT** pptrBint);

// //Author: Kim Ye-chan
// void DIV_Shift(BINT* bigint, BINT* result, int N);// DIV shift  
// void MUL_Shift(BINT* bigint, BINT* result , int N); // MUL shift

void FLIP_SIGN(BINT** pptrBint);

/** SHIFT and Reduction
 * 
*/
void left_shift(BINT** pptrbint, int num_bits);
void right_shift(BINT** pptrX, int num_bits);

void left_shift_word(BINT** pptrX, int shift_amount);
void right_shift_word(BINT** pptrBint, int shift_amount);

void reduction(BINT** pptrBint, int pwOf2);

#endif // BIGINTEGER_H