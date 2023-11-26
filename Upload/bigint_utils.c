#include "config.h"
#include "bigint_utils.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void exit_on_null_error(const void* ptr, const char* ptr_name, const char* function_name) {
    if(!ptr) {
        fprintf(stderr, "Error: '%s' is NULL in '%s'\n", ptr_name, function_name);
        exit(1);
    }
}
#define CHECK_PTR_AND_DEREF(ptr, name, func) \
    do { \
        exit_on_null_error(ptr, name, func); \
        exit_on_null_error(*ptr, "*" name, func); \
    } while(0)

#define CHECK_PTR_DEREF_AND_VAL(ptr, name, func) \
    do { \
        exit_on_null_error(ptr, name, func); \
        exit_on_null_error(*ptr, "*" name, func); \
        if(*ptr) exit_on_null_error((*ptr)->val, "(*" name ")->val", func); \
    } while(0)

void delete_bint(BINT** pptrBint) { // ptrBint = *pptrBint
    if(!(*pptrBint))
        return;
    free((*pptrBint)->val);
    free(*pptrBint);
    *pptrBint = NULL;
    // if (pptrBint && *pptrBint) {
    //     free((*pptrBint)->val); // Free the allocated memory for val
    //     free(*pptrBint); // Free the allocated memory for BINT structure
    //     *pptrBint = NULL; // Set the pointer to NULL after freeing memory to avoid using a dangling pointer
    // }
}

void init_bint(BINT** pptrBint, int wordlen) { // ptrBint = *pptrBint
    if((*pptrBint) != NULL)
        delete_bint(pptrBint);

    // Allocate memory for BINT structure
    *pptrBint = (BINT*)malloc(sizeof(BINT));
    if(!(*pptrBint)) {
        fprintf(stderr, "Error: Unable to allocate memory for BINT.\n");
        exit(1);
    }
    // Allocate memory for val (array of WORD)
    (*pptrBint)->val = (WORD*)calloc(wordlen,sizeof(WORD));
    if (!(*pptrBint)->val) {
        free(*pptrBint); // freeing the already allocated BINT memory before exiting
        fprintf(stderr, "Error: Unable to allocate memory for BINT val.\n");
        exit(1);
    }
    // Initialize structure members
    (*pptrBint)->sign = false;
    (*pptrBint)->wordlen = wordlen;
}

void SET_BINT_ZERO(BINT** pptrBint) { 
    init_bint(pptrBint, 1);
    (*pptrBint)->val[0] = 0x00;
}
void SET_BINT_ONE(BINT** pptrBint) { 
    init_bint(pptrBint, 1);
    (*pptrBint)->val[0] = 0x01;
 }

void SET_BINT_CUSTOM_ZERO(BINT** pptrBint, int num_words) {
    init_bint(pptrBint, num_words);
    // for(int i = 0; i < num_words; i++)
    //     (*pptrBint)->val[i] = 0x00;
}
void SET_BINT_CUSTOM_ONE(BINT** pptrBint, int num_words) {
    init_bint(pptrBint, num_words);
    (*pptrBint)->val[0] = 0x01;
    // for(int i = 1; i < num_words; i++)
    //     (*pptrBint)->val[i] = 0x00;
}

void SET_BIT(BINT** ptrBint, int bit_idx, bool bit_val) {
    int word_idx = bit_idx / (sizeof(WORD) * 8);
    int bit_pos = bit_idx % (sizeof(WORD) * 8);
    if (bit_val) {
        (*ptrBint)->val[word_idx] |= (1 << bit_pos);
    } else {
        (*ptrBint)->val[word_idx] &= ~(1 << bit_pos);
    }
}

void copyBINT(BINT** pptrBint_dst, BINT** pptrBint_src) {
    CHECK_PTR_AND_DEREF(pptrBint_src, "pptrBint_src", "copyBINT");
    
    init_bint(pptrBint_dst, (*pptrBint_src)->wordlen);
    // ptrBint_dst = (BINT*)calloc(1, sizeof(BINT));
    // (*pptrBint_dst)->val = (WORD*)calloc((*pptrBint_src)->wordlen, sizeof(WORD));
    for(int i = 0; i < (*pptrBint_src)->wordlen; i++)
        (*pptrBint_dst)->val[i] = (*pptrBint_src)->val[i];
    
    (*pptrBint_dst)->wordlen = (*pptrBint_src)->wordlen;
    (*pptrBint_dst)->sign = (*pptrBint_src)->sign;
}

