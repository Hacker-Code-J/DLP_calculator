#ifndef _CONFIG_H
#define _CONFIG_H

/**
 * @file config.h
 * @brief Configuration header file containing type definitions, constants, and macros for application-wide use.
 */

/**
 * @typedef u8
 * @brief Type definition for an unsigned 8-bit byte.
 */
typedef unsigned char u8;

/**
 * @typedef u32
 * @brief Type definition for an unsigned 32-bit integer.
 */
typedef unsigned int u32;

/**
 * @typedef u64
 * @brief Type definition for an unsigned 64-bit integer.
 */
typedef unsigned long long u64;

/**
 * @def false
 * @brief Boolean false value defined as 0 for readability.
 */
#define false 0

/**
 * @def true
 * @brief Boolean true value defined as 1 for readability.
 */
#define true  1

/**
 * @def FLAG
 * @brief Application-specific flag for algorithm optimization, e.g., threshold for the Karatsuba algorithm.
 */
#define FLAG 16

/**
 * @def MAXIMUM(x1, x2)
 * @brief Macro to calculate the maximum of two values.
 * @param x1 First value for comparison.
 * @param x2 Second value for comparison.
 * @return Maximum of x1 and x2.
 */
#define MAXIMUM(x1, x2) (x1 > x2 ? x1 : x2)

/**
 * @def MINIMUM(x1, x2)
 * @brief Macro to calculate the minimum of two values.
 * @param x1 First value for comparison.
 * @param x2 Second value for comparison.
 * @return Minimum of x1 and x2.
 */
#define MINIMUM(x1, x2) (x1 < x2 ? x1 : x2)

/**
 * @def WORD_BITLEN
 * @brief Define the size of a word in bits for BigInteger implementation.
 */
#define WORD_BITLEN 32

// Conditional compilation based on the word size for data type flexibility
#if WORD_BITLEN == 8
/**
 * @typedef WORD
 * @brief Type definition for WORD as an unsigned 8-bit integer when using 8-bit words.
 */
typedef u8 WORD;
/**
 * @def WORD_ONE
 * @brief Define WORD_ONE as 1 in an 8-bit representation.
 */
#define WORD_ONE 0x01
#elif WORD_BITLEN == 64
/**
 * @typedef WORD
 * @brief Type definition for WORD as an unsigned 64-bit integer when using 64-bit words.
 */
typedef u64 WORD;
/**
 * @def WORD_ONE
 * @brief Define WORD_ONE as 1 in a 64-bit representation.
 */
#define WORD_ONE 0x01LL
#else
/**
 * @typedef WORD
 * @brief Type definition for WORD as an unsigned 32-bit integer for the default word size.
 */
typedef u32 WORD;
/**
 * @def WORD_ONE
 * @brief Define WORD_ONE as 1 in a 32-bit representation.
 */
#define WORD_ONE 0x01
#endif

#endif // _CONFIG_H
