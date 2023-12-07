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
        free(*pptrBint); // Free the already allocated BINT memory
        fprintf(stderr, "Error: Unable to allocate memory for BINT val.\n");
        exit(1);
    }
    // Initialize structure members
    (*pptrBint)->sign = false;
    (*pptrBint)->wordlen = wordlen;
}

void copyBINT(BINT** pptrBint_dst, BINT** pptrBint_src) {
    CHECK_PTR_AND_DEREF(pptrBint_src, "pptrBint_src", "copyBINT");
    
    // Initialize destination BINT structure with the same word length as the source
    init_bint(pptrBint_dst, (*pptrBint_src)->wordlen);
    
    // Copy each element of the val array from source to destination
    for(int i = 0; i < (*pptrBint_src)->wordlen; i++)
        (*pptrBint_dst)->val[i] = (*pptrBint_src)->val[i];
    
    // Copy the word length and sign from source to destination
    (*pptrBint_dst)->wordlen = (*pptrBint_src)->wordlen;
    (*pptrBint_dst)->sign = (*pptrBint_src)->sign;
}

void swapBINT(BINT** pptrBint1, BINT** pptrBint2) {
    if((*pptrBint1) != (*pptrBint2)) { // If they aren't the same pointer
        // Swap the sign field of both BINTs using XOR swap algorithm
        (*pptrBint1)->sign ^= (*pptrBint2)->sign;
        (*pptrBint2)->sign ^= (*pptrBint1)->sign;
        (*pptrBint1)->sign ^= (*pptrBint2)->sign;

        // Swap the wordlen field of both BINTs using XOR swap algorithm
        (*pptrBint1)->wordlen ^= (*pptrBint2)->wordlen;
        (*pptrBint2)->wordlen ^= (*pptrBint1)->wordlen;
        (*pptrBint1)->wordlen ^= (*pptrBint2)->wordlen;

        // Swap the val pointers of both BINTs
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
        (ptrBint)->val[(ptrBint)->wordlen - 1] = (WORD)0;
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
        for(int i = ptrBint1->wordlen; i < max_wordlen; i++)
            ptrBint1->val[i] = (WORD)0;

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
        for(int i = ptrBint2->wordlen; i < max_wordlen; i++)
            ptrBint2->val[i] = (WORD)0;

        ptrBint2->wordlen = max_wordlen;
    }
}

void resetBINT(BINT* ptrBint) {
    for (int i = 0; i < ptrBint->wordlen; i++)
        ptrBint->val[i] = (WORD)0;
}

void refineBINT(BINT* ptrBint) {
    if(ptrBint == NULL) return;

    int new_wordlen = ptrBint->wordlen;
    
    // Loop to find the new word length by trimming trailing zeros
    while (new_wordlen > 1) { // at least one word needed
        if(ptrBint->val[new_wordlen-1] != 0)
            break;
        new_wordlen--;
    }

    // Update the word length and reallocate memory if necessary
    if(ptrBint->wordlen != new_wordlen) {
        ptrBint->wordlen = new_wordlen;
        WORD* tmp = ptrBint->val;
        tmp = (WORD*)realloc(ptrBint->val, sizeof(WORD)*new_wordlen);
        ptrBint->val = tmp;
    }

    // Reset the sign to false if the BINT represents zero
    if((ptrBint->wordlen == 1) && (ptrBint->val[0] == 0))
        ptrBint->sign = false;
}

void refine_BINT_word(BINT* ptrBint, int num_words) {
    if(ptrBint == NULL) return;

    int new_wordlen = ptrBint->wordlen - num_words;
    
    // Update the word length and reallocate memory if necessary
    if(ptrBint->wordlen != new_wordlen) {
        ptrBint->wordlen = new_wordlen;
        WORD* tmp = ptrBint->val;
        tmp = (WORD*)realloc(ptrBint->val, sizeof(WORD)*new_wordlen);
        ptrBint->val = tmp;
    }

    // Reset the sign to false if the BINT represents zero
    if((ptrBint->wordlen == 1) && (ptrBint->val[0] == 0))
        ptrBint->sign = false;
}

