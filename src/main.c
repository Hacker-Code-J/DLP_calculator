#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "arithmetic.h"

char getControlInput() {
    char ch;
    int result;
    printf("\t|                                               |\n");
    printf("\t|\tEnter R to restart or Q to quit: ");
    do {
        result = scanf(" %c", &ch);
        while(getchar() != '\n'); // Clear the input buffer
    } while (result != 1); // Repeat until a valid character is read
    return ch;
}

int isValidNumberForBase(const char* number, int base) {
    int i = 0, length = strlen(number);
    while (i < length) {
        if (base == 2 && (number[i] < '0' || number[i] > '1')) {
            return 0; // 2진법이 아닌 문자 발견
        }
        if (base == 10 && (number[i] < '0' || number[i] > '9')) {
            return 0; // 10진법이 아닌 문자 발견
        }
        if (base == 16 && !((number[i] >= '0' && number[i] <= '9') || (number[i] >= 'A' && number[i] <= 'F') || (number[i] >= 'a' && number[i] <= 'f'))) {
            return 0; // 16진법이 아닌 문자 발견
        }
        i++;
    }
    return 1; // 올바른 형식의 숫자
}

int convertToDecimal(char* number, int base) {
    int start = 0; // 시작 인덱스
    int length = strlen(number);

    // 16진수일 때 "0x" 또는 "0X" 접두사를 확인
    if (base == 16 && length >= 2 && (number[0] == '0' && (number[1] == 'x' || number[1] == 'X'))) {
        start = 2; // "0x" 또는 "0X"를 무시
    }

    // 2진수일 때 "0b" 또는 "0B" 접두사를 확인
    if (base == 2 && length >= 2 && (number[0] == '0' && (number[1] == 'b' || number[1] == 'B'))) {
        start = 2; // "0b" 또는 "0B"를 무시
    }

    if (!isValidNumberForBase(number + start, base)) {
        return -1; // 잘못된 형식의 숫자 입력
    }

    int decimalNumber = 0;
    for (int i = start; i < length; i++) {
        if (number[i] >= '0' && number[i] <= '9') {
            decimalNumber += (number[i] - '0') * pow(base, length - i - 1);
        }
        else if (base == 16 && (number[i] >= 'A' && number[i] <= 'F')) {
            decimalNumber += (number[i] - 'A' + 10) * pow(base, length - i - 1);
        }
        else if (base == 16 && (number[i] >= 'a' && number[i] <= 'f')) {
            decimalNumber += (number[i] - 'a' + 10) * pow(base, length - i - 1);
        }
    }
    return decimalNumber;
}

int add_func(int num1, int num2) {
    int add_res = 0;
    add_res = num1 + num2;
    return add_res;
}

int minus_func(int num1, int num2) {
    int min_res = 0;
    min_res = num1 - num2;
    return min_res;
}

int multi_func(int num1, int num2) {
    int mul_res = 0;
    mul_res = num1 * num2;
    return mul_res;
}

int share_func(int num1, int num2) {
    int sha_res = 0;
    sha_res = num1 / num2;
    return sha_res;
}

int rest_func(int num1, int num2) {
    int res_res = 0;
    res_res = num1 % num2;
    return res_res;
}

int exponent_win_func(int base, int exponent) {
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}


