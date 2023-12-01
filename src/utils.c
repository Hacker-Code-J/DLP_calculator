#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


void exit_on_null_error(const void* ptr, const char* ptr_name, const char* function_name) {
    if(!ptr) {
        fprintf(stderr, "Error: '%s' is NULL in '%s'\n", ptr_name, function_name);
        exit(1);
    }
}

void delete_bint(BINT** pptrBint) {
    if(!(*pptrBint))
        return;
    free((*pptrBint)->val);
    free(*pptrBint);
    *pptrBint = NULL;
}

void init_bint(BINT** pptrBint, int wordlen) {
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

void copyBINT(BINT** pptrBint_dst, BINT** pptrBint_src) {
    CHECK_PTR_AND_DEREF(pptrBint_src, "pptrBint_src", "copyBINT");
    
    init_bint(pptrBint_dst, (*pptrBint_src)->wordlen);
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

void refineBINT(BINT* ptrBint) {
    if(ptrBint == NULL) return;

    int new_wordlen = ptrBint->wordlen;
    while (new_wordlen > 1) { // at least one word needed
        if(ptrBint->val[new_wordlen-1] != 0)
            break;
        new_wordlen--;
    }
    if(ptrBint->wordlen != new_wordlen) {
        ptrBint->wordlen = new_wordlen;
        WORD* tmp = ptrBint->val;
        tmp = (WORD*)realloc(ptrBint->val, sizeof(WORD)*new_wordlen);
        ptrBint->val = tmp;
        // ptrBint->val = (WORD*)realloc(X->val, sizeof(WORD)*new_wordlen);
    }

    if((ptrBint->wordlen == 1) && (ptrBint->val[0] == 0))
        ptrBint->sign = false;
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

bool GET_BIT(BINT* ptrBint, int i_th)  {
   if (i_th >= WORD_BITLEN)
      return (((ptrBint)->val[i_th / WORD_BITLEN] >> (i_th % WORD_BITLEN)) & WORD_ONE);

   return (((ptrBint)->val[0] >> i_th) & WORD_ONE);
}

void RANDOM_ARRARY(WORD* dst, int wordlen) {
    u8* p = (u8*) dst;
    int cnt = wordlen * sizeof(WORD);
    while (cnt > 0) {
        *p = rand() & 0xff;
        p++;
        cnt--;
    }
}

void RANDOM_BINT(BINT** pptrBint, bool sign, int wordlen) {
    init_bint(pptrBint, wordlen);
    (*pptrBint)->sign = sign;
    RANDOM_ARRARY((*pptrBint)->val, wordlen);
    refineBINT(*pptrBint);
}

bool compare_bint(BINT* ptrBint1, BINT* ptrBint2) {
    // Ensure the provided pointers are valid
    CHECK_PTR_AND_DEREF(&ptrBint1, "pptrBint1", "compare_bint");
    CHECK_PTR_AND_DEREF(&ptrBint2, "pptrBint2", "compare_bint");

    // If one is negative and the other positive, the positive one is greater
    if ((ptrBint1)->sign ^ (ptrBint2)->sign) {
        return (ptrBint1)->sign < (ptrBint2)->sign;
    }

    // If both have the same sign, compare their absolute values
    bool abs_val = compare_abs_bint(ptrBint1, ptrBint2);

    // If both are positive, the one with the greater absolute value is greater
    // If both are negative, the one with the smaller absolute value is greater
    return (ptrBint1)->sign ? !abs_val : abs_val;
}

bool compare_abs_bint(BINT* ptrX, BINT* ptrY) {
    // Ensure the provided pointers are valid
    CHECK_PTR_AND_DEREF(&ptrX, "pptrX", "compare_abs_bint");
    CHECK_PTR_AND_DEREF(&ptrY, "pptrY", "compare_abs_bint");

    // Extract word lengths for both numbers
    int n = (ptrX)->wordlen; int m = (ptrY)->wordlen;

    // Compare the word lengths of the two numbers
    if(n > m) return 1;
    if(n < m) return 0;
    
    // Perform a word-by-word comparison starting from the most significant word
    matchSize(ptrX, ptrY);
    for(int i = (ptrX)->wordlen - 1; i >= 0; i--) {
        if((ptrX)->val[i] > (ptrY)->val[i]) return 1;
        if((ptrX)->val[i] < (ptrY)->val[i]) return 0;
    }
    // Numbers are equal in value
    return 1;
}

void print_bint_hex_py(const BINT* ptrBint) {
    if (!(&ptrBint)) {
        fprintf(stderr, "Invalid BINT** structure in 'print_hex_python'.\n");
        return;
    }
    if (!(ptrBint)) {
        fprintf(stderr, "Invalid BINT* structure in 'print_hex_python'.\n");
        return;
    }
    if (!(ptrBint)->val) {
        fprintf(stderr, "Invalid BINT->val in 'print_hex_python'.\n");
        return;
    }
    if ((ptrBint)->sign) { printf("-"); }
    printf("0x");
    
    for (int i = (ptrBint)->wordlen - 1; i >= 0; i--) {
#if WORD_BITLEN == 8
    printf("%02x", (ptrBint)->val[i]);
#elif WORD_BITLEN == 64
    printf("%016llx", (ptrBint)->val[i]);
#else
    printf("%08x", (ptrBint)->val[i]);
#endif
    }
}