bool isZero(BINT* ptrbint) {
    if (ptrbint->wordlen == 0) return true;

    // Loop through each word in the val array of ptrbint
    for (int i = 0; i < ptrbint->wordlen; ++i) {
        if (ptrbint->val[i] != 0) return false;
    }
    
    return true;
}

bool isOne(BINT* ptrbint) {
    if (ptrbint->wordlen < WORD_ONE) return false; // Can't be 1 if there are no words.
    if (ptrbint->val[0] != WORD_ONE) return false; // First word must be 1.
    if (ptrbint->sign) return false; // Sign must be positive.

    // Loop through each word after the first one in the val array
    for (int i = 1; i < ptrbint->wordlen; ++i) {
        if (ptrbint->val[i] != 0) return false; // Every other word must be 0.
    }
    
    return true;
}

bool GET_BIT(BINT* ptrBint, int i_th)  {
   // If it is, calculate which WORD it belongs to and which bit within that WORD
    // Then, extract that bit and return its value
    if (i_th >= WORD_BITLEN)
        return (((ptrBint)->val[i_th / WORD_BITLEN] >> (i_th % WORD_BITLEN)) & WORD_ONE);

    // If the i_th bit is within the first WORD
    // Shift the bits of the first WORD right by i_th positions and isolate the bit
    return (((ptrBint)->val[0] >> i_th) & WORD_ONE);
}

WORD GET_WORD(BINT* ptrBint, int m_th) {
    // Check if the requested word index is out of bounds
    if (m_th < 0 || m_th >= ptrBint->wordlen) {
        fprintf(stderr, "Error: Requested word index %d is out of bounds.\n", m_th);
        return 0;
    }

    // Return the m_th word from the val array
    return ptrBint->val[m_th];
}

void RANDOM_ARRARY(WORD* dst, int wordlen) {
    // Cast the destination pointer to a pointer to u8 (unsigned 8-bit integer)
    u8* p = (u8*) dst;
    
    // Calculate the total number of bytes to be filled
    int cnt = wordlen * sizeof(WORD);

    // Loop until all bytes are filled
    while (cnt > 0) {
        *p = rand() & 0xff; // Assign a random byte (0 to 255) to the current location pointed by p
        p++;                // Move to the next byte
        cnt--;
    }
}

void RANDOM_BINT(BINT** pptrBint, bool sign, int wordlen) {
    init_bint(pptrBint, wordlen);
    (*pptrBint)->sign = sign;
    RANDOM_ARRARY((*pptrBint)->val, wordlen);
    refineBINT(*pptrBint);
}

bool compare_abs_bint(BINT* ptrBint1, BINT* ptrBint2) {
    // Ensure the provided pointers are valid
    CHECK_PTR_AND_DEREF(&ptrBint1, "ptrBint1", "compare_abs_bint");
    CHECK_PTR_AND_DEREF(&ptrBint2, "ptrBint2", "compare_abs_bint");

    // Extract word lengths for both numbers
    int n = (ptrBint1)->wordlen; int m = (ptrBint2)->wordlen;

    // Compare the word lengths of the two numbers
    if(n > m) return 1;
    if(n < m) return 0;
    
    // Make both BINTs the same size for comparison
    matchSize(ptrBint1, ptrBint2);

    // Compare word by word from the most significant word
    for(int i = (ptrBint2)->wordlen - 1; i >= 0; i--) {
        if((ptrBint1)->val[i] > (ptrBint2)->val[i]) return 1;
        if((ptrBint1)->val[i] < (ptrBint2)->val[i]) return 0;
    }
    
    // Numbers are equal in value
    return 1;
}

bool compare_bint(BINT* ptrBint1, BINT* ptrBint2) {
    // Ensure the provided pointers are valid
    CHECK_PTR_AND_DEREF(&ptrBint1, "pptrBint1", "compare_bint");
    CHECK_PTR_AND_DEREF(&ptrBint2, "pptrBint2", "compare_bint");

    // Check if the signs of the two BINTs are different
    if ((ptrBint1)->sign ^ (ptrBint2)->sign) {
        return (ptrBint1)->sign < (ptrBint2)->sign;
    }

    // If signs are the same, compare their absolute values
    bool abs_val = compare_abs_bint(ptrBint1, ptrBint2);

    // If both are positive, the one with the greater absolute value is greater
    // If both are negative, the one with the smaller absolute value is greater
    return (ptrBint1)->sign ? !abs_val : abs_val;
}

