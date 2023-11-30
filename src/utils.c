#include "config.h"
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

/**
 * @brief Macro to check a pointer and its dereferenced value for NULL, and exit on error.
 * @details Calls exit_on_null_error to check both the pointer itself and its dereferenced value for NULL.
 *          If either is NULL, the program prints an error message and exits.
 * @param ptr The double pointer to be checked.
 * @param name The name of the double pointer, used in error messages.
 * @param func The name of the function performing the check.
 * @note This macro is used to ensure both a pointer and its dereferenced value are not NULL.
 * @warning Exits the program if the pointer or its dereferenced value is NULL.
 */
#define CHECK_PTR_AND_DEREF(ptr, name, func) \
    do { \
        exit_on_null_error(ptr, name, func); \
        exit_on_null_error(*ptr, "*" name, func); \
    } while(0)

/**
 * @brief Macro to check a pointer, its dereferenced value, and a nested value for NULL, and exit on error.
 * @details Calls exit_on_null_error to check the pointer, its dereferenced value, and a specific nested value within
 *          the dereferenced pointer for NULL. If any are NULL, the program prints an error message and exits.
 * @param ptr The triple pointer to be checked.
 * @param name The name of the triple pointer, used in error messages.
 * @param func The name of the function performing the check.
 * @note This macro is used to ensure a pointer, its dereferenced value, and a nested value are not NULL.
 * @warning Exits the program if any of the checked values are NULL.
 */
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

bool GET_BIT(BINT** pptrBint, int i_th)  {
   if (i_th >= WORD_BITLEN)
      return (((*pptrBint)->val[i_th / WORD_BITLEN] >> (i_th % WORD_BITLEN)) & 1);

   return (((*pptrBint)->val[0] >> i_th) & 1);
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
    refine_BINT(*pptrBint);
}

bool compare_abs_bint(BINT** pptrX, BINT** pptrY) {
    // Ensure the provided pointers are valid
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "compare_abs_bint");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "compare_abs_bint");

    // Extract word lengths for both numbers
    int n = (*pptrX)->wordlen; int m = (*pptrY)->wordlen;

    // Compare the word lengths of the two numbers
    if(n > m) return 1;
    if(n < m) return 0;
    
    // Perform a word-by-word comparison starting from the most significant word
    matchSize(*pptrX, *pptrY);
    for(int i = (*pptrX)->wordlen - 1; i >= 0; i--) {
        if((*pptrX)->val[i] > (*pptrY)->val[i]) return 1;
        if((*pptrX)->val[i] < (*pptrY)->val[i]) return 0;
    }
    // Numbers are equal in value
    return 1;
}

bool compare_bint(BINT** pptrBint1, BINT** pptrBint2) {
    // Ensure the provided pointers are valid
    CHECK_PTR_AND_DEREF(pptrBint1, "pptrBint1", "compare_bint");
    CHECK_PTR_AND_DEREF(pptrBint2, "pptrBint2", "compare_bint");

    // If one is negative and the other positive, the positive one is greater
    if ((*pptrBint1)->sign ^ (*pptrBint2)->sign) {
        return (*pptrBint1)->sign < (*pptrBint2)->sign;
    }

    // If both have the same sign, compare their absolute values
    bool abs_val = compare_abs_bint(pptrBint1, pptrBint2);

    // If both are positive, the one with the greater absolute value is greater
    // If both are negative, the one with the smaller absolute value is greater
    return (*pptrBint1)->sign ? !abs_val : abs_val;
}

int BIT_LENGTH(BINT** pptrBint) {
    int bit_len = (*pptrBint)->wordlen * WORD_BITLEN;
    // for (int i=bit_len-1 ; i>=0;i--){
    //     if (GET_BIT(pptrBint,i)==0){
    //         bit_len = bit_len -1;
    //     }
    //     else break;
    // }
    return bit_len;
}

