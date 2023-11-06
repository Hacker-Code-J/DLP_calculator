#include "setup.h"
#include "BigInteger.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

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


#define SET_WORD_DATA(PB, V, WL, S) if(*(PB)) delete_bint(PB); *(PB)=(BINT*)malloc(sizeof(BINT));\
    (*PB)->val=(WORD*)calloc(1,sizeof(WORD)); *(*PB)->val=(V); (*PB)->wordlen=(WL); (*PB)->sign=(S)

void SET_BINT_ZERO(BINT** pptrBint) { SET_WORD_DATA(pptrBint, 0x00, 1, false); }
void SET_BINT_ONE(BINT** pptrBint) { SET_WORD_DATA(pptrBint, 0x01, 1, false); }

void SET_BINT_CUSTOM_ZERO(BINT** pptrBint, int num_words) {
    init_bint(pptrBint, num_words);
    for(int i = 0; i < num_words; i++)
        (*pptrBint)->val[i] = 0x00;
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
void copyArray(WORD* ptrValX, WORD* ptrValY, int wordlen) {
    for(int i = 0; i < wordlen; i++)
        ptrValY[i] = ptrValX[i];
}
void assignBINT(BINT** pptrBint_dst, const BINT* ptrBint_src) {
    if(*pptrBint_dst)
        delete_bint(pptrBint_dst);
    init_bint(pptrBint_dst, ptrBint_src->wordlen);
    (*pptrBint_dst)->sign = ptrBint_src->sign;
    copyArray(ptrBint_src->val, (*pptrBint_dst)->val, ptrBint_src->wordlen);
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
    // // Swapping the signs
    // bool tempSign = ptrbint1->sign;
    // ptrbint1->sign = ptrbint1->sign;
    // ptrbint2->sign = tempSign;

    // // Swapping the word lengths
    // int tempWordlen = ptrbint1->wordlen;
    // ptrbint1->wordlen = ptrbint2->wordlen;
    // ptrbint2->wordlen = tempWordlen;

    // // Swapping the pointers
    // WORD* tempVal = ptrbint1->val;
    // ptrbint1->val = ptrbint2->val;
    // ptrbint2->val = tempVal;
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
    int max_wordlen = MAX(ptrBint1->wordlen, ptrBint2->wordlen);

    // Resize ptrBint1 if its wordlen is smaller than max_wordlen
    if(ptrBint1->wordlen < max_wordlen) {
        WORD* tmp = ptrBint1->val;
        tmp = (WORD*)realloc(ptrBint1->val, max_wordlen * sizeof(WORD));
        ptrBint1->val = tmp;
        if (!ptrBint1->val) {
            // Handle memory allocation failure, exit or return an error
            exit(1); 
        }

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
        ptrBint2->val = tmp;
        if (!ptrBint2->val) {
            // Handle memory allocation failure, exit or return an error
            exit(1); 
        }

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

// Function to convert a substring of a hex string to a WORD.
WORD hexSubstringToWord(const char* str, int start, int length) {
    WORD result = 0;
    for (int i = 0; i < length; ++i) {
        result <<= 4;  // Shift left by 4 bits to make space for the next hex digit.
        char c = str[start + i];
        if (c >= '0' && c <= '9') {
            result += c - '0';
        } else if (c >= 'a' && c <= 'f') {
            result += 10 + c - 'a';
        } else if (c >= 'A' && c <= 'F') {
            result += 10 + c - 'A';
        } // No need for else; assuming valid input for this example.
    }
    return result;
}

void strToBINT(BINT** pptrBint, const char* hexString) {
    // Check the string starts with "0x" or "0X".
    if (hexString[0] != '0' || (hexString[1] != 'x' && hexString[1] != 'X')) {
        // Handle error.
        printf("Invalid hex string format.\n");
        return;
    }

    int hexLength = strlen(hexString) - 2; // Minus the "0x".
    init_bint(pptrBint, (hexLength + 7) / 8); // Each WORD has 8 hex characters.

    // Convert each chunk of 8 hex digits to a WORD and store in val.
    int hexStart = 2 + hexLength - 8; // Start from the end, skipping the "0x".
    for (int i = 0; i < (*pptrBint)->wordlen; ++i) {
        int chunkSize = (hexLength >= 8) ? 8 : hexLength;
        (*pptrBint)->val[i] = hexSubstringToWord(hexString, hexStart, chunkSize);
        hexStart -= chunkSize;
        hexLength -= chunkSize;
    }
}

// Function to convert a hex char to its 4-bit binary equivalent
void hexCharToBinary(char hex, char* output) {
    switch (hex) {
        case '0': strcpy(output, "0000"); break;
        case '1': strcpy(output, "0001"); break;
        case '2': strcpy(output, "0010"); break;
        case '3': strcpy(output, "0011"); break;
        case '4': strcpy(output, "0100"); break;
        case '5': strcpy(output, "0101"); break;
        case '6': strcpy(output, "0110"); break;
        case '7': strcpy(output, "0111"); break;
        case '8': strcpy(output, "1000"); break;
        case '9': strcpy(output, "1001"); break;
        case 'A': case 'a': strcpy(output, "1010"); break;
        case 'B': case 'b': strcpy(output, "1011"); break;
        case 'C': case 'c': strcpy(output, "1100"); break;
        case 'D': case 'd': strcpy(output, "1101"); break;
        case 'E': case 'e': strcpy(output, "1110"); break;
        case 'F': case 'f': strcpy(output, "1111"); break;
        default: strcpy(output, "????"); break;  // Error handling
    }
}

void hexToBinary(const char *hex, char *binaryOutput) {
    // Ensure the output buffer is clear
    binaryOutput[0] = '\0';

    char buffer[6];  // Temporary buffer to store 4-bit binary + space + null terminator
    for (int i = 0; i < (int)strlen(hex); i++) {
        hexCharToBinary(hex[i], buffer);
        strcat(binaryOutput, buffer);
        
        // If not the last iteration, append space for separation
        if (i < (int)strlen(hex) - 1) {
            strcat(binaryOutput, " ");
        }
    }
}

/**
 * 
*/
// BINTQueue* createQueue() {
//     BINTQueue* q = (BINTQueue*)malloc(sizeof(BINTQueue));
//     if (!q) return NULL;  // Memory allocation failure

//     q->front = NULL;
//     q->rear = NULL;

//     return q;
// }

// void enqueue(BINTQueue* q, BINT data) {
//     if (!q) return;

//     Node* newNode = (Node*)malloc(sizeof(Node));
//     if (!newNode) return;  // Memory allocation failure

//     newNode->data = data;
//     newNode->next = NULL;

//     if (!q->rear) {
//         q->front = newNode;
//         q->rear = newNode;
//     } else {
//         q->rear->next = newNode;
//         q->rear = newNode;
//     }
// }

// BINT dequeue(BINTQueue* q) {
//     if (!q || !q->front) return BINT_ZERO;

//     Node* tempNode = q->front;
//     BINT data = tempNode->data;

//     q->front = q->front->next;

//     if (!q->front) {
//         q->rear = NULL;  // If the last node was dequeued, rear should also be NULL
//     }

//     free(tempNode);

//     return data;
// }

// BINT peek(BINTQueue* q) {
//     if (!q || !q->front) return BINT_ZERO;
//     return q->front->data;
// }

// bool isempty(BINTQueue* q) {
//     return (q == NULL || q->front == NULL);
// }

// void freeQueue(BINTQueue* q) {
//     while (!isempty(q)) {
//         dequeue(q);
//     }
//     free(q);
// }

/**
 * 
*/
bool store_bint(const char* filename, BINT* b) {
    if(!filename || !b) return false;

    FILE* f = fopen(filename, "wb");
    if(!f) return false;

    fwrite(&b->sign, sizeof(b->sign), 1, f);
    fwrite(&b->wordlen, sizeof(b->wordlen), 1, f);
    fwrite(b->val, sizeof(WORD), b->wordlen, f);

    fclose(f);
    return true;
}
bool multi_store_bints(const char* filename, BINT** bint_array, int num_bints) {
    if(!filename || !bint_array || num_bints <= 0) return false;

    FILE* f = fopen(filename, "wb");
    if(!f) return false;

    fwrite(&num_bints, sizeof(int), 1, f); // Write number of BINTs

    for (int i = 0; i < num_bints; i++) {
        BINT* b = bint_array[i];
        fwrite(&b->sign, sizeof(b->sign), 1, f);
        fwrite(&b->wordlen, sizeof(b->wordlen), 1, f);
        fwrite(b->val, sizeof(WORD), b->wordlen, f);
    }

    fclose(f);
    return true;
}
BINT* load_bint(const char* filename) {
    if(!filename) return NULL;

    FILE* f = fopen(filename, "rb");
    if(!f) return NULL;

    BINT* b = NULL;

    // Read sign and wordlen first
    bool tmp_sign;
    int tmp_wordlen;

    if (fread(&tmp_sign, sizeof(tmp_sign), 1, f) != 1 ||
        fread(&tmp_wordlen, sizeof(tmp_wordlen), 1, f) != 1) {
        fclose(f);
        return NULL;
    }

    // Now, use init_bint with the read wordlen
    init_bint(&b, tmp_wordlen);

    if (!b || !b->val) {
        fclose(f);
        return NULL;
    }

    // Assign the read values to the BINT structure
    b->sign = tmp_sign;
    b->wordlen = tmp_wordlen;

    // Read the val array
    if (fread(b->val, sizeof(WORD), b->wordlen, f) != (long unsigned int)b->wordlen) {
        delete_bint(&b);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return b;
}
BINT** multi_load_bints(const char* filename, int* num_bints) {
    if(!filename) return NULL;

    FILE* f = fopen(filename, "rb");
    if(!f) return NULL;

    int tmp_num_bints;
    if (fread(&tmp_num_bints, sizeof(int), 1, f) != 1) {
        fclose(f);
        return NULL;
    }

    BINT** bint_array = (BINT**)malloc(tmp_num_bints * sizeof(BINT*));
    if (!bint_array) {
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < tmp_num_bints; i++) {
        bool tmp_sign;
        int tmp_wordlen;

        if (fread(&tmp_sign, sizeof(tmp_sign), 1, f) != 1 ||
            fread(&tmp_wordlen, sizeof(tmp_wordlen), 1, f) != 1) {
            for (int j = 0; j < i; j++) {
                delete_bint(&bint_array[j]);
            }
            free(bint_array);
            fclose(f);
            return NULL;
        }

        BINT* b = NULL;
        init_bint(&b, tmp_wordlen);
        if (!b || !b->val) {
            for (int j = 0; j < i; j++) {
                delete_bint(&bint_array[j]);
            }
            free(bint_array);
            fclose(f);
            return NULL;
        }
        b->sign = tmp_sign;
        if (fread(b->val, sizeof(WORD), b->wordlen, f) != (long unsigned int)b->wordlen) {
            delete_bint(&b);
            for (int j = 0; j < i; j++) {
                delete_bint(&bint_array[j]);
            }
            free(bint_array);
            fclose(f);
            return NULL;
        }
        bint_array[i] = b;
    }

    fclose(f);
    *num_bints = tmp_num_bints; // update the number of loaded BINTs
    return bint_array;
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
void print_bint_bin_python(const BINT* ptrBint) {
    if (ptrBint->sign) printf("-");
    printf("0b");
    for (int i = ptrBint->wordlen - 1; i >= 0; --i) {
        for (int j = WORD_BITLEN - 1; j >= 0; --j)
            printf("%d", (ptrBint->val[i] >> j) & 1);
    }
}
void print_bint_hex_python(const BINT* ptrBint) {
    if (ptrBint->sign) { printf("-"); }
    printf("0x");
    for (int i = ptrBint->wordlen - 1; i >= 0; --i) {
        printf("%x", ptrBint->val[i]);
    }
}

// Function to convert a single hexadecimal digit to binary.
void HexDigitToBinary(WORD hex_digit, bool *binary, int start_index, int bits) {
    for (int i = 0; i < bits; i++) {
        binary[start_index + i] = (hex_digit >> (bits - 1 - i)) & 1;
    }
}

// Function to convert a hexadecimal BINT to binary.
bool* HexToBinary(BINT* hex) {
    int bits_per_word = WORD_BITLEN;
    bool *binary = malloc(bits_per_word * hex->wordlen * sizeof(bool));
    if (!binary) {
        fprintf(stderr, "Memory allocation failure in HexToBinary");
        // Handle memory allocation failure.
        return NULL;
    }

    for (int i = 0; i < hex->wordlen; i++) {
        HexDigitToBinary(hex->val[i], binary, i * bits_per_word, bits_per_word);
    }

    return binary;
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
    int bits_per_word = sizeof(WORD) * 8;
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

    for (int i = 0; i < wordlen; i++) {
        hex->val[i] = BinaryToHexDigit(binary, i * bits_per_word, bits_per_word);
    }

    hex->wordlen = wordlen;
    // Assuming the sign is determined elsewhere.
    hex->sign = false;

    return hex;
}

// Helper function to print the binary array.
void PrintBinary(const bool* binary, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%d", binary[i] ? 1 : 0);
        if ((i + 1) % 4 == 0 && (i + 1) != length) {
            printf(" "); // Optional: Print a space every 4 bits for readability.
        }
    }
    printf("\n"); // Print a newline at the end.
}

// Example usage:
// Assuming 'binaryNumber' is a bool* pointing to an array with 'length' elements.
// PrintBinary(binaryNumber, length);


/******************************************************************/
void printHex(BINT* X) {
    printf("[%d] 0x ",X->sign);
    for (int i=X->wordlen-1; i>=0; i--)
        printf("%08x ", X->val[i]);
    printf("\n");
}

void printHex2(BINT* X) {
    printf("0x");
    for (int i=X->wordlen-1; i>=0; i--) {
        printf("%08x", X->val[i]);
    }
    //printf("\n");
}

void custom_printHex(BINT* X, BINT* Y, BINT* Z,  int opt) {
    //option 0 = addition
    //option 1 = subtraction
    //option 2 = multiplication

    char opr;

    switch (opt)
    {
    case 0:
        opr = '+';
        break;
    case 1:
        opr = '-';
        break;
    case 2:
        opr = '*';
        break;
    default:
        opr = '+';
        break;
    }

    int n = (X->wordlen >= Y->wordlen) ? X->wordlen:Y->wordlen;
    //print X 
    printf("  [%d] 0x ", X->sign);
    for(int i=0; i<(n-X->wordlen); i++)
        printf("%08x ", 0);
    for (int i=X->wordlen-1; i>=0; i--)
        printf("%08x ", X->val[i]);
    printf("\n");

    //print Y
    printf("%c [%d] 0x ", opr, Y->sign);
    for(int i=0; i<(n-Y->wordlen); i++)
        printf("%08x ", 0);
    for (int i=Y->wordlen-1; i>=0; i--)
        printf("%08x ", Y->val[i]);
    printf("\n");

    //print line
    printf("--------");
    for(int i=0; i<n; i++) {
        printf("-");
        printf("--------");
    } printf("\n");

    //print Z
    printf("  [%d] 0x ", Z->sign);
    for(int i=0; i<n-Z->wordlen; i++)
        printf("%08x ", 0);
    for (int i=Z->wordlen-1; i>=0; i--)
        printf("%08x ", Z->val[i]);
    printf("\n\n");
}

void custom_printHex_xy(BINT* X, BINT* Y, int n) {
    //print X 
    printf("x: [%d] 0x ",X->sign);
    for(int i=0; i<(n-X->wordlen); i++)
        printf("%08x ", 0);
    for (int i=X->wordlen-1; i>=0; i--)
        printf("%08x ", X->val[i]);
    printf("\n");

    //print Y
    printf("y: [%d] 0x ", Y->sign);
    for(int i=0; i<(n-Y->wordlen); i++)
        printf("%08x ", 0);
    for (int i=Y->wordlen-1; i>=0; i--)
        printf("%08x ", Y->val[i]);
    printf("\n\n");
}

void refine_BINT(BINT* X) {
    if(X == NULL) return;

    int new_wordlen = X->wordlen;
    while (new_wordlen > 1) { // at least one word needed
        if(X->val[new_wordlen-1] != 0)
            break;
        new_wordlen--;
    }
    if(X->wordlen != new_wordlen) {
        X->wordlen = new_wordlen;
        WORD* tmp = X->val;
        tmp = (WORD*)realloc(X->val, sizeof(WORD)*new_wordlen);
        X->val = tmp;
    }

    if((X->wordlen == 1) && (X->val[0] == 0))
        X->sign = false;
}


void refine_BINT_word(BINT* ptrX, int num_words) {
    if(ptrX == NULL) return;

    int new_wordlen = ptrX->wordlen - num_words;
    if(ptrX->wordlen != new_wordlen) {
        ptrX->wordlen = new_wordlen;
        WORD* tmp = ptrX->val;
        tmp = (WORD*)realloc(ptrX->val, sizeof(WORD)*new_wordlen);
        ptrX->val = tmp;
    }

    if((ptrX->wordlen == 1) && (ptrX->val[0] == 0))
        ptrX->sign = false;
}

//Generate Random BINT

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

/**
 * @brief Compare the absolute values of two BINT numbers.
 *
 * This function performs a comparison of the absolute values of two given BINT numbers.
 * If the absolute value of the number pointed by pptrX is greater than or equal to 
 * the number pointed by pptrY, it returns true (1); otherwise, it returns false (0).
 *
 * Error checks are performed at the beginning to ensure valid pointers are passed.
 * The comparison starts by looking at the word lengths of the numbers, 
 * and if they're equal, it then performs a word-by-word comparison.
 *
 * @param pptrX Double pointer to the first BINT number.
 * @param pptrY Double pointer to the second BINT number.
 * 
 * @return Returns true (1) if abs(*pptrX) >= abs(*pptrY), otherwise false (0).
 */
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

//Author: Moon Ye-chan
int Get_bitlen(BINT* x){
    int bitlen =  (sizeof(x->val)*8);
    return bitlen;
}

int Get_sign(BINT* x){
    return x->sign;
}

// void Flip_sign(BINT* x){
//     if (x->sign == 0){
//         x->sign = -1;
//     }//양수면 음수로
//     if (x->sign == -1){
//         x->sign = 0;
//     }//음수면 양수로 부호 바꾸기
// }

void FLIP_SIGN(BINT** pptrBint) {
    if((*pptrBint)->sign)
        (*pptrBint)->sign = false;
    if(!((*pptrBint)->sign))
        (*pptrBint)->sign = true;
}

int BIT_LENGTH(BINT** pptrBint) {
    return (*pptrBint)->wordlen * WORD_BITLEN;
}

// //Author: Kim Ye-chan
// void MUL_Shift(BINT* X, BINT* result , int N) {
//     int q = N / 32;
//     int r = N % 32;

//     if (r == 0) {
//         for (int i = X->wordlen-1; i>=0; i--) {
//             result->val[i+q] = X->val[i];
            
//         }
//         for(int i=0; i<q; i++){
//             result->val[i] = 0;
            
//         }
        
//     } 
//     else {
//         // r이 0이 아닌 경우 처리

//         for (int i = X->wordlen; i>=0; i--) {
//             if(i==X->wordlen){
//                 result->val[i] = (X->val[i-1] >> (32-r));                
//                 continue; 
//             }
//             else if(i < X->wordlen && i > 0){
//                 result->val[i] = (X->val[i-1] >> (32-r)) | (X->val[i] << r);
//                 continue;
//             }

//             else if(i == 0){
//                 result->val[i] = (X->val[i] << r);
                
//             }
//         }
//         if (q > 0) {
//             for (int i = result->wordlen-1; i>=0; i--) {
//                 result->val[i+q] = result->val[i];
//             }
//             for(int i=0; i<q; i++){
//                 result->val[i] = 0;
//             }
//         }         
//     }
//     // for (int i = result->wordlen - 1 ; i >= 0  ; i--) {
//     // printf("%08x ", result->val[i]);
//     // }


// } 

// void DIV_Shift(BINT* X, BINT* result, int N) {
//     int q = N / 32;
//     int r = N % 32;

//     if (r == 0) {
//         for (int i = 0; i < X->wordlen - q; i++) {
//             result->val[i] = X->val[i + q];
//         }

//     } else {
//         for (int i = 0; i < X->wordlen; i++) {
//             if (i < X->wordlen - 1) {
//                 result->val[i] = (X->val[i + 1] << (32 - r)) | (X->val[i] >> r);
//             } else {
//                 result->val[i] = X->val[i] >> r;
//             }
//         }
//         for (int i = 0; i < result->wordlen - q; i++) {
//             result->val[i] = result->val[i + q];
//         }
//     }
//     for (int i = result->wordlen - 1 ; i >= 0  ; i--) {
//     printf("%08x ", result->val[i]);
//     }
// }

void left_shift(BINT** pptrbint, int num_bits) {
    // if (!pptrbint || !*pptrbint) return;
    exit_on_null_error(pptrbint, "pptrbint", "left_shift");
    exit_on_null_error(*pptrbint, "*pptrbint", "left_shift");

    BINT* ptrbint = *pptrbint;

    // WORD_BITLEN = sizeof(WORD) * 8
    int word_shift = num_bits / WORD_BITLEN;  // Number of whole WORDs to shift
    int bit_shift = num_bits % WORD_BITLEN;   // Number of bits within a WORD to shift

    // Allocating new memory for the shifted value
    WORD* new_val = (WORD*)calloc(ptrbint->wordlen + word_shift + 1, sizeof(WORD)); // +1 for potential overflow

    if (!new_val) {
        // Memory allocation failed
        exit(1);
    }

    // Handle the shift within a WORD
    for (int i = 0; i < ptrbint->wordlen; i++) {
        new_val[i + word_shift] |= ptrbint->val[i] << bit_shift;
        if (i + word_shift + 1 < ptrbint->wordlen + word_shift + 1) {
            new_val[i + word_shift + 1] |= ptrbint->val[i] >> (WORD_BITLEN - bit_shift);
        }
    }

    // Free old memory and assign new memory to the BINT
    free(ptrbint->val);
    ptrbint->val = new_val;
    ptrbint->wordlen = ptrbint->wordlen + word_shift + 1; // Update wordlen
}

// NOTE: Still, be careful with memory management. You'll need to eventually free the val in BINT.

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


void right_shift(BINT** pptrX, int num_bits) {
    // if (!pptrX || !*pptrX) return;
    exit_on_null_error(pptrX, "pptrX", "right_shift");
    exit_on_null_error(*pptrX, "*pptrX", "right_shift");

    BINT* ptrX = *pptrX;

    int word_shift = num_bits / WORD_BITLEN;  // Number of whole WORDs to shift
    int bit_shift = num_bits % WORD_BITLEN;   // Number of bits within a WORD to shift

    // If the right shift is greater than the total length of our BINT, set it to zero
    if (word_shift >= ptrX->wordlen) {
        free(ptrX->val);
        ptrX->val = (WORD*)calloc(1, sizeof(WORD)); // Allocate memory for a single WORD set to 0
        if (!ptrX->val) {
            // Memory allocation failed
            exit(1);
        }
        ptrX->wordlen = 1;
        return;
    }

    // Allocate new memory for the shifted value
    int new_wordlen = ptrX->wordlen - word_shift;
    WORD* new_val = (WORD*)calloc(new_wordlen, sizeof(WORD));

    if (!new_val) {
        // Memory allocation failed
        exit(1);
    }

    // Handle the shift within a WORD
    for (int i = 0; i < new_wordlen; i++) {
        new_val[i] = ptrX->val[i + word_shift] >> bit_shift;
        if (i + word_shift + 1 < ptrX->wordlen) {
            new_val[i] |= ptrX->val[i + word_shift + 1] << (WORD_BITLEN - bit_shift);
        }
    }

    // Free old memory and assign new memory to the BINT
    free(ptrX->val);
    ptrX->val = new_val;
    ptrX->wordlen = new_wordlen;
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
    if (!ptrBint || shift_amount <= 0) {
        fprintf(stderr, "Invalid parameters or no shift needed.");
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

    // WORD carry = 0;
    // int shift_word = shift_amount / WORD_BITLEN;
    // int shift_bit = shift_amount % WORD_BITLEN;

    // // Perform bit-level shifts.
    // for (int i = 0; i < ptrBint->wordlen; ++i) {
    //     WORD next_carry = ptrBint->val[i] << (WORD_BITLEN - shift_bit);
    //     ptrBint->val[i] = (ptrBint->val[i] >> shift_bit) | carry;
    //     carry = next_carry;
    // }

    // // Zero-fill the upper words if there is a word-level shift.
    // for (int i = ptrBint->wordlen - 1; i >= ptrBint->wordlen - shift_word; --i) {
    //     ptrBint->val[i] = 0;
    // }

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