#include "config.h"

#include <stdbool.h>

#ifndef BIGINT_UTILS_H
#define BIGINT_UTILS_H

/**
 * @brief Terminates program execution if a null pointer is encountered.
 * @details This function checks if a given pointer is NULL. If it is, the function
 *          prints an error message with the pointer's name and the name of the function
 *          that encountered the error, then exits the program.
 * @author Your Name
 * @date Today's Date
 * @param ptr Pointer to be checked for NULL.
 * @param ptr_name The name of the pointer variable, used in the error message.
 * @param function_name The name of the function calling this check, used in the error message.
 * @pre ptr and ptr_name must be valid pointers, function_name must be a valid string.
 * @post If ptr is NULL, the program will terminate.
 * @warning Terminates program execution on encountering a NULL pointer.
 */
void exit_on_null_error(const void* ptr, const char* ptr_name, const char* function_name);

/**
 * @brief Deletes a dynamically allocated BINT object.
 * @details Frees the memory allocated for a BINT object and sets the pointer to NULL to
 *          prevent dangling pointer issues.
 * @author Your Name
 * @date Today's Date
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
 * @author Your Name
 * @date Today's Date
 * @param pptrBint Double pointer to the BINT object to be initialized.
 * @param wordlen The word length for the BINT's internal representation.
 * @pre wordlen should be a positive integer.
 * @post pptrBint points to an initialized BINT object.
 */
void init_bint(BINT** pptrBint, int wordlen);

/**
 * @brief Sets a BINT object's value to zero.
 * @details Modifies the BINT object to represent the value zero.
 * @author Your Name
 * @date Today's Date
 * @param pptrBint Double pointer to the BINT object to be modified.
 * @pre pptrBint must point to a valid BINT object.
 * @post The BINT object represents the value zero.
 */
void SET_BINT_ZERO(BINT** pptrBint);

/**
 * @brief Sets a BINT object's value to one.
 * @details Modifies the BINT object to represent the value one.
 * @author Your Name
 * @date Today's Date
 * @param pptrBint Double pointer to the BINT object to be modified.
 * @pre pptrBint must point to a valid BINT object.
 * @post The BINT object represents the value one.
 */
void SET_BINT_ONE(BINT** pptrBint);

/**
 * @brief Copies the value from one BINT object to another.
 * @details The function copies the value of the source BINT object to the destination BINT object.
 * @author Your Name
 * @date Today's Date
 * @param pptrBint_dst Double pointer to the destination BINT object.
 * @param ptrBint_src Pointer to the source BINT object.
 * @pre ptrBint_src must point to a valid BINT object and pptrBint_dst must point to an initialized BINT object.
 * @post pptrBint_dst contains the same value as ptrBint_src.
 */
void copyBINT(BINT** pptrBint_dst, const BINT* ptrBint_src);

/**
 * @brief Swaps the values of two BINT objects.
 * @details Exchanges the values between the two provided BINT objects.
 * @author Your Name
 * @date Today's Date
 * @param pptrBint1 Double pointer to the first BINT object.
 * @param pptrBint2 Double pointer to the second BINT object.
 * @pre Both pptrBint1 and pptrBint2 must point to valid BINT objects.
 * @post The values of the two BINT objects are swapped.
 */
void swapBINT(BINT** pptrBint1, BINT** pptrBint2);

/**
 * @brief Adjusts the value of a BINT object to be even.
 * @details If the BINT object represents an odd number, this function increments it to the next even number.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint Pointer to the BINT object to be modified.
 * @pre ptrBint must point to a valid, initialized BINT object.
 * @post The BINT object represents an even number.
 * @note If the BINT object is already even, the value remains unchanged.
 */
void makeEven(BINT* ptrBint);

/**
 * @brief Matches the size of two BINT objects.
 * @details Adjusts the internal structure of both BINT objects so they both have the same size, facilitating operations on them.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint1 Pointer to the first BINT object.
 * @param ptrBint2 Pointer to the second BINT object.
 * @pre Both ptrBint1 and ptrBint2 must point to valid, initialized BINT objects.
 * @post Both BINT objects have the same size.
 */
void matchSize(BINT* ptrBint1, BINT* ptrBint2);

/**
 * @brief Resets a BINT object to its default state.
 * @details Clears the value of the BINT object, setting it back to a default state as if it was just initialized.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint Pointer to the BINT object to be reset.
 * @pre ptrBint must point to a valid BINT object.
 * @post The BINT object is in a default, uninitialized state.
 * @note This function does not deallocate memory.
 */
void resetBINT(BINT* ptrBint);

/**
 * @brief Refines the representation of a BINT object.
 * @details Optimizes the internal structure of a BINT object, potentially reducing its size without changing its value.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint Pointer to the BINT object to be refined.
 * @pre ptrBint must point to a valid, initialized BINT object.
 * @post The BINT object might have a reduced size but represents the same value.
 */
void refineBINT(BINT* ptrBint);

/**
 * @brief Checks if a BINT object represents zero.
 * @details Returns true if the BINT object's value is zero; otherwise, false.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint Pointer to the BINT object to be checked.
 * @pre ptrBint must point to a valid BINT object.
 * @post The BINT object is unchanged.
 * @return bool True if the BINT object's value is zero, false otherwise.
 */
bool isZero(BINT* ptrBint);

/**
 * @brief Checks if a BINT object represents one.
 * @details Returns true if the BINT object's value is one; otherwise, false.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint Pointer to the BINT object to be checked.
 * @pre ptrBint must point to a valid BINT object.
 * @post The BINT object is unchanged.
 * @return bool True if the BINT object's value is one, false otherwise.
 */
bool isOne(BINT* ptrBint);

/**
 * @brief Retrieves the value of a specified bit in a BINT object.
 * @details Returns the value of the i-th bit of a BINT object as a boolean value.
 * @author Your Name
 * @date Today's Date
 * @param pptrBint Double pointer to the BINT object.
 * @param i_th The index of the bit to retrieve.
 * @pre pptrBint must point to a valid BINT object, and i_th must be within the range of the BINT's size.
 * @post The BINT object is unchanged.
 * @return bool The value of the specified bit in the BINT object.
 * @note Bits are indexed starting at 0.
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