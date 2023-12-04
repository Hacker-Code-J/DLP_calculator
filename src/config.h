#ifndef _CONFIG_H
#define _CONFIG_H

// Type definitions for standardizing the size across different platforms
typedef unsigned char u8;  // Define a byte (8-bit) as 'u8'
typedef unsigned int u32;  // Define a 32-bit unsigned integer as 'u32'
typedef unsigned long long u64; // Define a 64-bit unsigned integer as 'u64'

// Boolean definitions for readability
#define false 0            // Define 'false' as 0
#define true  1            // Define 'true' as any value not equal to 'false'

// Application-specific flag for algorithm optimization
#define FLAG 8             // Define 'FLAG' for Karatsuba algorithm threshold

// Macros for calculating maximum and minimum of two values
#define MAXIMUM(x1, x2) (x1 > x2 ? x1 : x2) // Return the maximum of x1 and x2
#define MINIMUM(x1, x2) (x1 < x2 ? x1 : x2) // Return the minimum of x1 and x2

// Define the size of a word in bits for the BigInteger implementation
#define WORD_BITLEN 32    // Set the word size for big integers to 32 bits

// Conditional compilation based on the word size for data type flexibility
#if WORD_BITLEN == 8
typedef u8 WORD;          // Define WORD as an unsigned 8-bit integer for 8-bit words
#define WORD_ONE 0x01     // Define WORD_ONE as 1 in 8-bit representation
#elif WORD_BITLEN == 64
typedef u64 WORD;         // Define WORD as an unsigned 64-bit integer for 64-bit words
#define WORD_ONE 0x01LL   // Define WORD_ONE as 1 in 64-bit representation
#else
typedef u32 WORD;         // Default to an unsigned 32-bit integer for the WORD type
#define WORD_ONE 0x01     // Define WORD_ONE as 1 in 32-bit representation
#endif

#endif // _CONFIG_H
