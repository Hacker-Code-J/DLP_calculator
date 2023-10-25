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

int main() {
    srand((unsigned int)time(NULL));

    clock_t start, end;

    BINT* my_bint = NULL;

    // Initialize a BINT structure with a wordlen of 5 (for demonstration purposes)
    init_bint(&my_bint, 5);

    // Example: populate the BINT structure
    my_bint->sign = false;  // set as positive
    my_bint->val[0] = 123;
    my_bint->val[1] = 456;
    my_bint->val[2] = 789;
    my_bint->val[3] = 111;
    my_bint->val[4] = 222;

    // Store the BINT to a file
    if (store_bint("bint_data.bin", my_bint)) {
        printf("Data successfully stored to file.\n");
    } else {
        fprintf(stderr, "Failed to store data to file.\n");
    }

    // Cleanup the memory of the original BINT structure
    delete_bint(&my_bint);

    // Now, let's load the BINT from the file we saved
    BINT* loaded_bint = load_bint("bint_data.bin");
    if (loaded_bint) {
        printf("Data successfully loaded from file.\n");
        printf("Sign: %d\n", loaded_bint->sign);
        printf("Word Length: %u\n", loaded_bint->wordlen);
        for (u32 i = 0; i < loaded_bint->wordlen; i++) {
            printf("Word[%u]: %u\n", i, loaded_bint->val[i]);
        }

        // Cleanup memory of the loaded BINT structure
        delete_bint(&loaded_bint);
    } else {
        fprintf(stderr, "Failed to load data from file.\n");
    }

    // BINT* a = NULL;
    // BINT* b = NULL;
    // BINT* c = NULL;

    // int n = 0x00;
    // int m = 0x00;
    // int max;
    // int a_sgn, b_sgn;
    
    // //0: '+', 1: '-', 2: '*'
    // int opt;

    // for(int i=0; i<1; i++) {
    //     //opt = rand() % 0x02;
    //     opt = 0;
        
    //     a_sgn = rand() % 0x02;
    //     b_sgn = rand() % 0x02;
        
    //     n = rand() % 0x3; n++;
    //     m = rand() % 0x3; m++;
    //     max = MAX(n, m);

    //     //printf("\n****************************\n[Test %d]\n****************************\n", i+1);
    //     //printf("n m: %d %d\n", n,m);
    //     rand_bint(&a, a_sgn, n);
    //     rand_bint(&b, b_sgn, m);
    //     init_bint(&c, max);
        
    //     //custom_printHex_xy(a,b,max);

    //     start = clock();
    //     if(opt == 0) {
    //         ADD_xyz(a,b,c);
    //     } else if(opt == 1) {
    //         SUB_xyz(a,b,c);
    //     } else {
    //         //mult_xyc(a->val,b->val,c->val);
    //     }
    //     end = clock();

    //     //printf("\nResult:\n\n");
    //     a->sign = a_sgn;
    //     b->sign = b_sgn;
    //     //custom_printHex(a,b,c,opt);

    //     printSage(a,b,c,opt,i);

    //     delete_bint(&a);
    //     delete_bint(&b);
    //     delete_bint(&c);

    //     double time_taken = ((double) end - start) / CLOCKS_PER_SEC;
    //     //printf("\nTime: %f.\n", time_taken);
    // }

    return 0;
}