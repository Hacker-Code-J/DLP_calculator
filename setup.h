#ifndef _SETUP_H
#define _SETUP_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

#define false 0
#define true !false

/**
 * Find the maximum of two values
 * @param x1 the first value
 * @param x2 the second value
 * @return the larger element
 */
#define MAX(x1, x2) (x1 > x2 ? x1 : x2)

/**
 * Find the minimum of two values
 * @param x1 the first value
 * @param x2 the second value
 * @return the smaller element
 */
#define MIN(x1, x2) (x1 < x2 ? x1 : x2)

#endif
