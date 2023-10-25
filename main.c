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
#include "matrix.h"
#include "operation.h"

int main() {
    srand((unsigned int)time(NULL));

    clock_t start, end;

    printf("%ld\n", sizeof(Matrix));

    BINT* a = NULL;
    BINT* b = NULL;
    BINT* c = NULL;

    int n = 0x00;
    int m = 0x00;
    int max;
    int a_sgn, b_sgn;
    
    //0: '+', 1: '-', 2: '*'
    int opt;

    for(int i=0; i<1; i++) {
        //opt = rand() % 0x02;
        opt = 0;
        
        a_sgn = rand() % 0x02;
        b_sgn = rand() % 0x02;
        
        n = rand() % 0x3; n++;
        m = rand() % 0x3; m++;
        max = MAX(n, m);

        printf("\n****************************\n[Test %d]\n****************************\n", i+1);
        printf("n m: %d %d\n", n,m);
        rand_bint(&a, a_sgn, n);
        rand_bint(&b, b_sgn, m);
        init_bint(&c, max);
        
        custom_printHex_xy(a,b,max);

        start = clock();
        if(opt == 0) {
            ADD_xyz(a,b,c);
        } else if(opt == 1) {
            SUB_xyz(a,b,c);
        } else {
            //mult_xyc(a->val,b->val,c->val);
        }
        end = clock();

        printf("\nResult:\n\n");
        a->sign = a_sgn;
        b->sign = b_sgn;
        custom_printHex(a,b,c,opt);

        printSage(a,b,c,opt,i);

        delete_bint(&a);
        delete_bint(&b);
        delete_bint(&c);

        double time_taken = ((double) end - start) / CLOCKS_PER_SEC;
        printf("\nTime: %f.\n", time_taken);
    }

    return 0;
}