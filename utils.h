/**
 * @file utils.h
 * @brief Header file that defines the BINT structure and provides basic operations for manipulating BINT structures.
 */

#ifndef _UTILS_H
#define _UTILS_H

#include "config.h"

#include <stdbool.h>

/**
 * @struct BINT
 * @brief Structure for representing large integers.
 *
 * This structure is used to represent integers that are too large to be
 * handled by standard integer types in C. It provides a way to work with
 * such numbers using an array of smaller 'word' units. A big integer
 * consists of a sign bit, a count of these word units, and a pointer to 
 * the array of word units.
 */
typedef struct {
    bool sign; /**< @brief Sign bit of the big integer.
                    @details This field represents the sign of the big integer.
                    It is set to true for negative numbers and false for 
                    non-negative numbers (including zero). */

    int wordlen; /**< @brief The number of words in the big integer.
                      @details This field holds the count of WORD units that 
                      constitute the big integer. It determines the size and, 
                      consequently, the precision of the big integer. */

    WORD* val; /**< @brief Pointer to the array of words representing the integer value.
                    @details This is a pointer to an array of WORD units, where each WORD 
                    is a part of the entire big integer. The array is used to store 
                    the actual numerical value of the big integer, split into smaller,
                    manageable units. */
} BINT;

/**
 * @brief Terminates program execution if a null pointer is encountered.
 * @details This function checks if a given pointer is NULL. If it is, the function
 *          prints an error message with the pointer's name and the name of the function
 *          that encountered the error, then exits the program.
 * @param ptr Pointer to be checked for NULL.
 * @param ptr_name The name of the pointer variable, used in the error message.
 * @param function_name The name of the function calling this check, used in the error message.
 * @pre ptr and ptr_name must be valid pointers, function_name must be a valid string.
 * @post If ptr is NULL, the program will terminate.
 * @warning Terminates program execution on encountering a NULL pointer.
 */
void exit_on_null_error(const void* ptr, const char* ptr_name, const char* function_name);

/**
 * @brief Macro to check a pointer and its dereferenced value for NULL, and exit on error.
 * @details Calls exit_on_null_error to check both the pointer itself and its dereferenced value for NULL.
 *          If either is NULL, the program prints an error message and exits.
 * @param pptr The double pointer to be checked.
 * @param name The name of the double pointer, used in error messages.
 * @param func The name of the function performing the check.
 * @note This macro is used to ensure both a pointer and its dereferenced value are not NULL.
 * @warning Exits the program if the pointer or its dereferenced value is NULL.
 */
#define CHECK_PTR_AND_DEREF(pptr, name, func) \
    do { \
        exit_on_null_error(pptr, name, func); \
        exit_on_null_error(*pptr, "*" name, func); \
    } while(0)

/**
 * @brief Deletes a dynamically allocated BINT object.
 * @details Frees the memory allocated for a BINT object and sets the pointer to NULL to
 *          prevent dangling pointer issues.
 * @param pptrBint Double pointer to the BINT object to be deleted.
 * @pre pptrBint must point to a valid BINT object.
 * @post pptrBint is set to NULL after deletion.
 * @note This function assumes that the BINT object was dynamically allocated.
 */
void delete_bint(BINT** pptrBint);

/**
 * @brief Initializes a BINT object with a specified word length.
 * @details Allocates memory for a BINT object and initializes it to represent a number with
 *          the given word length.
 * @param pptrBint Double pointer to the BINT object to be initialized.
 * @param wordlen The word length for the BINT's internal representation.
 * @pre wordlen should be a positive integer.
 * @post pptrBint points to an initialized BINT object.
 */
void init_bint(BINT** pptrBint, int wordlen);

/**
 * @brief Copies the value from one BINT object to another.
 * @details The function copies the value of the source BINT object to the destination BINT object.
 * @param pptrBint_dst Double pointer to the destination BINT object.
 * @param pptrBint_src Double pointer to the source BINT object.
 * @pre pptrBint_src must point to a valid BINT object and pptrBint_dst must point to an initialized BINT object.
 * @post pptrBint_dst contains the same value as pptrBint_src.
 */
void copyBINT(BINT** pptrBint_dst, BINT** pptrBint_src);

/**
 * @brief Swaps the values of two BINT objects.
 * @details Exchanges the values between the two provided BINT objects.
 * @param pptrBint1 Double pointer to the first BINT object.
 * @param pptrBint2 Double pointer to the second BINT object.
 * @pre Both pptrBint1 and pptrBint2 must point to valid BINT objects.
 * @post The values of the two BINT objects are swapped.
 */
void swapBINT(BINT** pptrBint1, BINT** pptrBint2);

/**
 * @brief Adjusts the value of a BINT object to be even.
 * @details If the BINT object represents an odd number, this function increments it to the next even number.
 * @param ptrBint Pointer to the BINT object to be modified.
 * @pre ptrBint must point to a valid, initialized BINT object.
 * @post The BINT object represents an even number.
 * @note If the BINT object is already even, the value remains unchanged.
 */
void makeEven(BINT* ptrBint);

