#ifndef SETUP_H
#define SETUP_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

#define false 0
#define true !false

/**
 * Determine the greater value between two inputs.
 * @param x1 first input value
 * @param x2 second input value
 * @return the larger of the two values
 */
#define MAX(x1, x2) (x1 > x2 ? x1 : x2)

/**
 * Determine the lesser value between two inputs.
 * @param x1 first input value
 * @param x2 second input value
 * @return the smaller of the two values
 */
#define MIN(x1, x2) (x1 < x2 ? x1 : x2)

#endif // SETUP_H
