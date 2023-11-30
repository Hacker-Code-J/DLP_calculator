#include "config.h"

#include <stdbool.h>

#ifndef UTILS_H
#define UTILS_H

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
 * @brief Generates a random array of WORDs.
 * @details Populates an array with random WORD values, with the array length specified by wordlen.
 * @author Your Name
 * @date Today's Date
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
 * @author Your Name
 * @date Today's Date
 * @param pptrBint Double pointer to the BINT object to be generated.
 * @param sign Boolean indicating the sign of the generated BINT (true for negative, false for non-negative).
 * @param wordlen The word length of the BINT's internal representation.
 * @pre pptrBint must be a valid double pointer to a BINT object.
 * @post pptrBint points to a BINT object with a random value.
 * @note This function allocates memory for the BINT object.
 */
void RANDOM_BINT(BINT** pptrBint, bool sign, int wordlen);

/**
 * @brief Compares two BINT objects for equality.
 * @details Returns true if both BINT objects represent the same value, including their sign.
 * @author Your Name
 * @date Today's Date
 * @param pptrBint1 Pointer to the first BINT object.
 * @param pptrBint2 Pointer to the second BINT object.
 * @pre Both pptrBint1 and pptrBint2 must point to valid BINT objects.
 * @post The BINT objects remain unchanged.
 * @return bool True if both BINT objects are equal, false otherwise.
 */
bool compare_bint(BINT* pptrBint1, BINT* pptrBint2);

/**
 * @brief Compares the absolute values of two BINT objects.
 * @details Returns true if the absolute values of both BINT objects are equal, ignoring their sign.
 * @author Your Name
 * @date Today's Date
 * @param pptrBint1 Pointer to the first BINT object.
 * @param pptrBint2 Pointer to the second BINT object.
 * @pre Both pptrBint1 and pptrBint2 must point to valid BINT objects.
 * @post The BINT objects remain unchanged.
 * @return bool True if the absolute values of both BINT objects are equal, false otherwise.
 */
bool compare_abs_bint(BINT* pptrBint1, BINT* pptrBint2);

/**
 * @brief Calculates the bit length of a BINT object.
 * @details Returns the number of bits required to represent the BINT object, including the sign bit.
 * @author Your Name
 * @date Today's Date
 * @param pptrBint Pointer to the BINT object.
 * @pre pptrBint must point to a valid BINT object.
 * @post The BINT object remains unchanged.
 * @return int The number of bits required to represent the BINT object.
 */
int BIT_LENGTH(BINT* pptrBint);

/**
 * @brief Calculates the bit length of a nonzero BINT object.
 * @details Returns the number of bits required to represent the BINT object, excluding leading zeros and the sign bit.
 *        If the BINT object is zero, returns 0.
 * @author Your Name
 * @date Today's Date
 * @param pptrBint Pointer to the BINT object.
 * @pre pptrBint must point to a valid BINT object.
 * @post The BINT object remains unchanged.
 * @return int The number of bits required to represent the BINT object, excluding leading zeros and the sign bit.
 */
int BIT_LENGTH_NONZERO(BINT* pptrBint);

/**
 * @brief Performs a left shift operation on a BINT object by a specified number of words.
 * @details Shifts the internal representation of the BINT object to the left by a number of words equal to shift_amount,
 *          effectively multiplying the BINT by 2^(word size * shift_amount).
 * @author Your Name
 * @date Today's Date
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
 * @author Your Name
 * @date Today's Date
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
 * @author Your Name
 * @date Today's Date
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
 * @author Your Name
 * @date Today's Date
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
 * @brief Converts a hexadecimal substring to a WORD.
 * @details Extracts a substring of specified length starting at 'start' from a hexadecimal string and converts it to a WORD.
 * @author Your Name
 * @date Today's Date
 * @param str Pointer to the hexadecimal string.
 * @param start The starting index of the substring within the string.
 * @param length The length of the substring to be converted.
 * @pre str must be a valid pointer to a null-terminated hexadecimal string. start and length must specify a valid range within str.
 * @post None.
 * @return WORD The numerical value of the hexadecimal substring.
 */
WORD hexSubstringToWord(const char* str, int start, int length);

/**
 * @brief Converts a hexadecimal string to a BINT object.
 * @details Parses a hexadecimal string and creates a BINT object representing the same numerical value.
 * @author Your Name
 * @date Today's Date
 * @param pptrBint Double pointer to the BINT object to be set.
 * @param hexString Pointer to the null-terminated hexadecimal string.
 * @pre hexString must be a valid hexadecimal string.
 * @post *pptrBint points to a BINT object representing the value of hexString.
 */
void strToBINT(BINT** pptrBint, const char* hexString);

/**
 * @brief Converts a single hexadecimal digit to its binary representation.
 * @details Converts the hexadecimal digit 'hex_digit' into a binary array, starting from 'start_index', covering 'bits' number of bits.
 * @author Your Name
 * @date Today's Date
 * @param hex_digit The hexadecimal digit to convert.
 * @param binary Pointer to the boolean array where the binary representation will be stored.
 * @param start_index The starting index in the binary array for the conversion.
 * @param bits The number of bits to be used for representing the hex digit.
 * @pre binary must be a valid array with enough space to accommodate the binary representation.
 * @post The binary array contains the binary representation of hex_digit starting from start_index.
 */