int main() {
    int calculatorType;
    char num1_str[32], num2_str[32];    // 전달해 줄 인자 저장 배열
    int num1, num2, result, base;
    char base_str[10];
    char operator;
    int validInput = 0;

    printf("\t=================================================\n");
    printf("\t|           P.A.N.D.A - PUBAO                   |\n");
    printf("\t=================================================\n");

    while (1) {
        printf("\t|\t   1. DLP DECRYPTOR                     |\n");
        printf("\t|\t   2. BIG Integer Calculator            |\n");
        printf("\t|\t   Choose calculator type: ");
        if (scanf("%d", &calculatorType) != 1) {
            printf("\t=================================================\n");
            printf("\t|\tInvalid input. Please enter 1 or 2.     |\n");
            printf("\t=================================================\n");
            while (getchar() != '\n');
            continue;
        }

        if (calculatorType == 2) {
            printf("\t=================================================\n");
            // while (!validInput) {
            //     int result;
            //     printf("\t|\t   Choose number base                   |\n\t|\t   (2 - Bin, 10 - Dec, 16 - Hex): ");
            //     result = scanf("%9s", base_str);
            //     while(getchar() != '\n'); // Clear the input buffer
            //     if (result != 1) {
            //         printf("\t|          Error: Invalid input                 |\n");
            //         continue;
            //     }

            //     if (strcmp(base_str, "2") == 0 || strcmp(base_str, "10") == 0 || strcmp(base_str, "16") == 0) {
            //         validInput = 1;      
            //         base = atoi(base_str);
            //         break;
            //     }
            //     else {
                   
            //         printf("\t=================================================\n");
            //         printf("\t|                                               |\n");
            //         printf("\t|          Error: Invalid input                 |\n");
            //         printf("\t|                                               |\n");
            //         printf("\t=================================================\n");
            //         continue;
            //     }
            // }

           
            printf("\t=================================================\n");

            while (1) {
                int result;
                printf("\t|                                               |\n");
                printf("\t\t   First number1 (HEX) : 0x");
                do {
                    result = scanf("%s", num1_str);
                    while(getchar() != '\n'); // Clear the input buffer
                } while (result != 1); // Repeat until a valid input is read

                num1 = convertToDecimal(num1_str, 16);
                if (num1 == -1) {
                    printf("\t|                                               |\n");
                    printf("\t=================================================\n");
                    printf("\t|                                               |\n");
                    printf("\t|          Error: Invalid number format         |\n");
                    printf("\t|                                               |\n");
                    printf("\t=================================================\n");
                    continue;
                }
                break;
            }

            while (1) {
                int result;
                printf("\t|\t   Enter operator (+, -, *, /, %%, ^) : ");
                do {
                    result = scanf(" %c", &operator);
                    while(getchar() != '\n'); // Clear the input buffer
                } while (result != 1); // Repeat until a valid input is read
 
                if (operator == '+' || operator == '-' || operator == '*' || operator == '/' || operator == '%' || operator == '^') {
                    break;
                }
                else {
                    printf("\t|                                               |\n");
                    printf("\t=================================================\n");
                    printf("\t|                                               |\n");
                    printf("\t|         Error: Invalid operator               |\n");
                    printf("\t|                                               |\n");
                    printf("\t=================================================\n");
                    printf("\t|                                               |\n");
                }
            }

            while (1) {
                int result;
                printf("\t|\t   Second number2 : ");
                do {
                    result = scanf("%31s", num2_str);
                    while(getchar() != '\n'); // Clear the input buffer
                } while (result != 1); // Repeat until a valid input is read
 
                num2 = convertToDecimal(num2_str, base);
                if (num2 == -1) {
                    printf("\t|                                               |\n");
                    printf("\t=================================================\n");
                    printf("\t|                                               |\n");
                    printf("\t|          Error: Invalid number format         |\n");
                    printf("\t|                                               |\n");
                    printf("\t=================================================\n");
                    printf("\t|                                               |\n");
                    continue;
                }
                break;
            }


            printf("\t|                                               |\n");
            printf("\t=================================================\n");

            switch (operator) {
            case '+':
                printf("\t|                                               |\n");
                result = add_func(num1, num2);
                break;
            case '-':
                result = minus_func(num1, num2);
                printf("\t|                                               |\n");
                break;
            case '*':
                result = multi_func(num1, num2);
                printf("\t|                                               |\n");
                break;
            case '/':
                if (num2 == 0) {
                    printf("\t|         Error: Division by zero               |\n");
                    printf("\t=================================================\n");
                    char controlChar = getControlInput();
                    if (controlChar == 'Q' || controlChar == 'q') {
                        printf("\t|\tProgram terminated.                     |\n");
                        printf("\t|                                               |\n");
                        printf("\t=================================================\n");
                        exit(1);
                    }
                    else if (controlChar == 'R' || controlChar == 'r') {
                        printf("\t|                                               |\n");
                        printf("\t=================================================\n");
                        continue;
                    }
                }
                printf("\t|                                               |\n");
                result = share_func(num1, num2);
                break;
            case '%':
                if (num2 == 0) {
                    printf("\t|         Error: Division by zero               |\n");
                    printf("\t=================================================\n");
                    char controlChar = getControlInput();
                    if (controlChar == 'Q' || controlChar == 'q') {
                        printf("\t|\tProgram terminated.                     |\n");
                        printf("\t|                                               |\n");
                        printf("\t=================================================\n");
                        exit(1);
                    }
                    else if (controlChar == 'R' || controlChar == 'r') {
                        printf("\t|                                               |\n");
                        printf("\t=================================================\n");
                        continue;
                    }
                }
                printf("\t|                                               |\n");
                result = rest_func(num1, num2);
                break;
            case '^':
                printf("\t|                                               |\n");
                result = exponent_win_func(num1, num2);
                break;
            default:
                printf("\t|         Error: Invalid operator               |\n");
                printf("\t=================================================\n");
                return 1;
                break;
            }


            printf("\t|                   %5d                       |\n", result);
            printf("\t|                                               |\n");
            printf("\t=================================================\n");

            char controlChar = getControlInput();
            if (controlChar == 'Q' || controlChar == 'q') {
                printf("\t|\tProgram terminated.                     |\n");
                printf("\t|                                               |\n");
                printf("\t=================================================\n");
                break;
            }
            else if (controlChar == 'R' || controlChar == 'r') {
                printf("\t|                                               |\n");
                printf("\t=================================================\n");
                continue;
            }
        }
        else if (calculatorType == 1) {
            // DLP 계산기 관련 코드
            printf("\t=================================================\n");
            printf("\t|      DLP Calculator is not implemented yet    |\n");
            printf("\t=================================================\n");
            char controlChar = getControlInput();
            if (controlChar == 'Q' || controlChar == 'q') {
                printf("\t|\tProgram terminated.                     |\n");
                printf("\t|                                               |\n");
                printf("\t=================================================\n");
                break;
            }
            else if (controlChar == 'R' || controlChar == 'r') {
                printf("\t|                                               |\n");
                printf("\t=================================================\n");
                continue;
            }
        }
        else {
            printf("\t=================================================\n");
            printf("\t|               Invalid choice                  |\n");
            printf("\t=================================================\n");
            char controlChar = getControlInput();
            if (controlChar == 'Q' || controlChar == 'q') {
                printf("\t|\tProgram terminated.                     |\n");
                printf("\t|                                               |\n");
                printf("\t=================================================\n");
                break;
            }
            else if (controlChar == 'R' || controlChar == 'r') {
                printf("\t|                                               |\n");
                printf("\t=================================================\n");
                continue;
            }
        }
    }

    return 0;
}