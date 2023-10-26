#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "setup.h"

#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#define WORD u32        // base used to represent each digit in the big integer

/**
 * Data structure for representing an Integer
 * Every integer in the array signifies a digit in base 2^w, where w = 8 * sizeof(WORD)
 * 
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
*/
typedef struct {
    bool sign;      // the sign of the number (false if 0 or positive, true if negative)
    u32 wordlen;    //
    WORD* val;      // threshold between performing long and karatsuba multiplication
} BINT;

extern const BINT BINT_ZERO;    //zero integer
extern const BINT BINT_ONE;     //one integer
extern const BINT BINT_NEG_ONE; //negative one integer

/**
 * BINT* ptrBint;
 * BINT* init_bint(&ptrBint, k);
 * 
 * Before calling 'init_bint':
 * +-----------+
 * |  ptrBint  |
 * |-----------|
 * |  [null]   |
 * +-----------+
 *
 * Initializing other members of the structure:
 * +-----------+
 * |  ptrBint  |
 * |-----------|
 * | [address]-|-----> +----------------------+
 * +-----------+       |     initialized      |
 *                     |----------------------|
 *                     | sign:    false       |
 *                     | wordlen: k           |
 *                     | val:     [address]---|----> +---------+---------+---+-----------+
 *                     +----------------------+      | WORD[0] | WORD[1] |...| WORD[k-1] |
 *                                                   +---------+---------+---+-----------+ 
 * 
 * allocate memory for the integer
 * @param bint_ptr the point of BINT
 * @param wordlen the initial size of the array
 * @return the integer pointer
 */
BINT* init_bint(BINT** pptrBint, int wordlen);

/**
 * BINT* ptrBint;
 * BINT* delete_bint(&ptrBint);
 * 
 * Before calling 'delete_bint':
 * +-----------+
 * |  ptrBint  |
 * |-----------|
 * | [address]-|-----> +---------------------+
 * +-----------+       |     initialized     |
 *                     |---------------------|
 *                              ...
 * Inside 'delete_bint':
 * - Step1:
 * +-----------+
 * |  ptrBint  |
 * |-----------|
 * | [address]-|-----> +---------------------+
 * +-----------+       |   partially free    |
 *                     |---------------------|
 *                     | sign:    false      |
 *                     | wordlen: wordlen    |
 *                     | val:     [freed]    |
 *                     +---------------------+
 * - Step2:
 * +-----------+
 * |  ptrBint  |
 * |-----------|
 * |  [null]   |
 * +-----------+
*/
void delete_bint(BINT** pptrBint);

typedef struct Node {
    BINT data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} BINTQueue;

BINTQueue* createQueue();
void enqueue(BINTQueue* q, BINT data);
BINT dequeue(BINTQueue* q);
BINT peek(BINTQueue* q);
bool isempty(BINTQueue* q);
void freeQueue(BINTQueue* q);

/**
 * File I/O
*/
bool store_bint(const char* filename, BINT* b);
bool multi_store_bints(const char* filename, BINT** bint_array, int num_bints);
BINT* load_bint(const char* filename);
BINT** multi_load_bints(const char* filename, int* num_bints);

/**
 * option - 0: '+' | 1: '-' | 2: '*'
 *
def hex_to_int(hex_str, sign):
    val = Integer(hex_str.replace(' ', '').replace('0x', ''), 16)
    return -val if sign == "[1]" else val

def int_to_hex(val):
    sign = "[0]" if val >= 0 else "[1]"
    abs_val = abs(val)
    
    hex_str = hex(abs_val)[2:]
    hex_str = ' '.join([hex_str[i:i+8] for i in range(0, len(hex_str), 8)])
    
    return f"{sign} 0x {hex_str}"

def check_operation(x_sign, x_hex, y_sign, y_hex, expected_sign, expected_hex, operation):
    x_int = hex_to_int(x_hex, x_sign)
    y_int = hex_to_int(y_hex, y_sign)
    
    if operation == "add":
        result_int = x_int + y_int
    elif operation == "sub":
        result_int = x_int - y_int
    elif operation == "mul":
        result_int = x_int * y_int
    else:
        raise ValueError(f"Invalid operation: {operation}")
    
    result_hex = int_to_hex(result_int)
    
    return result_hex.replace(' ', '').lower() == (expected_sign + ' ' + expected_hex).replace(' ', '').lower()

# Sample sets of values for addition, subtraction, and multiplication.
values = [
    ("[0]", "0x 00000000 3659e17f e9e44af4", "[0]", "0x 4a9f22e4 623b83af 7dea0427", "[0]", "0x 4a9f22e4 9895652f 67ce4f1b", "add"),
    ("[0]", "0x 4a9f22e4 9895652f 67ce4f1b", "[0]", "0x 4a9f22e4 623b83af 7dea0427", "[0]", "0x 00000000 3659e17f e9e44af4", "sub"),
    ("[0]", "0x 00000000 00000000 00000002", "[0]", "0x 00000000 00000000 00000003", "[0]", "0x 00000000 00000000 00000006", "mul"),
]

for idx, (x_sign, x, y_sign, y, expected_sign, expected, op) in enumerate(values):
    is_correct = check_operation(x_sign, x, y_sign, y, expected_sign, expected, op)
    print(is_correct)
 *
*/
// void printSage(BINTQueue* queue,  int opt, int loop);

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


#endif // BIGINTEGER_H