int BIT_LENGTH(BINT* ptrBint) {
    int bit_len = (ptrBint)->wordlen * WORD_BITLEN;
    for (int i=bit_len-1 ; i>=0;i--){
        if (GET_BIT(ptrBint,i)==0){
            bit_len = bit_len -1;
        }
        else break;
    }
    return bit_len;
}

void left_shift_word(BINT** pptrBint, int shift_amount) {
    CHECK_PTR_AND_DEREF(pptrBint, "pptrBint", "left_shift_word");

    if (shift_amount < 0) {
        fprintf(stderr, "Error: shift_amount is negative in 'left_shift_word'\n");
        return;
    }

    int new_len = (*pptrBint)->wordlen + shift_amount;

    // Reallocate memory for the new word length
    WORD* new_val = (*pptrBint)->val;
    new_val = (WORD*)realloc((*pptrBint)->val, new_len * sizeof(WORD));
    if (!new_val) {
        fprintf(stderr, "Error: Memory reallocation failed in 'left_shift_word'\n");
        exit(1);
    }
    (*pptrBint)->val = new_val; // Assign the possibly new address to ptrX->val

    // Shift the existing values
    for (int i = new_len - 1; i >= shift_amount; i--) {
        (*pptrBint)->val[i] = (*pptrBint)->val[i - shift_amount];
    }

    // Set the newly shifted-in part to zero
    for (int i = 0; i < shift_amount; i++) {
        (*pptrBint)->val[i] = 0x00;
    }

    (*pptrBint)->wordlen = new_len;
}

void right_shift_word(BINT** pptrBint, int shift_amount) {
    CHECK_PTR_AND_DEREF(pptrBint, "pptrBint", "right_shift_word");

    if (shift_amount < 0) {
        fprintf(stderr, "Error: shift_amount is negative in 'right_shift_word'\n");
        return;
    }

    if (shift_amount >= (*pptrBint)->wordlen) {
        // fprintf(stderr, "Error: shift_amount exceeds or equals word length in 'right_shift_word'\n");
        return;
    }

    int new_len = (*pptrBint)->wordlen - shift_amount;

    // Shift the existing values
    for (int i = 0; i < new_len; i++) {
        (*pptrBint)->val[i] = (*pptrBint)->val[i + shift_amount];
    }
    for (int i = new_len; i < (*pptrBint)->wordlen; i++) {
        (*pptrBint)->val[i] = 0;
    }

    // Reallocate memory for the new word length
    WORD* new_val = (*pptrBint)->val;
    new_val = (WORD*)realloc((*pptrBint)->val, new_len * sizeof(WORD));
    if (!new_val) {
        fprintf(stderr, "Error: Memory reallocation failed in 'right_shift_word'\n");
        exit(1);
    }
    (*pptrBint)->val = new_val; // Assign the new address to ptrX->val

    (*pptrBint)->wordlen = new_len;
}