/**
 * @brief Matches the size of two BINT objects.
 * @details Adjusts the internal structure of both BINT objects so they both have the same size, facilitating operations on them.
 * @param ptrBint1 Pointer to the first BINT object.
 * @param ptrBint2 Pointer to the second BINT object.
 * @pre Both ptrBint1 and ptrBint2 must point to valid, initialized BINT objects.
 * @post Both BINT objects have the same size.
 */
void matchSize(BINT* ptrBint1, BINT* ptrBint2);

/**
 * @brief Resets a BINT object to its default state.
 * @details Clears the value of the BINT object, setting it back to a default state as if it was just initialized.
 * @param ptrBint Pointer to the BINT object to be reset.
 * @pre ptrBint must point to a valid BINT object.
 * @post The BINT object is in a default, uninitialized state.
 * @note This function does not deallocate memory.
 */
void resetBINT(BINT* ptrBint);

/**
 * @brief Refines the representation of a BINT object.
 * @details Optimizes the internal structure of a BINT object, potentially reducing its size without changing its value.
 * @param ptrBint Pointer to the BINT object to be refined.
 * @pre ptrBint must point to a valid, initialized BINT object.
 * @post The BINT object might have a reduced size but represents the same value.
 */
void refineBINT(BINT* ptrBint);


void refine_BINT_word(BINT* ptrBint, int num_words);

/**
 * @brief Checks if a BINT object represents zero.
 * @details Returns true if the BINT object's value is zero; otherwise, false.
 * @param ptrBint Pointer to the BINT object to be checked.
 * @pre ptrBint must point to a valid BINT object.
 * @post The BINT object is unchanged.
 * @return bool True if the BINT object's value is zero, false otherwise.
 */
bool isZero(BINT* ptrBint);

/**
 * @brief Checks if a BINT object represents one.
 * @details Returns true if the BINT object's value is one; otherwise, false.
 * @param ptrBint Pointer to the BINT object to be checked.
 * @pre ptrBint must point to a valid BINT object.
 * @post The BINT object is unchanged.
 * @return bool True if the BINT object's value is one, false otherwise.
 */
bool isOne(BINT* ptrBint);

/**
 * @brief Retrieves the value of a specified bit in a BINT object.
 * @details Returns the value of the i-th bit of a BINT object as a boolean value.
 * @param ptrBint Double pointer to the BINT object.
 * @param i_th The index of the bit to retrieve.
 * @pre ptrBint must point to a valid BINT object, and i_th must be within the range of the BINT's size.
 * @post The BINT object is unchanged.
 * @return bool The value of the specified bit in the BINT object.
 * @note Bits are indexed starting at 0.
 */
bool GET_BIT(BINT* ptrBint, int i_th);

/**
 * @brief Retrieves the value of a specified word in a BINT object.
 * @details This function returns the value of the m-th word of a BINT (Big Integer) object.
 *          A "word" in this context refers to a segment of the BINT object, typically
 *          representing a fixed number of bits, as defined in the BINT structure.
 *          The function ensures safe access by checking the boundaries of the requested word index.
 * @param ptrBint Pointer to the BINT object.
 * @param m_th The index of the word to retrieve.
 * @pre ptrBint must point to a valid BINT object, and m_th must be a non-negative integer
 *      within the range of the BINT's word length (wordlen).
 * @post The BINT object remains unchanged. The function performs a read-only operation
 *       on the BINT object.
 * @return WORD The value of the specified word in the BINT object. If the index m_th is out
 *         of bounds (either negative or exceeding the word length), the function returns 0.
 * @note Words in a BINT object are indexed starting at 0. The word size and structure are
 *       dependent on the implementation of the BINT type, which represents a large integer 
 *       typically broken into smaller, fixed-size segments or "words" for efficient storage and manipulation.
 */
WORD GET_WORD(BINT* ptrBint, int m_th);

/**
 * @brief Generates a random array of WORDs.
 * @details Populates an array with random WORD values, with the array length specified by wordlen.
 * @param dst Pointer to the destination array where the random WORDs will be stored.
 * @param wordlen The length of the array, indicating how many WORDs will be generated.
 * @pre dst must be a valid pointer to an array of WORDs with at least 'wordlen' elements.
 * @post The array pointed to by dst is filled with random WORD values.
 * @note The randomness depends on the underlying random number generation mechanism.
 */
void RANDOM_ARRAY(WORD* dst, int wordlen);

/**
 * @brief Generates a random BINT object.
 * @details Creates a BINT object with a random value, having a specified word length and sign.
 * @param pptrBint Double pointer to the BINT object to be generated.
 * @param sign Boolean indicating the sign of the generated BINT (true for negative, false for non-negative).
 * @param wordlen The word length of the BINT's internal representation.
 * @pre pptrBint must be a valid double pointer to a BINT object.
 * @post pptrBint points to a BINT object with a random value.
 * @note This function allocates memory for the BINT object.
 */
void RANDOM_BINT(BINT** pptrBint, bool sign, int wordlen);

