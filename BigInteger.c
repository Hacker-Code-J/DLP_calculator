#include "setup.h"
#include "BigInteger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

WORD arrOne[1] = {1};

const BINT BINT_ZERO = {false, 0, NULL};
const BINT BINT_ONE = {false, 1, arrOne};
const BINT BINT_NEG_ONE = {true, 1, arrOne};

/**
 * 
*/
BINT* init_bint(BINT** bint_ptr, int wordlen) {
    // Allocate memory for BINT structure
    *bint_ptr = (BINT*)malloc(sizeof(BINT));
    if(!*bint_ptr) {
        fprintf(stderr, "Error: Unable to allocate memory for BINT.\n");
        exit(1);
    }
    // Allocate memory for val (array of WORD)
    (*bint_ptr)->val = (WORD*)calloc(wordlen,sizeof(WORD));
    if (!(*bint_ptr)->val) {
        free(*bint_ptr); // freeing the already allocated BINT memory before exiting
        fprintf(stderr, "Error: Unable to allocate memory for BINT val.\n");
        exit(1);
    }
    // Initialize structure members
    (*bint_ptr)->sign = false;
    (*bint_ptr)->wordlen = wordlen;
    return *bint_ptr;
}

/**
 * 
*/
void delete_bint(BINT** bint_ptr) {
    if (bint_ptr && *bint_ptr) {
        free((*bint_ptr)->val); // Free the allocated memory for val
        free(*bint_ptr); // Free the allocated memory for BINT structure
        *bint_ptr = NULL; // Set the pointer to NULL after freeing memory to avoid using a dangling pointer
    }
}