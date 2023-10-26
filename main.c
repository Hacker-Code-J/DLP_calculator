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
#include "operation.h"

BINTQueue* queue;

int main() {
    srand((unsigned int)time(NULL));

    // clock_t start, end;
    int t = 2;

    queue = createQueue();

    BINT* bint1 = NULL;
    BINT* bint2 = NULL;
    BINT* bint3 = NULL;

    while(t != 0) {
        bool rand_sgn1 = rand() % 0x02;
        bool rand_sgn2 = rand() % 0x02;
        int n = rand() % 0x05  + 1;
        int m = rand() % 0x05  + 1;
        int max = MAX(n, m);
        rand_bint(&bint1, rand_sgn1, n);
        rand_bint(&bint2, rand_sgn2, m);
        bint3 = init_bint(&bint3, max);

        ADD_xyz(bint1,bint2,bint3);
        bint1->sign = rand_sgn1;
        bint2->sign = rand_sgn2;

        custom_printHex(bint1,bint2,bint3,0);

        delete_bint(&bint1);
        delete_bint(&bint2);
        delete_bint(&bint3);
        t--;
    }

}