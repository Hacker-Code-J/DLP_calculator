#include <stdint.h>
#include <stdio.h>

// Function to count the number of bits in 'a'.
unsigned nbits(uint32_t a) {
    unsigned bits = 0;
    while (a) {
        bits++;
        a >>= 1;
    }
    return bits;
}

// Function to perform binary long division.
void div_binlong(uint32_t a, uint32_t b, uint32_t* q, uint32_t* r) {
    *q = 0;
    *r = 0;
    int n = nbits(a) - 1; // Get the number of bits and start from the highest bit of 'a'

    while (n >= 0) {
        *r = (*r << 1) + ((a >> n) & 1); // Shift 'r' left and add the nth bit of 'a'
        uint32_t t = *r >= b;
        *q += (1u << n) * t; // Add 2^n * t to 'q'
        *r -= b * t; // Subtract b*t from 'r'
        n = n - 1; // Decrement bit position
    }
}

int main() {
    uint32_t a = 0x9C; // Example values
    uint32_t b = 0x2;
    uint32_t q, r;

    div_binlong(a, b, &q, &r);

    printf("Quotient: 0x%X, Remainder: 0x%X\n", q, r);
    return 0;
}
