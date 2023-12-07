/**
 * @file measure.h
 * @brief Header file for measuring accuracy and performance of arithmetic operations.
 *
 * This file declares functions for measuring the accuracy and performance (speed)
 * of the large integer arithmetic operations implemented in arithmetic.h and arithmetic.c.
 * It includes functions for benchmarking and validation of results.
 */

#ifndef _MEASURE_H
#define _MEASURE_H

#include "../arithmetic.h"

// Define Macros for Bit Lengths based on 8-bit word units
#define u8_BIT_1024 0x080  // 128 * 8 = 1024 bits
#define u8_BIT_2048 0x100  // 256 * 8 = 2048 bits
#define u8_BIT_3072 0x180  // 384 * 8 = 3072 bits
#define u8_BIT_7680 0x3C0  // 960 * 8 = 7680 bits

// Define Macros for Bit Lengths based on 32-bit word units
#define u32_BIT_1024 0x20  // 032 * 32 = 1024 bits
#define u32_BIT_2048 0x40  // 064 * 32 = 2048 bits
#define u32_BIT_3072 0x60  // 096 * 32 = 3072 bits
#define u32_BIT_7680 0xF0  // 240 * 32 = 7680 bits

// Define Macros for Bit Lengths based on 64-bit word units
#define u64_BIT_1024 0x10  // 016 * 64 = 1024 bits
#define u64_BIT_2048 0x20  // 032 * 64 = 2048 bits
#define u64_BIT_3072 0x30  // 048 * 64 = 3072 bits
#define u64_BIT_7680 0x78  // 120 * 64 = 7680 bits

// Configuration Macros
#define TEST_ITERATIONS 10000
#define MAX_BIT_LENGTH u32_BIT_3072
#define MIN_BIT_LENGTH u32_BIT_2048

void performBINT_2ArgFn(void (*testFunc)(BINT**, BINT**), BINT** pptrX, BINT** pptrZ);
void performTEST_2ArgFn(void (*testFunc1)(BINT**, BINT**), void (*testFunc2)(BINT**, BINT**));
void performBINT_3ArgFn(void (*testFunc)(BINT**, BINT**, BINT**), BINT** pptrX, BINT** pptrY, BINT** pptrZ);
void performTEST_3ArgFn(void (*testFunc1)(BINT**, BINT**, BINT**), void (*testFunc2)(BINT**, BINT**, BINT**));
void performBINT_4ArgFn(void (*testFunc)(BINT**, BINT**, BINT**, BINT**), BINT** pptrX, BINT** pptrY, BINT** pptrM, BINT** pptrN);
void performTEST_4ArgFn(void (*testFunc1)(BINT**, BINT**, BINT**, BINT**), void (*testFunc2)(BINT**, BINT**, BINT**, BINT**), int test_cnt);

/**
 * @brief Correctness Test for Addition
 * @details Performs a series of automated tests to verify the correctness of the addition operation in the system.
 *          The function iteratively tests addition operations and checks for accuracy against expected results.
 * @param test_cnt The number of test cases to be executed.
 * @pre Assumes an existing implementation of the addition operation that needs to be tested.
 * @post Provides a summary of the test results, indicating success or failure of each test case.
 * @note Used primarily in a development or testing environment to ensure the reliability of the addition operation.
 * @warning Ensure that the testing framework and addition operation are properly implemented to avoid false positives or negatives in test results.
 */
void correctTEST_ADD(int test_cnt);

// /**
//  * @brief Performance Test for Addition
//  * @details Evaluates the performance of the addition operation under various conditions and loads, using the specified number of test cases.
//  *          This function measures execution time, resource usage, and scalability of the addition operation.
//  * @param test_cnt The number of test cases to be executed for performance evaluation.
//  * @pre Requires an operational and correctly functioning addition module to test.
//  * @post Outputs performance metrics such as execution time, memory usage, and throughput.
//  * @note Aimed at identifying performance bottlenecks and optimizing the efficiency of the addition operation.
//  * @warning Performance results may vary based on the system's hardware and current load; ensure a controlled test environment for accurate measurements.
//  */
// void performTEST_ADD(int test_cnt);

/**
 * @brief Correctness Test for Subtraction
 * @details Conducts a series of tests to check the accuracy and correctness of the subtraction operation. 
 *          Each test case compares the result of the subtraction operation with expected values.
 * @param test_cnt The number of test cases to run.
 * @pre Assumes the presence of a subtraction operation in the system to be tested.
 * @post Summarizes the outcomes of all test cases, highlighting any discrepancies or failures.
 * @note Essential for validating the correctness of the subtraction operation, especially after modifications or updates.
 * @warning Inaccurate test cases or expected results can lead to misleading conclusions about the operation's correctness.
 */
