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
#include "bint_fifo.h"
#include "operation.h"

int main() {
    srand((unsigned int)time(NULL));

    // clock_t start, end;

    int t = 5;
    BINTQueue* queue = createQueue(t);
    
    while (!t)
    {
        
    }
    
    // Suppose you've created multiple BINTs:
    bool rand_sgn1 = rand() % 0x02;
    bool rand_sgn2 = rand() % 0x02;
    int n = rand() % 0x05  + 1;
    int m = rand() % 0x05  + 1;
    int max = MAX(n, m);

    BINT* bint1 = NULL;
    BINT* bint2 = NULL;
    BINT* bint3 = NULL;
    rand_bint(&bint1, rand_sgn1, n);
    rand_bint(&bint2, rand_sgn2, m);
    init_bint(&bint3, max);

    ADD_xyz(bint1,bint2,bint3);
    bint1->sign = rand_sgn1;
    bint2->sign = rand_sgn2;
    enqueue(queue, *bint1);
    enqueue(queue, *bint2);
    enqueue(queue, *bint3);
    //... populate the BINT structures ...

    // Saving multiple BINTs:
    // BINT* bint_array[] = {bint1, bint2, bint3};
    // multi_store_bints("multi_bint_data.bin", bint_array, 3);

    // multi_store_bints2("multi_bint_data.bin", queue, t);

    // Cleanup
    delete_bint(&bint1);
    delete_bint(&bint2);
    delete_bint(&bint3);

    for (int i = 0; i < t; i++) {
        BINT b1 = dequeue(queue);
        BINT b2 = dequeue(queue);
        BINT b3 = dequeue(queue);
        printSage(&b1, &b2, &b3, 0, t);
    }

    // Loading multiple BINTs:
    // int num_bints;
    // BINT** loaded_bints = multi_load_bints("multi_bint_data.bin", &num_bints);
    // if (loaded_bints) {
    //     for (int i = 0; i < num_bints; i++) {
    //         // BINT* b1 = loaded_bints[i];
    //         // BINT* b2 = loaded_bints[3*i + 1];
    //         // BINT* b3 = loaded_bints[3*i + 2];
    //         BINT b1 = dequeue(queue);
    //         BINT b2 = dequeue(queue);
    //         BINT b3 = dequeue(queue);
    //         printSage(&b1, &b2, &b3, 0, t);

    //         delete_bint(&b1);
    //         delete_bint(&b2);
    //         delete_bint(&b3);
    //     }
    //     free(loaded_bints); // free the array after deleting all its BINT pointers
    // }

    freeQueue(queue);
    return 0;
}