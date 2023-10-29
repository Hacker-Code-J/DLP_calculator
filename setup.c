#include "setup.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void exit_on_null_error(const void* ptr, const char* ptr_name, const char* function_name) {
    if(!ptr) {
        fprintf(stderr, "Error: '%s' is NULL in '%s'\n", ptr_name, function_name);
        exit(1);
    }
}