void left_shift_bit(BINT** pptrBint, int shift_amount) {
    if (!(*pptrBint) || !pptrBint) {
        fprintf(stderr, "Parameter is NULL in 'left_shift_bit'\n");
        return; // Invalid parameters or no shift needed.
    }
    if (shift_amount <= 0) {
        // fprintf(stderr, "No shift needed.\n");
        return; // Invalid parameters or no shift needed.
    }

    while (shift_amount > 0) {
        WORD carry = 0;
        for (int i = 0; i < (*pptrBint)->wordlen; ++i) {
            WORD next_carry = ((*pptrBint)->val[i] >> (WORD_BITLEN - 1)) & 1; // Save the bit that will be shifted out.
            (*pptrBint)->val[i] = ((*pptrBint)->val[i] << 1) | carry;
            carry = next_carry;
        }
        if (carry) {
            // We need to increase the size of val to accommodate the new bit.
            WORD* new_val = (*pptrBint)->val;
            new_val = realloc((*pptrBint)->val, ((*pptrBint)->wordlen + 1) * sizeof(WORD));
            if (new_val) {
                (*pptrBint)->val = new_val;
                (*pptrBint)->val[(*pptrBint)->wordlen] = 0; // Initialize the new WORD to zero before setting the carry bit.
                (*pptrBint)->val[(*pptrBint)->wordlen] |= carry; // Add the carried bit in the new WORD.
                (*pptrBint)->wordlen++;
            } else {
                fprintf(stderr, "Memory allocation failure during left shift operation.\n");
                return; // Stop the function upon allocation failure.
            }
        }
        shift_amount--;
    }

}
void right_shift_bit(BINT** pptrBint, int shift_amount) {
    if (!(*pptrBint) || !pptrBint) {
        fprintf(stderr, "Parameter is NULL in 'left_shift_bit'\n");
        return; // Invalid parameters or no shift needed.
    }
    if (shift_amount <= 0) {
        // fprintf(stderr, "No shift needed.\n");
        return; // Invalid parameters or no shift needed.
    }

    while (shift_amount > 0) {
        WORD carry = 0;
        for (int i = (*pptrBint)->wordlen - 1; i >= 0; --i) {
            WORD next_carry = (*pptrBint)->val[i] & 1; // Save the bit that will be shifted out.
            (*pptrBint)->val[i] = ((*pptrBint)->val[i] >> 1) | (carry << (WORD_BITLEN - 1));
            carry = next_carry;
        }
        // No need to check for size reduction of val, as we're shifting right.
        shift_amount--;
    }
}

void reduction(BINT** pptrBint, int pwOf2) {
    if (pwOf2 > BIT_LENGTH(*pptrBint) ) return; // Trivial Case

    if (pwOf2 % WORD_BITLEN == 0 && pwOf2 < BIT_LENGTH(*pptrBint)) {
#if WORD_BITLEN == 8
    WORD* tmp = (*pptrBint)->val;
    tmp = (WORD*)realloc(tmp, pwOf2 / WORD_BITLEN);
    (*pptrBint)->val = tmp;
#elif WORD_BITLEN == 64
    WORD* tmp = (*pptrBint)->val;
    tmp = (WORD*)realloc(tmp, 8 * (pwOf2 / WORD_BITLEN));
    (*pptrBint)->val = tmp;
#else
    WORD* tmp = (*pptrBint)->val;
    tmp = (WORD*)realloc((*pptrBint)->val, 4 * (pwOf2 / WORD_BITLEN));
    (*pptrBint)->val = tmp;
#endif
    (*pptrBint)->wordlen = pwOf2 / WORD_BITLEN;
    return;
    }

    (*pptrBint)->val[pwOf2 / WORD_BITLEN] = (*pptrBint)->val[pwOf2 / WORD_BITLEN] && (0xFF >> (pwOf2 % WORD_BITLEN));

#if WORD_BITLEN == 8
    WORD* tmp = (*pptrBint)->val;
    tmp = (WORD*)realloc(tmp, (pwOf2 / WORD_BITLEN) + 1);
    (*pptrBint)->val = tmp;
#elif WORD_BITLEN == 64
    WORD* tmp = (*pptrBint)->val;
    tmp = (WORD*)realloc(tmp, 8 * (pwOf2 / WORD_BITLEN) + 1);
    (*pptrBint)->val = tmp;
#else
    WORD* tmp = (*pptrBint)->val;
    tmp = (WORD*)realloc((*pptrBint)->val, 4 * (pwOf2 / WORD_BITLEN) + 1);
    (*pptrBint)->val = tmp;
#endif

    (*pptrBint)->wordlen = (pwOf2 / WORD_BITLEN) + 1;
    return;
}

void print_bint_hex_py(const BINT* ptrBint) {
    if (!(ptrBint)) {
        fprintf(stderr, "Invalid BINT* structure in 'print_hex_py'.\n");
        return;
    }
    if (!(ptrBint)->val) {
        fprintf(stderr, "Invalid BINT->val in 'print_hex_py'.\n");
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