/**
 * @brief Compares the absolute values of two BINT objects.
 * @details Returns true if the absolute values of both BINT objects are equal, ignoring their sign.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint1 Pointer to the first BINT object.
 * @param ptrBint2 Pointer to the second BINT object.
 * @pre Both ptrBint1 and ptrBint2 must point to valid BINT objects.
 * @post The BINT objects remain unchanged.
 * @return bool True if the absolute values of both BINT objects are equal, false otherwise.
 */
bool compare_abs_bint(BINT* ptrBint1, BINT* ptrBint2);

/**
 * @brief Compares two BINT objects for equality.
 * @details Returns true if both BINT objects represent the same value, including their sign.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint1 Pointer to the first BINT object.
 * @param ptrBint2 Pointer to the second BINT object.
 * @pre Both ptrBint1 and ptrBint2 must point to valid BINT objects.
 * @post The BINT objects remain unchanged.
 * @return bool True if both BINT objects are equal, false otherwise.
 */
bool compare_bint(BINT* pptrBint1, BINT* pptrBint2);

/**
 * @brief Calculates the bit length of a BINT object.
 * @details Returns the number of bits required to represent the BINT object, including the sign bit.
 * @param ptrBint Pointer to the BINT object.
 * @pre pptrBint must point to a valid BINT object.
 * @post The BINT object remains unchanged.
 * @return int The number of bits required to represent the BINT object.
 */
int BIT_LENGTH(BINT* ptrBint);

/**
 * @brief Performs a left shift operation on a BINT object by a specified number of words.
 * @details Shifts the internal representation of the BINT object to the left by a number of words equal to shift_amount,
 *          effectively multiplying the BINT by 2^(word size * shift_amount).
 * @param pptrBint Double pointer to the BINT object to be shifted.
 * @param shift_amount The number of words by which the BINT object will be shifted left.
 * @pre pptrBint must point to a valid BINT object, and shift_amount must be non-negative.
 * @post The BINT object is shifted left by the specified number of words.
 * @note The BINT object might grow in size depending on the shift_amount.
 */
void left_shift_word(BINT** pptrBint, int shift_amount);

/**
 * @brief Performs a right shift operation on a BINT object by a specified number of words.
 * @details Shifts the internal representation of the BINT object to the right by a number of words equal to shift_amount,
 *          effectively dividing the BINT by 2^(word size * shift_amount) and truncating towards zero.
 * @param pptrBint Double pointer to the BINT object to be shifted.
 * @param shift_amount The number of words by which the BINT object will be shifted right.
 * @pre pptrBint must point to a valid BINT object, and shift_amount must be non-negative.
 * @post The BINT object is shifted right by the specified number of words.
 * @note The BINT object might decrease in size depending on the shift_amount.
 */
void right_shift_word(BINT** pptrBint, int shift_amount);

/**
 * @brief Performs a left shift operation on a BINT object by a specified number of bits.
 * @details Shifts the internal representation of the BINT object to the left by a number of bits equal to shift_amount,
 *          effectively multiplying the BINT by 2^shift_amount.
 * @param pptrBint Double pointer to the BINT object to be shifted.
 * @param shift_amount The number of bits by which the BINT object will be shifted left.
 * @pre pptrBint must point to a valid BINT object, and shift_amount must be non-negative.
 * @post The BINT object is shifted left by the specified number of bits.
 */
void left_shift_bit(BINT** pptrBint, int shift_amount);

/**
 * @brief Performs a right shift operation on a BINT object by a specified number of bits.
 * @details Shifts the internal representation of the BINT object to the right by a number of bits equal to shift_amount,
 *          effectively dividing the BINT by 2^shift_amount and truncating towards zero.
 * @param pptrBint Double pointer to the BINT object to be shifted.
 * @param shift_amount The number of bits by which the BINT object will be shifted right.
 * @pre pptrBint must point to a valid BINT object, and shift_amount must be non-negative.
 * @post The BINT object is shifted right by the specified number of bits.
 */
void right_shift_bit(BINT** pptrBint, int shift_amount);

/**
 * @brief Performs a modular reduction of a BINT object by a power of 2.
 * @details Reduces the BINT object modulo 2^pwOf2, effectively keeping the lower pwOf2 bits of the BINT and discarding the rest.
 * @author Your Name
 * @date Today's Date
 * @param pptrBint Double pointer to the BINT object to be reduced.
 * @param pwOf2 The power of 2 that will be used for the modular reduction.
 * @pre pptrBint must point to a valid BINT object, and pwOf2 must be a non-negative integer.
 * @post The BINT object is reduced modulo 2^pwOf2.
 * @note This function is typically used in cryptographic algorithms for modulus operations.
 */
void reduction(BINT** pptrBint, int pwOf2);

/**
 * @brief Prints the hexadecimal representation of a BINT object in a Python-friendly format.
 * @details Outputs the hexadecimal representation of the BINT object in a format that is compatible with Python's hexadecimal notation, starting with '0x'.
 * @param ptrBint Pointer to the BINT object to be printed.
 * @pre ptrBint must point to a valid, initialized BINT object.
 * @post None.
 * @note Useful for generating hexadecimal strings that can be directly used in Python scripts.
 */
void print_bint_hex_py(const BINT* ptrBint);

#endif // _UTILS_H