int BIT_LENGTH_NONZERO(BINT** pptrBint) {
    int bit_len = (*pptrBint)->wordlen * WORD_BITLEN;
    for (int i=bit_len-1 ; i>=0;i--){
        if (GET_BIT(pptrBint,i)==0){
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

    // // Efficiently set the shifted part to zero and update the word length
    // memset(ptrX->val, 0x00, shift_amount * sizeof(WORD)); // Setting initial shift_amount elements to zero
    // memmove(ptrX->val + shift_amount, ptrX->val, (new_len - shift_amount) * sizeof(WORD)); // Shifting the values to their new positions

    // ptrX->wordlen = new_len;
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
        (*pptrBint)->val[i] = 0x00;
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

    // Alternatively, for efficiency:
    // memmove(ptrX->val, ptrX->val + shift_amount, new_len * sizeof(WORD)); // Shifting the values to their new positions
    // ptrX->val = (WORD*)realloc(ptrX->val, new_len * sizeof(WORD)); // Adjust memory allocation
    // ptrX->wordlen = new_len;
}

void left_shift_bit(BINT* ptrBint, int shift_amount) {
    if (!ptrBint) {
        fprintf(stderr, "Parameter is NULL in 'left_shift_bit'\n");
        return; // Invalid parameters or no shift needed.
    }
    if (shift_amount <= 0) {
        // fprintf(stderr, "No shift needed.\n");
        return; // Invalid parameters or no shift needed.
    }

    // WORD carry = 0;
    // int shift_word = shift_amount / WORD_BITLEN;
    // int shift_bit = shift_amount % WORD_BITLEN;

    // // Handle word-level shifts if necessary.
    // if (shift_word > 0) {
    //     // Allocate new space for the increased val array.
    //     WORD* new_val = ptrBint->val;
    //     new_val = realloc(ptrBint->val, (ptrBint->wordlen + shift_word) * sizeof(WORD));
    //     if (!new_val) {
    //         // Handle memory allocation error.
    //         return;
    //     }
    //     ptrBint->val = new_val;
    //     // Move words in the array.
    //     memmove(ptrBint->val + shift_word, ptrBint->val, ptrBint->wordlen * sizeof(WORD));
    //     // Zero-fill the lower words.
    //     memset(ptrBint->val, 0, shift_word * sizeof(WORD));
    //     ptrBint->wordlen += shift_word;
    // }

    // // Perform bit-level shifts.
    // for (int i = ptrBint->wordlen - 1; i >= shift_word; --i) {
    //     WORD next_carry = ptrBint->val[i] >> (WORD_BITLEN - shift_bit);
    //     ptrBint->val[i] = (ptrBint->val[i] << shift_bit) | carry;
    //     carry = next_carry;
    // }

    while (shift_amount > 0) {
        WORD carry = 0;
        for (int i = 0; i < ptrBint->wordlen; ++i) {
            WORD next_carry = (ptrBint->val[i] >> (WORD_BITLEN - 1)) & 1; // Save the bit that will be shifted out.
            ptrBint->val[i] = (ptrBint->val[i] << 1) | carry;
            carry = next_carry;
        }
        if (carry) {
            // We need to increase the size of val to accommodate the new bit.
            WORD* new_val = ptrBint->val;
            new_val = realloc(ptrBint->val, (ptrBint->wordlen + 1) * sizeof(WORD));
            // ptrBint->val = new_val;
            if (new_val) {
                ptrBint->val = new_val;
                ptrBint->val[ptrBint->wordlen] = 0; // Initialize the new WORD to zero before setting the carry bit.
                ptrBint->val[ptrBint->wordlen] |= carry; // Add the carried bit in the new WORD.
                ptrBint->wordlen++;
                // ptrBint->val = new_val;
                // ptrBint->val[ptrBint->wordlen] = carry; // Add the carried bit in the new WORD.
                // ptrBint->wordlen++;
            } else {
                fprintf(stderr, "Memory allocation failure during left shift operation.\n");
                return; // Stop the function upon allocation failure.
            }
        }
        shift_amount--;
    }

}
void right_shift_bit(BINT* ptrBint, int shift_amount) {
    if (!ptrBint || shift_amount <= 0) {
        fprintf(stderr, "Invalid parameters or no shift needed.");
        return; // Invalid parameters or no shift needed.
    }

    while (shift_amount > 0) {
        WORD carry = 0;
        for (int i = ptrBint->wordlen - 1; i >= 0; --i) {
            WORD next_carry = ptrBint->val[i] & 1; // Save the bit that will be shifted out.
            ptrBint->val[i] = (ptrBint->val[i] >> 1) | (carry << (WORD_BITLEN - 1));
            carry = next_carry;
        }
        // No need to check for size reduction of val, as we're shifting right.
        shift_amount--;
    }
}

void reduction(BINT** pptrBint, int pwOf2) {
    if (pwOf2 > BIT_LENGTH(pptrBint) ) return; // Trivial Case

    if (pwOf2 % WORD_BITLEN == 0 && pwOf2 < BIT_LENGTH(pptrBint)) {
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

// Function to convert a single hexadecimal digit to binary, stored backwards.
void HexDigitToBinary(WORD hex_digit, bool *binary, int start_index, int bits) {
    for (int i = 0; i < bits; i++) {
        // Store bits in reverse order.
        binary[start_index + bits - 1 - i] = (hex_digit >> i) & 1;
    }
}

// Function to convert a hexadecimal BINT to binary, with bits stored backwards.
bool* HexToBinary(BINT* hex) {
    int bits_per_word = WORD_BITLEN;
    bool *binary = malloc(bits_per_word * hex->wordlen * sizeof(bool));
    if (!binary) {
        fprintf(stderr, "Memory allocation failure in HexToBinary");
        // Handle memory allocation failure.
        return NULL;
    }

    for (int i = 0; i < hex->wordlen; i++) {
        // The start index is calculated to fill the array from the end.
        HexDigitToBinary(hex->val[i], binary, (hex->wordlen - 1 - i) * bits_per_word, bits_per_word);
    }

    return binary;
}

// Helper function to print the binary array.
void PrintBinary(bool* binary, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%d", binary[i] ? 1 : 0);
        if ((i + 1) % 16 == 0 && (i + 1) != length) {
            printf(" "); // Optional: Print a space every 4 bits for readability.
        }
    }
    printf("\n"); // Print a newline at the end.
}

// Function to convert a binary digit array to a single hexadecimal WORD.
WORD BinaryToHexDigit(bool *binary, int start_index, int bits) {
    WORD hex_digit = 0;
    for (int i = 0; i < bits; i++) {
        hex_digit |= (binary[start_index + i] << (bits - 1 - i));
    }
    return hex_digit;
}

// Function to convert binary BINT to hexadecimal.
BINT* BinaryToHex(bool *binary, int length) {
    int bits_per_word = WORD_BITLEN;
    int wordlen = (length + bits_per_word - 1) / bits_per_word;
    BINT *hex = malloc(sizeof(BINT));
    if (!hex) {
        // Handle memory allocation failure.
        return NULL;
    }
    hex->val = malloc(wordlen * sizeof(WORD));
    if (!hex->val) {
        // Handle memory allocation failure.
        free(hex);
        return NULL;
    }

    // for (int i = 0; i < wordlen; i++) {
    //     hex->val[i] = BinaryToHexDigit(binary, i*bits_per_word, bits_per_word);
    // }
    // Reverse the order in which WORDs are stored in hex->val.
    for (int i = 0; i < wordlen; i++) {
        int bits_to_convert = bits_per_word;
        // Adjust for the last WORD if the binary array is not a multiple of bits_per_word.
        if (i == wordlen - 1 && length % bits_per_word != 0) {
            bits_to_convert = length % bits_per_word;
        }
        // Store WORDs in reverse order.
        hex->val[wordlen - 1 - i] = BinaryToHexDigit(binary, i * bits_per_word, bits_to_convert);
    }


    hex->wordlen = wordlen;
    // Assuming the sign is determined elsewhere.
    hex->sign = false;

    return hex;
}

void print_bint_bin(const BINT* ptrBint) {
    if (ptrBint->sign) printf("-");
    // printf("0b");
    for (int i = ptrBint->wordlen - 1; i >= 0; --i) {
        for (int j = WORD_BITLEN - 1; j >= 0; --j)
            printf("%d", (ptrBint->val[i] >> j) & 1);
    }
    printf("\n");
}
void print_bint_hex(const BINT* ptrBint) {
    if (ptrBint->sign) { printf("-"); }
    // printf("0x");
    for (int i = ptrBint->wordlen - 1; i >= 0; --i) {
        printf("%x", ptrBint->val[i]);
    }
    printf("\n");
}
void print_bint_bin_split(const BINT* ptrBint) {
    if (ptrBint->sign) printf("-");
    printf("0b ");
    for (int i = ptrBint->wordlen - 1; i >= 0; --i) {
     for (int j = WORD_BITLEN - 1; j >= 0; --j) {
            printf("%d", (ptrBint->val[i] >> j) & 1);
        }
        if (i > 0) {
            printf(" ");
        }
    }
    printf("\n");
}
void print_bint_hex_split(const BINT* ptrBint) {
    if (ptrBint->sign) { printf("-"); }
    printf("0x ");
    for (int i = ptrBint->wordlen - 1; i >= 0; --i) {
        printf("%x ", ptrBint->val[i]);
    }
    printf("\n");
}
void print_bint_bin_python(BINT* ptrBint) {
    if ((ptrBint)->sign) printf("-");
    printf("0b");
    for (int i = (ptrBint)->wordlen - 1; i >= 0; i--) {
        for (int j = WORD_BITLEN - 1; j >= 0; j--)
            printf("%d", ((ptrBint)->val[i] >> j) & 1);
    }
}
void print_bint_hex_python(BINT** pptrBint) {
    if (!pptrBint) {
        fprintf(stderr, "Invalid BINT** structure in 'print_hex_python'.\n");
        return;
    }
    if (!(*pptrBint)) {
        fprintf(stderr, "Invalid BINT* structure in 'print_hex_python'.\n");
        return;
    }
    if (!(*pptrBint)->val) {
        fprintf(stderr, "Invalid BINT->val in 'print_hex_python'.\n");
        return;
    }
    if ((*pptrBint)->sign) { printf("-"); }
    printf("0x");
    
    for (int i = (*pptrBint)->wordlen - 1; i >= 0; i--) {
        printf("%08x", (*pptrBint)->val[i]);
        // if(i >= 3) {  // Assuming MAX_WORDLEN is the maximum size of the val array
        //     fprintf(stderr, "Word length exceeds maximum allowed length.\n");
        //     return;
        // }
    }
}