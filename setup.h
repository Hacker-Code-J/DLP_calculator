#ifndef SETUP_H
#define SETUP_H

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long u64;

#define false 0
#define true !false
#define FLAG 8 // Karatsuba Depth

/**
 * Determine the greater value between two inputs.
 * @param x1 first input value
 * @param x2 second input value
 * @return the larger of the two values
 */
#define MAXIMUM(x1, x2) (x1 > x2 ? x1 : x2)

/**
 * Determine the lesser value between two inputs.
 * @param x1 first input value
 * @param x2 second input value
 * @return the smaller of the two values
 */
#define MINIMUM(x1, x2) (x1 < x2 ? x1 : x2)

/** Print Error
 * Error: 'ptr_name' is NULL in 'function_name'
*/
void exit_on_null_error(const void* ptr, const char* ptr_name, const char* function_name);

#define CHECK_PTR_AND_DEREF(ptr, name, func) \
    do { \
        exit_on_null_error(ptr, name, func); \
        exit_on_null_error(*ptr, "*" name, func); \
    } while(0)

#define CHECK_PTR_DEREF_AND_VAL(ptr, name, func) \
    do { \
        exit_on_null_error(ptr, name, func); \
        exit_on_null_error(*ptr, "*" name, func); \
        if(*ptr) exit_on_null_error((*ptr)->val, "(*" name ")->val", func); \
    } while(0)


#endif // SETUP_H