void correctTEST_SUB(int test_cnt);

// /**
//  * @brief Performance Test for Subtraction
//  * @details Assesses the performance characteristics of the subtraction operation, such as speed and resource efficiency, across a range of test cases.
//  *          Focuses on measuring aspects like response time and computational load.
//  * @param test_cnt Number of test cases to execute for assessing performance.
//  * @pre A functional subtraction operation is required for effective performance evaluation.
//  * @post Outputs detailed performance data, offering insights into the efficiency of the subtraction process.
//  * @note Useful for identifying potential improvements in the subtraction operation's implementation.
//  * @warning Performance results are influenced by the system configuration and workload; ensure consistent test conditions for reliable data.
//  */
// void performTEST_SUB(int test_cnt);

/**
 * @brief Correctness Test for Textbook Algorithm
 * @details Executes a series of tests to verify the correctness of the textbook algorithm implemented in the system. 
 *          It runs a predefined number of test cases, comparing the algorithm's output with expected results to ensure accuracy.
 * @param test_cnt The number of test cases to be conducted.
 * @pre The textbook algorithm must be implemented and operational before conducting these tests.
 * @post The function will provide a report on the number of passed and failed test cases, aiding in identifying any issues in the algorithm.
 * @note This function is crucial for initial validation of the textbook algorithm, ensuring it performs as expected under various scenarios.
 * @warning Misconfigured or erroneous test cases can lead to false positives or negatives, affecting the reliability of the test outcomes.
 */
void correctTEST_TxtBk(int test_cnt);

/**
 * @brief Correctness Test for Improved Textbook Algorithm
 * @details Conducts correctness testing for an enhanced version of the textbook algorithm. 
 *          The function assesses the accuracy of the improved algorithm by running a specified number of test cases.
 * @param test_cnt The total number of tests to execute for assessing the algorithm.
 * @pre Requires the improved textbook algorithm to be in place and functioning.
 * @post Generates a summary of test results, highlighting the performance and reliability of the improved algorithm.
 * @note Particularly valuable after modifications or optimizations have been made to the standard textbook algorithm.
 * @warning Ensure that the improved algorithm and test cases are accurately defined to prevent erroneous assessments.
 */
void correctTEST_ImpTxtBk(int test_cnt);

/**
 * @brief Correctness Test for Karatsuba Algorithm
 * @details Implements test cases to validate the correctness of the Karatsuba multiplication algorithm. 
 *          This function tests the algorithm against a series of scenarios to ensure its accurate operation.
 * @param test_cnt The number of test cases to be run for validation.
 * @pre The Karatsuba multiplication algorithm must be implemented and ready for testing.
 * @post Outputs detailed results of the testing process, indicating the algorithm's correctness and potential areas of concern.
 * @note An essential step for verifying the Karatsuba algorithm's correctness, especially useful in complex numerical computations.
 * @warning Inaccurate test data or implementation flaws in the algorithm could lead to incorrect validation results.
 */
void correctTEST_Krtsb(int test_cnt);

/**
 * @brief Correctness Test for Binary Division
 * @details This function conducts a series of tests to verify the correctness of the binary division algorithm. 
 *          It executes a specified number of test cases, comparing the output of the binary division operation 
 *          with expected results to ensure accuracy and reliability.
 * @param test_cnt The number of test cases to be executed for the binary division correctness test.
 * @pre Binary division algorithm must be implemented and operational before these tests can be conducted.
 * @post The function outputs a detailed report on the test outcomes, including the number of successful and failed tests, 
 *       providing insights into the reliability and accuracy of the binary division algorithm.
 * @note This function is crucial for validating the binary division implementation, especially after modifications or optimizations.
 * @warning Accurate and diverse test cases are essential for a comprehensive assessment. Inaccurate or insufficient test data can lead to misleading conclusions.
 */
void corretTEST_BinDIV(int test_cnt);

/**
 * @brief Correctness Test for General Division
 * @details Executes correctness testing for a general division algorithm, running a defined number of tests to validate its accuracy.
 *          This function assesses the algorithm's performance by comparing its results against known correct outcomes.
 * @param test_cnt The number of test cases to run for the general division correctness test.
 * @pre A general division algorithm must be in place and functional for the test to be meaningful.
 * @post Produces a summary of the test results, indicating the overall accuracy and reliability of the general division algorithm.
 * @note Essential for ensuring the correctness of the general division algorithm, particularly in systems where division operations are critical.
 * @warning The quality and variety of test cases significantly impact the validity of the test results. Ensure comprehensive and accurate test scenarios.
 */
