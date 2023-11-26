#include "config.h"

#include <stdbool.h>

#ifndef BIGINT_UTILS_H
#define BIGINT_UTILS_H

/**
 * @brief Exits program if pointer is NULL, indicating an allocation error.
 * @author Ji Yong-Hyeon
 * @date 2023-11-26
 * @param ptr - The pointer to check.
 * @param ptr_name - The name of the pointer variable.
 * @param function_name - The name of the function calling this check.
 * @note This function will terminate the program on a null pointer error.
 */
void exit_on_null_error(const void* ptr, const char* ptr_name, const char* function_name);

/**
 * @brief Frees the memory allocated for a BINT object and sets the pointer to NULL.
 * @author Ji Yong-Hyeon
 * @date 2023-11-26
 * @param pptrBint A pointer to the pointer of the BINT object to be deleted.
 * @note This function should be called to avoid memory leaks.
 */
void delete_bint(BINT** pptrBint);
/**
 * @brief Initializes a BINT object with the specified word length.
 * @author Ji Yong-Hyeon
 * @date 2023-11-26
 * @param pptrBint A pointer to the pointer of the BINT object to be initialized.
 * @param wordlen The word length for the BINT object.
 * @note The BINT object's memory will be dynamically allocated.
 */
void init_bint(BINT** pptrBint, int wordlen);

/**
 * @brief Sets the value of a BINT object to zero.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the pointer of the BINT object to be set to zero.
 * @note Assumes the BINT object has been properly initialized.
 */
void SET_BINT_ZERO(BINT** pptrBint);
/**
 * @brief Sets the value of a BINT object to one (1).
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the pointer of the BINT object to be set to one.
 * @note Assumes the BINT object has been properly initialized.
 */
void SET_BINT_ONE(BINT** pptrBint);

/**
 * @brief Copies the value of one BINT object to another.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint_dst A pointer to the point of the destination BINT object pointer.
 * @param ptrBint_src A pointer to the point of the source BINT object to copy from.
 * @note Assumes that both BINT objects have been properly initialized.
 */
void copyBINT(BINT** pptrBint_dst, const BINT* ptrBint_src);
/**
 * @brief Swaps the values of two BINT objects.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint1 A pointer to the point of the first BINT object pointer.
 * @param pptrBint2 A pointer to the point of the second BINT object pointer.
 * @note Assumes that both BINT objects have been properly initialized.
 */
void swapBINT(BINT** pptrBint1, BINT** pptrBint2);

/**
 * @brief Modifies a BINT object to be an even number by clearing the least significant bit.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrBint A pointer to the BINT object to be modified.
 * @note If the BINT object is already even, the function will have no effect.
 */
void makeEven(BINT* ptrBint);
/**
 * @brief Ensures that two BINT objects have the same size by padding the smaller one with zeros.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrBint1 A pointer to the first BINT object.
 * @param ptrBint2 A pointer to the second BINT object.
 * @note This can be important for operations that require BINT objects of the same size.
 */
void matchSize(BINT* ptrBint1, BINT* ptrBint2);
/**
 * @brief Resets a BINT object to its default state with a value of zero and minimal size.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrBint A pointer to the BINT object to reset.
 * @note This function can be used to reuse a BINT object without reallocating memory.
 */
void resetBINT(BINT* ptrBint);
/**
 * @brief Refines the size of a BINT object to remove any leading zero words.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrBint A pointer to the BINT object to refine.
 * @note This function optimizes memory usage by a BINT object.
 */
void refineBINT(BINT* ptrBint);

/**
 * @brief Checks if a BINT object represents the zero value.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrBint A pointer to the BINT object to check.
 * @return True if the BINT object is zero; otherwise, false.
 * @note This is a quick check and does not modify the BINT object.
 */
bool isZero(BINT* ptrBint);

/**
 * @brief Checks if a BINT object represents the value one (1).
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrBint A pointer to the BINT object to check.
 * @return True if the BINT object is one; otherwise, false.
 * @note This is a quick check and does not modify the BINT object.
 */
