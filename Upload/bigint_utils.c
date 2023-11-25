#include "config.h"
#include "bigint_utils.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void exit_on_null_error(const void* ptr, const char* ptr_name, const char* function_name) {
    if(!ptr) {
        fprintf(stderr, "Error: '%s' is NULL in '%s'\n", ptr_name, function_name);
        exit(1);
    }
}
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