void swapBINT(BINT** pptrBint1, BINT** pptrBint2) {
    if((*pptrBint1) != (*pptrBint2)) { // If they aren't the same pointer
        (*pptrBint1)->sign ^= (*pptrBint2)->sign;
        (*pptrBint2)->sign ^= (*pptrBint1)->sign;
        (*pptrBint1)->sign ^= (*pptrBint2)->sign;

        (*pptrBint1)->wordlen ^= (*pptrBint2)->wordlen;
        (*pptrBint2)->wordlen ^= (*pptrBint1)->wordlen;
        (*pptrBint1)->wordlen ^= (*pptrBint2)->wordlen;

        WORD* tmpVal = (*pptrBint1)->val;
        (*pptrBint1)->val = (*pptrBint2)->val;
        (*pptrBint2)->val = tmpVal;
    }
}

void makeEven(BINT* ptrBint) {
    // Check if wordlen is odd
    if ((ptrBint)->wordlen % 2 == 1) {
        (ptrBint)->wordlen++; // Increment wordlen to make it even

        // Reallocate memory for val
        (ptrBint)->val = realloc((ptrBint)->val, (ptrBint)->wordlen * sizeof(WORD));
        if (!(ptrBint)->val) {
            // Handle memory allocation failure, exit or return an error
            exit(1); 
        }

        // Fill the new WORD with 0
        (ptrBint)->val[(ptrBint)->wordlen - 1] = 0;
    }
}

void matchSize(BINT* ptrBint1, BINT* ptrBint2) {
    int max_wordlen = MAXIMUM(ptrBint1->wordlen, ptrBint2->wordlen);

    // Resize ptrBint1 if its wordlen is smaller than max_wordlen
    if(ptrBint1->wordlen < max_wordlen) {
        WORD* tmp = ptrBint1->val;
        tmp = (WORD*)realloc(ptrBint1->val, max_wordlen * sizeof(WORD));
        if (!tmp) {
            // Handle memory allocation failure, exit or return an error
            fprintf(stderr,"Memory allocation failure in 'matchSize'");
            exit(1); 
        }
        ptrBint1->val = tmp;

        // Initialize the newly allocated WORDs with 0
        for(int i = ptrBint1->wordlen; i < max_wordlen; i++) {
            ptrBint1->val[i] = 0;
        }

        ptrBint1->wordlen = max_wordlen;
    }

    // Resize ptrBint2 if its wordlen is smaller than max_wordlen
    if(ptrBint2->wordlen < max_wordlen) {
        WORD* tmp = ptrBint2->val;
        tmp = (WORD*)realloc(ptrBint2->val, max_wordlen * sizeof(WORD));
        if (!tmp) {
            // Handle memory allocation failure, exit or return an error
            fprintf(stderr,"Memory allocation failure in 'matchSize'");
            exit(1); 
        }
        ptrBint2->val = tmp;

        // Initialize the newly allocated WORDs with 0
        for(int i = ptrBint2->wordlen; i < max_wordlen; i++) {
            ptrBint2->val[i] = 0;
        }

        ptrBint2->wordlen = max_wordlen;
    }
}

void reset_bint(BINT* ptrBint) {
    for (int i = 0; i < ptrBint->wordlen; i++)
        ptrBint->val[i] = 0;
}

bool isZero(BINT* ptrbint) {
    if (ptrbint->wordlen == 0) return true;

    for (int i = 0; i < ptrbint->wordlen; ++i) {
        if (ptrbint->val[i] != 0) return false;
    }
    
    return true;
}
bool isOne(BINT* ptrbint) {
    if (ptrbint->wordlen < 1) return false; // Can't be 1 if there are no words.
    if (ptrbint->val[0] != 1) return false; // First word must be 1.
    if (ptrbint->sign) return false; // Sign must be positive.

    for (int i = 1; i < ptrbint->wordlen; ++i) {
        if (ptrbint->val[i] != 0) return false; // Every other word must be 0.
    }
    
    return true;
}