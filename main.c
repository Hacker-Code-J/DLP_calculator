#include <stdio.h>
#include <stdint.h>

#define NO_BASE_DIGITS 10     // number of base 10 digits the base uses
#define BASE ((uint32_t)0xFFFFFFFF)  // 2^32-1

typedef struct {
    uint32_t digits[NO_BASE_DIGITS];
} BigNum;

BigNum addBigNums(BigNum a, BigNum b) {
    BigNum result = {0};
    uint64_t carry = 0;

    for (int i = 0; i < NO_BASE_DIGITS; i++) {
        uint64_t sum = (uint64_t)a.digits[i] + b.digits[i] + carry;
        result.digits[i] = sum & BASE;
        carry = sum >> 32;
    }

    return result;
}

int main() {
    BigNum a = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
    BigNum b = {{10, 9, 8, 7, 6, 5, 4, 3, 2, 1}};

    BigNum sum = addBigNums(a, b);

    printf("Result: ");
    for (int i = NO_BASE_DIGITS - 1; i >= 0; i--) {
        printf("%u ", sum.digits[i]);
    }

    return 0;
}