bool isOne(BINT* ptrBint);

/**
 * @brief Retrieves the value of a specific bit in a BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the pointer of the BINT object.
 * @param i_th The index of the bit to retrieve.
 * @return True if the bit is set; otherwise, false.
 * @note Indexing starts at 0 and refers to the least significant bit.
 */
bool GET_BIT(BINT** pptrBint, int i_th);

/**
 * @brief Fills an array with random WORD values.
 * @author Your Name
 * @date Date of creation or last update
 * @param dst A pointer to the WORD array to fill.
 * @param wordlen The length of the WORD array.
 * @note The randomness depends on the underlying random number generator.
 */
void RANDOM_ARRARY(WORD* dst, int wordlen);
/**
 * @brief Initializes a BINT object with random values.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the pointer of the BINT object to be initialized.
 * @param sign The sign of the BINT object, where true indicates a negative number.
 * @param wordlen The word length for the BINT object.
 * @note This function is useful for generating random BINT objects for testing.
 */
void RANDOM_BINT(BINT** pptrBint, bool sign, int wordlen);

/**
 * @brief Compares two BINT objects for equality.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint1 A pointer to the first BINT object for comparison.
 * @param pptrBint2 A pointer to the second BINT object for comparison.
 * @return True if the BINT objects are equal; otherwise, false.
 * @note This function compares both value and sign.
 */
bool compare_bint(BINT* pptrBint1, BINT* pptrBint2);
/**
 * @brief Compares the absolute values of two BINT objects for equality.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint1 A pointer to the first BINT object for comparison.
 * @param pptrBint2 A pointer to the second BINT object for comparison.
 * @return True if the absolute values of the BINT objects are equal; otherwise, false.
 * @note This function ignores the signs of the BINT objects.
 */
bool compare_abs_bint(BINT* pptrBint1, BINT* pptrBint2);

/**
 * @brief Calculates the bit length of a BINT object, including leading zeros.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the BINT object.
 * @return The bit length of the BINT object.
 * @note This function includes leading zeros in the bit length calculation.
 */
int BIT_LENGTH(BINT* pptrBint);

/**
 * @brief Calculates the bit length of a BINT object, excluding leading zeros.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the BINT object.
 * @return The bit length of the BINT object, excluding leading zeros.
 * @note Useful for determining the actual number of significant bits.
 */
int BIT_LENGTH_NONZERO(BINT* pptrBint);

/**
 * @brief Performs a left shift operation on a BINT object by a specified number of words.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the BINT object pointer to be shifted.
 * @param shift_amount The number of words to shift the BINT object by.
 * @note This operation is equivalent to multiplying the BINT object by 2^(WORD_SIZE * shift_amount).
 */
void left_shift_word(BINT** pptrBint, int shift_amount);
/**
 * @brief Performs a right shift operation on a BINT object by a specified number of words.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the BINT object pointer to be shifted.
 * @param shift_amount The number of words to shift the BINT object by.
 * @note This operation is equivalent to dividing the BINT object by 2^(WORD_SIZE * shift_amount), without a remainder.
 */
void right_shift_word(BINT** pptrBint, int shift_amount);
/**
 * @brief Performs a left shift operation on a BINT object by a specified number of bits.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the BINT object pointer to be shifted.
 * @param shift_amount The number of bits to shift the BINT object by.
 * @note This operation can increase the size of the BINT object if the shift goes beyond the current most significant bit.
 */
void left_shift_bit(BINT** pptrBint, int shift_amount);
/**
 * @brief Performs a right shift operation on a BINT object by a specified number of bits (possibly a typo for right_shift_bit).
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the BINT object pointer to be shifted.
 * @param shift_amount The number of bits to shift the BINT object by.
 * @note This operation can decrease the size of the BINT object if the shift goes beyond the current least significant bit.
 */
void left_right_bit(BINT** pptrBint, int shift_amount);
/**
 * @brief Reduces a BINT object modulo a power of 2 specified by pwOf2.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the BINT object pointer to be reduced.
 * @param pwOf2 The power of 2 to use as the modulus for the reduction.
 * @note The reduction is done in-place and affects the original BINT object.
 */