void HexDigitToBinary(WORD hex_digit, bool *binary, int start_index, int bits);

/**
 * @brief Converts a BINT object representing a hexadecimal number to a binary array.
 * @details Creates a binary array representing the binary form of the hexadecimal number in the BINT object.
 * @author Your Name
 * @date Today's Date
 * @param hex Pointer to the BINT object.
 * @pre hex must point to a valid BINT object representing a hexadecimal number.
 * @post None.
 * @return bool* Pointer to a dynamically allocated array representing the binary form of the hexadecimal number.
 * @note The caller is responsible for freeing the returned array.
 */
bool* HexToBinary(BINT* hex);

/**
 * @brief Prints a binary array.
 * @details Outputs the contents of a binary array to standard output, representing each boolean value as a binary digit.
 * @author Your Name
 * @date Today's Date
 * @param binary Pointer to the boolean array.
 * @param length The number of elements in the binary array.
 * @pre binary must be a valid pointer to a boolean array of at least 'length' elements.
 * @post None.
 */
void PrintBinary(bool* binary, int length);

/**
 * @brief Converts a binary sequence into a hexadecimal digit.
 * @details Converts a sequence of binary values starting at 'start_index' and spanning 'bits' bits into a single hexadecimal digit.
 * @author Your Name
 * @date Today's Date
 * @param binary Pointer to the boolean array containing the binary sequence.
 * @param start_index The starting index in the binary array for the conversion.
 * @param bits The number of bits to be converted into a hexadecimal digit.
 * @pre binary must be a valid array with at least (start_index + bits) elements.
 * @post None.
 * @return WORD The hexadecimal digit representing the specified binary sequence.
 */
WORD BinaryToHexDigit(bool *binary, int start_index, int bits);

/**
 * @brief Converts a binary array into a BINT object representing a hexadecimal number.
 * @details Creates a BINT object representing a hexadecimal number from a binary array of a specified length.
 * @author Your Name
 * @date Today's Date
 * @param binary Pointer to the boolean array representing the binary sequence.
 * @param length The number of elements in the binary array.
 * @pre binary must be a valid pointer to a boolean array of at least 'length' elements.
 * @post None.
 * @return BINT* Pointer to a dynamically allocated BINT object representing the hexadecimal value of the binary sequence.
 * @note The caller is responsible for managing the memory of the returned BINT object.
 */
BINT* BinaryToHex(bool *binary, int length);

/**
 * @brief Prints the binary representation of a BINT object.
 * @details Outputs the binary representation of the value stored in the BINT object pointed to by ptrBint.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint Pointer to the BINT object to be printed.
 * @pre ptrBint must point to a valid, initialized BINT object.
 * @post None.
 * @note This function prints the binary representation directly without any formatting.
 */
void print_bint_bin(const BINT* ptrBint);

/**
 * @brief Prints the hexadecimal representation of a BINT object.
 * @details Outputs the hexadecimal representation of the value stored in the BINT object pointed to by ptrBint.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint Pointer to the BINT object to be printed.
 * @pre ptrBint must point to a valid, initialized BINT object.
 * @post None.
 * @note This function prints the hexadecimal representation directly without any formatting.
 */
void print_bint_hex(const BINT* ptrBint);

/**
 * @brief Prints the binary representation of a BINT object with split digits.
 * @details Outputs the binary representation of the BINT object, with each digit of the binary number printed separately for readability.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint Pointer to the BINT object to be printed.
 * @pre ptrBint must point to a valid, initialized BINT object.
 * @post None.
 * @note The output format is more readable with individual binary digits split.
 */
void print_bint_bin_split(const BINT* ptrBint);

/**
 * @brief Prints the hexadecimal representation of a BINT object with split digits.
 * @details Outputs the hexadecimal representation of the BINT object, with each digit of the hexadecimal number printed separately for readability.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint Pointer to the BINT object to be printed.
 * @pre ptrBint must point to a valid, initialized BINT object.
 * @post None.
 * @note The output format is more readable with individual hexadecimal digits split.
 */
void print_bint_hex_split(const BINT* ptrBint);

/**
 * @brief Prints the binary representation of a BINT object in a Python-friendly format.
 * @details Outputs the binary representation of the BINT object in a format that is compatible with Python's binary notation, starting with '0b'.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint Pointer to the BINT object to be printed.
 * @pre ptrBint must point to a valid, initialized BINT object.
 * @post None.
 * @note Useful for generating binary strings that can be directly used in Python scripts.
 */
void print_bint_bin_py(const BINT* ptrBint);

/**
 * @brief Prints the hexadecimal representation of a BINT object in a Python-friendly format.
 * @details Outputs the hexadecimal representation of the BINT object in a format that is compatible with Python's hexadecimal notation, starting with '0x'.
 * @author Your Name
 * @date Today's Date
 * @param ptrBint Pointer to the BINT object to be printed.
 * @pre ptrBint must point to a valid, initialized BINT object.
 * @post None.
 * @note Useful for generating hexadecimal strings that can be directly used in Python scripts.
 */
void print_bint_hex_py(const BINT* ptrBint);

#endif // UTILS_H