void corretTEST_GenDIV(int test_cnt);

/**
 * @brief Correctness Test for Modular Exponentiation (Left-to-Right)
 * @details This function tests the accuracy of the left-to-right modular exponentiation algorithm. 
 *          It runs through a specified number of test cases, each designed to validate the algorithm's correctness in computing \( a^b \mod n \) using a left-to-right approach.
 * @param test_cnt The number of test cases to be executed.
 * @pre The left-to-right modular exponentiation algorithm must be implemented and functional prior to running these tests.
 * @post Upon completion, the function will provide a report detailing the number of passed and failed test cases, 
 *       thereby assessing the reliability and accuracy of the algorithm.
 * @note This testing is crucial for applications requiring robust modular exponentiation, such as cryptographic systems.
 * @warning The integrity of the test results depends heavily on the accuracy of the test cases and the proper implementation of the algorithm.
 */
void corretTEST_EXP_MOD_L2R(int test_cnt);

/**
 * @brief Correctness Test for Modular Exponentiation (Right-to-Left)
 * @details Executes a series of tests to verify the accuracy of the right-to-left modular exponentiation algorithm. 
 *          It methodically assesses the algorithm's capability to accurately perform \( a^b \mod n \) computations in a right-to-left manner.
 * @param test_cnt The total number of test cases to be performed.
 * @pre A right-to-left modular exponentiation algorithm should be in place and operational.
 * @post Produces a comprehensive summary of the test outcomes, offering insights into the precision and reliability of the algorithm.
 * @note Vital for ensuring the correctness of right-to-left modular exponentiation, especially in high-stakes computational environments.
 * @warning Accurate test scenarios are essential for a meaningful validation of the algorithm's correctness.
 */
void corretTEST_EXP_MOD_R2L(int test_cnt);

/**
 * @brief Correctness Test for Montgomery Modular Exponentiation
 * @details This function is designed to test the correctness of the Montgomery modular exponentiation algorithm. 
 *          It conducts a series of test cases to confirm the algorithm's effectiveness in performing efficient and accurate modular exponentiation, 
 *          employing the Montgomery method.
 * @param test_cnt Number of test cases intended for execution.
 * @pre The Montgomery modular exponentiation algorithm must be correctly implemented and operational.
 * @post Generates a detailed report on the test results, highlighting the accuracy and efficiency of the Montgomery modular exponentiation method.
 * @note An essential function for systems that rely on fast and accurate modular exponentiation, such as in cryptographic algorithms.
 * @warning The reliability of the test outcomes is highly dependent on the precision of the test cases and the correct functioning of the Montgomery method.
 */
void corretTEST_EXP_MOD_Montgomery(int test_cnt);

/**
 * @brief Correctness Test for Barrett Reduction
 * @details This function performs a series of tests to assess the correctness of the Barrett Reduction algorithm. 
 *          It runs through a specified number of test cases to ensure the algorithm accurately performs modular reduction 
 *          using the Barrett method, which is an efficient way to compute modular reductions for large numbers.
 * @param test_cnt The number of test cases to be executed for the Barrett Reduction correctness test.
 * @pre The Barrett Reduction algorithm must be implemented and functional before conducting these tests.
 * @post The function provides a report detailing the outcomes of the test cases, indicating the accuracy and reliability of the algorithm.
 * @note This testing is crucial for applications that require efficient modular arithmetic, such as in cryptographic algorithms.
 * @warning Accurate and comprehensive test cases are vital for a meaningful validation of the algorithm's correctness.
 */
void corretTEST_BarrettRed(int test_cnt);

/**
 * @brief Correctness Test for Extended Euclidean Algorithm
 * @details Conducts a set of tests to verify the accuracy and reliability of the Extended Euclidean Algorithm (EEA). 
 *          The function executes a predetermined number of test cases, each aimed at assessing the EEA's ability 
 *          to compute the greatest common divisor (GCD) and the coefficients of Bézout's identity.
 * @param test_cnt The total number of test cases to run for assessing the EEA.
 * @pre An implementation of the Extended Euclidean Algorithm must be in place and operational.
 * @post Outputs a summary of test results, highlighting the correctness and effectiveness of the EEA.
 * @note Essential for validating the EEA, particularly in systems where accurate GCD computation and Bézout coefficients are crucial.
 * @warning The integrity of the test outcomes is highly dependent on the quality of the test cases and the correctness of the EEA implementation.
 */
void corretTEST_EEA(int test_cnt);

void performTEST_MUL(int test_cnt);
// void performTEST_DIV(int test_cnt);
// void performFastRed(int test_cnt);

#endif // _MEASURE_H