void reduction(BINT** pptrBint, int pwOf2);

/**
 * @brief Converts a hexadecimal substring to a WORD value.
 * @author Your Name
 * @date Date of creation or last update
 * @param str The string containing the hexadecimal characters.
 * @param start The starting index of the substring.
 * @param length The length of the substring.
 * @return The WORD value of the hexadecimal substring.
 * @note If the substring is longer than what a WORD can represent, the behavior is undefined.
 */
WORD hexSubstringToWord(const char* str, int start, int length);
/**
 * @brief Converts a hexadecimal string to a BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the pointer of the BINT object to be initialized.
 * @param hexString The hexadecimal string to convert.
 * @note The function initializes the BINT object with the value represented by the hexadecimal string.
 */
void strToBINT(BINT** pptrBint, const char* hexString);

/**
 * @brief Converts a hexadecimal digit to a binary representation and stores it in an array.
 * @author Your Name
 * @date Date of creation or last update
 * @param hex_digit The hexadecimal digit to convert.
 * @param binary A pointer to the array where the binary representation will be stored.
 * @param start_index The starting index in the array where the binary representation begins.
 * @param bits The number of bits to represent the hexadecimal digit.
 * @note Assumes the binary array has enough space to store the representation.
 */
void HexDigitToBinary(WORD hex_digit, bool *binary, int start_index, int bits);

Continuing with the professional annotations for the remaining functions in your C header file:

c
Copy code
/**
 * @brief Performs a left shift operation on a BINT object by a specified number of words.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the BINT object pointer to be shifted.
 * @param shift_amount The number of words to shift the BINT object by.
 * @note This operation is equivalent to multiplying the BINT object by 2^(WORD_SIZE * shift_amount).
 */
void left_shift_word(BINT** pptrBint, int shift_amount);

/**
 * @brief Performs a right shift operation on a BINT object by a specified number of words.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the BINT object pointer to be shifted.
 * @param shift_amount The number of words to shift the BINT object by.
 * @note This operation is equivalent to dividing the BINT object by 2^(WORD_SIZE * shift_amount), without a remainder.
 */
void right_shift_word(BINT** pptrBint, int shift_amount);

/**
 * @brief Performs a left shift operation on a BINT object by a specified number of bits.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the BINT object pointer to be shifted.
 * @param shift_amount The number of bits to shift the BINT object by.
 * @note This operation can increase the size of the BINT object if the shift goes beyond the current most significant bit.
 */
void left_shift_bit(BINT** pptrBint, int shift_amount);

/**
 * @brief Performs a right shift operation on a BINT object by a specified number of bits (possibly a typo for right_shift_bit).
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the BINT object pointer to be shifted.
 * @param shift_amount The number of bits to shift the BINT object by.
 * @note This operation can decrease the size of the BINT object if the shift goes beyond the current least significant bit.
 */
void left_right_bit(BINT** pptrBint, int shift_amount); // Assuming this should be 'right_shift_bit'

/**
 * @brief Reduces a BINT object modulo a power of 2 specified by pwOf2.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the BINT object pointer to be reduced.
 * @param pwOf2 The power of 2 to use as the modulus for the reduction.
 * @note The reduction is done in-place and affects the original BINT object.
 */
void reduction(BINT** pptrBint, int pwOf2);

/**
 * @brief Converts a hexadecimal substring to a WORD value.
 * @author Your Name
 * @date Date of creation or last update
 * @param str The string containing the hexadecimal characters.
 * @param start The starting index of the substring.
 * @param length The length of the substring.
 * @return The WORD value of the hexadecimal substring.
 * @note If the substring is longer than what a WORD can represent, the behavior is undefined.
 */
WORD hexSubstringToWord(const char* str, int start, int length);

/**
 * @brief Converts a hexadecimal string to a BINT object.
 * @author Your Name
 * @date Date of creation or last update
 * @param pptrBint A pointer to the pointer of the BINT object to be initialized.
 * @param hexString The hexadecimal string to convert.
 * @note The function initializes the BINT object with the value represented by the hexadecimal string.
 */
