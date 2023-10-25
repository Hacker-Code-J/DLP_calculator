#include "setup.h"
#include "BigInteger.h"

#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
    BINT* data;
    int rows;
    int cols;
} Matrix;

Matrix matrix_init(int rows, int cols);
void matrix_set(Matrix* mat, int row, int col, BINT value);
BINT matrix_get(const Matrix* mat, int row, int col);
void matrix_free(Matrix* mat);

#endif // MATRIX_H