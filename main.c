/**
 * @mainpage PUBAO Big Integer Library Documentation
 *
 * @section intro Introduction
 * The PUBAO Big Integer Library provides an implementation for handling very large integers 
 * that are beyond the capacity of standard C integer types. This library is useful in 
 * scenarios such as cryptographic computations, high precision calculations, and 
 * anywhere standard integer types are insufficient due to their size limitations.
 *
 * This documentation covers the usage, functionalities, and implementation details of 
 * the Big Integer Library. It is intended for developers who need to integrate large 
 * integer operations into their applications.
 *
 * @section usage Usage
 * To measure the accuracy and speed of this library, include 'measure.h' in your C source files. 
 * This header provides necessary functions and utilities for conducting performance tests and 
 * accuracy assessments. It allows you to benchmark the library functions and verify their 
 * precision under various conditions. While 'arithmetic.h' focuses on large integer operations, 
 * 'measure.h' is dedicated to evaluating these operations from a performance and accuracy standpoint.
 *
 * Note: In the current library version, direct modification of the source code in 'main.c' is 
 * required for testing purposes. This involves integrating the 'measure.h' functionalities 
 * into the 'main.c' file to enable comprehensive testing and assessment of the library's capabilities.
 * 
 * @section PerformanceTest Performance Testing
 * This section details how to perform performance and accuracy tests for the library.
 * 
 * The Makefile provided with the library includes several targets to facilitate these tests, 
 * compiling the necessary source files and executing tests to generate visualizations of the results.
 * 
 * Before performing any tests, it is recommended to run `make rebuild`. 
 * This ensures that all components of the library are freshly compiled with the latest changes, 
 * providing a clean state for accurate performance and accuracy measurement.
 * 
 * Accuracy Testing:
 * 1. `make rebuild` - This will clean any previous builds and recompile the library.
 * 2. `make success` - This target runs the application, captures its output for accuracy assessment, 
 *    and then visualizes these results using a Python script. The script generates a chart that 
 *    illustrates the accuracy of the library functions. The output and visualization are moved to 
 *    the 'Views' directory for easy access and review.
 *
 * Performance Testing:
 * 1. Run `make rebuild` - This will clean any previous builds and recompile the library.
 * 2. Choose and run a performance test target:
 * //    - `make speed` - Runs the application to gather speed data and visualizes it using a Python script.
 *    - `make speed-mul` - Tests the performance of multiplication operations.
 *    - `make speed-squ` - Tests the performance of squaring operations.
 * //    - `make speed-div` - Tests the performance of division operations.
 * //      - `make speed-red` - Tests the performance of reduction operations.
 *    All results are stored and visualized in the 'Views' directory.
 *
 * Note: Ensure Python3 is installed and accessible in your environment to run the scripts and visualize the results properly.
 * 
 * @section notes Implementation Notes
 * - The library uses dynamic memory allocation for managing the array of words in `BINT`.
 * - Care should be taken to free the allocated memory using the appropriate function 
 *   provided by the library.
 * - The library handles both positive and negative numbers, represented by the `sign` 
 *   field in the `BINT` struct.
 * - Error handling is implemented using specific return codes from functions.
 *
 * @section development Development
 * The source code and development resources for the Big Integer Library can be found on GitHub:
 * [Big Integer Library on GitHub](https://github.com/Hacker-Code-J/DLP_calculator)
 *
 * @author Ji Yong-Hyeon, Moon Ye-chan, Kim Ye-chan, Yoo Geun-oh
 * @date 2023-12-08
 */

/**
 * @file main.c
 * @brief Main entry point for testing arithmetic operations.
 *
 * This file contains the main function which is used to test the accuracy and performance
 * of the large integer arithmetic operations. It leverages the functionality provided by
 * measure.h to conduct these tests.
 */

#include "measure.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    /*
    * It's crucial to uncomment and check your test functions one by one.
    * Enter as many integer values as you want.
    * TEST_ITERATIONS is an integer 10000
    */ 
    
    /*
    * ******************** Use 'make success' **********************
    */
    // correctTEST_ADD(TEST_ITERATIONS);
    // correctTEST_SUB(TEST_ITERATIONS);

    // correctTEST_TxtBk(TEST_ITERATIONS);
    // correctTEST_ImpTxtBk(TEST_ITERATIONS);
    correctTEST_Krtsb(TEST_ITERATIONS);
    
    // correctTEST_SQU_TxtBk(TEST_ITERATIONS);
    // correctTEST_SQU_Krtsb(TEST_ITERATIONS);

    // corretTEST_BinDIV(TEST_ITERATIONS);
    // corretTEST_GenDIV(TEST_ITERATIONS);

    // corretTEST_EXP_MOD_L2R(TEST_ITERATIONS);
    // corretTEST_EXP_MOD_R2L(TEST_ITERATIONS);
    // corretTEST_EXP_MOD_Montgomery(TEST_ITERATIONS);

    // corretTEST_BarrettRed(TEST_ITERATIONS);
    // corretTEST_EEA(TEST_ITERATIONS);

    /*
    * ********************** Use 'make speed-mul' **********************
    */
    // performTEST_MUL(); // TextBook vs Improved TextBook

    /*
    * ********************** Use 'make speed-squ' **********************
    */
    // performTEST_SQU(); // TextBook vs Karatsuba

    return 0;
}