void strToBINT(BINT** pptrBint, const char* hexString);

/**
 * @brief Converts a hexadecimal digit to a binary representation and stores it in an array.
 * @author Your Name
 * @date Date of creation or last update
 * @param hex_digit The hexadecimal digit to convert.
 * @param binary A pointer to the array where the binary representation will be stored.
 * @param start_index The starting index in the array where the binary representation begins.
 * @param bits The number of bits to represent the hexadecimal digit.
 * @note Assumes the binary array has enough space to store the representation.
 */
void HexDigitToBinary(WORD hex_digit, bool *binary, int start_index, int bits);

/**
 * @brief Converts a BINT object's hexadecimal value to a binary array representation.
 * @author Your Name
 * @date Date of creation or last update
 * @param hex A pointer to the BINT object to convert.
 * @return A pointer to the binary array representing the BINT object's value.
 * @note The caller is responsible for freeing the memory allocated for the binary array.
 */
bool* HexToBinary(BINT* hex);
/**
 * @brief Prints the binary representation of an array to standard output.
 * @author Your Name
 * @date Date of creation or last update
 * @param binary A pointer to the binary array to print.
 * @param length The length of the binary array.
 * @note Useful for debugging or displaying the binary form of data.
 */
void PrintBinary(bool* binary, int length);

/**
 * @brief Converts a binary array to a hexadecimal digit starting from a specified index.
 * @author Your Name
 * @date Date of creation or last update
 * @param binary A pointer to the binary array.
 * @param start_index The starting index in the array where the binary digit begins.
 * @param bits The number of bits to use for the conversion.
 * @return The WORD value of the hexadecimal digit.
 * @note Assumes the binary array has at least 'bits' elements starting from 'start_index'.
 */
WORD BinaryToHexDigit(bool *binary, int start_index, int bits);
/**
 * @brief Converts a binary array to a BINT object's hexadecimal representation.
 * @author Your Name
 * @date Date of creation or last update
 * @param binary A pointer to the binary array to convert.
 * @param length The length of the binary array.
 * @return A pointer to the BINT object representing the binary array's value.
 * @note The caller is responsible for managing the memory of the returned BINT object.
 */
BINT* BinaryToHex(bool *binary, int length);

/**
 * @brief Prints the binary representation of a BINT object to standard output.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrBint A pointer to the BINT object to print.
 * @note Useful for debugging or displaying the binary form of a BINT object.
 */
void print_bint_bin(const BINT* ptrBint);
/**
 * @brief Prints the hexadecimal representation of a BINT object to standard output.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrBint A pointer to the BINT object to print.
 * @note Useful for debugging or displaying the hexadecimal form of a BINT object.
 */
void print_bint_hex(const BINT* ptrBint);
/**
 * @brief Prints the binary representation of a BINT object to standard output with a split for readability.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrBint A pointer to the BINT object to print.
 * @note The binary output is split into sections for easier reading.
 */
void print_bint_bin_split(const BINT* ptrBint);
/**
 * @brief Prints the hexadecimal representation of a BINT object to standard output with a split for readability.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrBint A pointer to the BINT object to print.
 * @note The hexadecimal output is split into sections for easier reading.
 */
void print_bint_hex_split(const BINT* ptrBint);
/**
 * @brief Prints the binary representation of a BINT object to standard output in a format compatible with Python lists.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrBint A pointer to the BINT object to print.
 * @note The output format is designed to be directly usable in Python code.
 */
void print_bint_bin_py(const BINT* ptrBint);
/**
 * @brief Prints the hexadecimal representation of a BINT object to standard output in a format compatible with Python lists.
 * @author Your Name
 * @date Date of creation or last update
 * @param ptrBint A pointer to the BINT object to print.
 * @note The output format is designed to be directly usable in Python code.
 */
void print_bint_hex_py(const BINT* ptrBint);

#endif // BIGINT_UTILS_H