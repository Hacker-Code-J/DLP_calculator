/***************************************************************************
 * Project Name: PUBAO (P.A.N.D.A.'s Unbounded Big Arithmetic Operations)
 * Team Name: P.A.N.D.A. (Programmers Aspiring to Navigate Digital Arithmetic)
 * 
 * File Description: This file contains the definitions and implementations
 *                   for large integer arithmetic.
 * 
 * Author(s): Ji Yong-Hyeon, Kim Ye-chan, Moon Ye-chan, Yoo Geun-oh
 * Date Created: 2023-09-21
 * Last Modified: 2023-10-25
 *
 * Note: This library aims to provide a robust and efficient solution for
 *       performing arithmetic on large integers beyond the limitations of
 *       standard data types.
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>

#include "setup.h"
#include "BigInteger.h"

int main() {
    srand((unsigned int)time(NULL));\

    BINT* a = NULL;
    init_bint(&a, 10);

    printf("bool: %ld\n", 8*sizeof(bool));
    printf("int: %ld\n", 8*sizeof(int));
    printf("WORD: %ld\n", 8*sizeof(WORD));
    printf("BINT: %ld\n", 8*sizeof(BINT));
    printf("BINT-val: %ld\n", 8*sizeof(a->val));

    delete_bint(&a);

    return 0;
}