#include <stdarg.h>
#include <stdbool.h>

#include "setup.h"

#ifndef _BIGINTEGER_H
#define _BIGINTEGER_H

#define WORD u32        // base used to represent each digit in the big integer
#define NO_BASE_DIGITS  // n

/**
 * Data structure for representing an Integer
 * Every integer in the array signifies a digit in base 2^w, where w = 8 * sizeof(WORD)
 * +---------------------+
 * |   BINT(128-bit)     |
 * +---------------------+
 * |    sign (4byte)     | --> NEGATIVE or NON_NEGATIVE
 * +---------------------+
 * |   wordlen (4byte)   | --> n-word BINT
 * +---------------------+
 * |    val (64byte)     | --> [addr] -->+---------+---------+---+---------+
 * +---------------------+               | WORD[0] | WORD[1] |...| WORD[n] |
 *                                       +---------+---------+---+---------+
*/
typedef struct {
    bool sign;      // the sign of the number (true if 0 or positive, false if negative)
    u32 wordlen;    //
    WORD* val;      // threshold between performing long and karatsuba multiplication
} BINT;

extern const BINT BINT_ZERO;    //zero integer
extern const BINT BINT_ONE;     //one integer
extern const BINT BINT_NEG_ONE; //negative one integer

/**
 * allocate memory for the integer
 * @param bint_ptr the point of BINT
 * @param wordlen the initial size of the array
 * @return the integer pointer
 */
BINT* init_bint(BINT** bint_ptr, int wordlen);

/**
 * 
*/
void delete_bint(BINT** bint_ptr);

/**
 * convert string to a big integer by parsing each digit
 * 
 * PROCESS
 * for each block of 9 base 10 digits
 * val = digit[8] * 10^8 + digit[7] * 10^7 + ... + digit[0] + 10^0
 *     = ((digit[8] * 10 + digit[7]) * 10 + ... ) * 10 + digit[0]
 * 
 * @param str the string
 * @return the integer
 */
BINT bigint_fromString(char *str);

/**
 * creates new positive big integer from an unsigned integer
 * 
 * PROCESS - conversion of bases
 * i_10 / BASE = i' R digits[0] = LSD (least significant digit)
 * i' / BASE = i'' R digits[1]
 * ...
 * i''' / BASE = 0 R digits[N - 1] = MSD (most significant digit)
 * 
 * @param i the integer
 * @return the integer
 */
BINT bigint_fromUint(unsigned int i);

/**
 * creates new positive big integer from an unsigned long long
 * same process as bigint_fromUint
 * 
 * @param l the num
 * @return the big integer
 */
BINT bigint_fromULLint(unsigned long long l);

/**
 * creates new big integer from an integer
 * calls bigint_fromUint then applies sign after
 * @param i the integer
 * @return the big integer
 */
BINT bigint_fromInt(int i);

/**
 * creates new big integer from a long long
 * calls bigint_fromULLint then applies sign after
 * @param i the long long
 * @return the big integer
 */
BINT bigint_fromLLint(long long l);

/**
 * multiplies two integers together and handles possible overflow
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the big integer containing the product
 */
BINT bigint_fromUiMult(unsigned int i1, unsigned int i2);

/**
 * trims leading zeros in the array of digits
 * @param b the pointer to the integer to trim
 */
void bigint_trim(BINT *b);

/**
 * get string representation of number
 * @param i the big integer
 * @return the string
 */
char *bigint_toString(BINT i);

/**
 * comparison of two integers
 * @param i1 the first integer
 * @param i2 the second integer
 * @return -1 if i1 < i2, 0 if i1 = i2, 1 if i1 > i2
 */
char bigint_compare(BINT i1, BINT i2);

/**
 * 
*/
void printHex(BINT* X);

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

/**********************************************************************
* Sage:
* print(int("####", 16) + int("####", 16) == int("####", 16))
*
* @:option - 0: '+' | 1: '-' | 2: '*'
**********************************************************************/
void printSage(BINT* X, BINT* Y, BINT* Z,  int option, int loop);

void refine_BINT(BINT* X);

//Generate Random BINT

void rand_array(WORD* dst, int wordlen);
void rand_bint(BINT**x, int sign, int wordlen);

//Copy Arrary
void array_copy_x2y(WORD* X, WORD* Y, int wordlen);

//Assign Y<-X
void assgin_x2y(BINT* X, BINT** Y);

//Compare
// X>Y return 1
// X<Y return -1
// X=Y return 0
int compare_xy(BINT* X, BINT* Y);

//Author: Moon Ye-chan
int Get_bitlen(BINT* x);
int Get_sign(BINT* x);
void Flip_sign(BINT* x);

//Author: Kim Ye-chan
void DIV_Shift(BINT* bigint, BINT* result, int N);// DIV shift  
void MUL_Shift(BINT* bigint, BINT* result , int N); // MUL shift


#endif // _BIGINTEGER_H