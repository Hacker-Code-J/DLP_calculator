#include "matrix.h"

#include <stdlib.h>

Matrix matrix_init(int rows, int cols) {
    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = (BINT*)malloc(rows * cols * sizeof(BINT));
    return mat;
}

void matrix_set(Matrix* mat, int row, int col, BINT value) {
    mat->data[row * mat->cols + col] = value;
}

BINT matrix_get(const Matrix* mat, int row, int col) {
    return mat->data[row * mat->cols + col];
}

void matrix_free(Matrix* mat) {
    free(mat->data);
    mat->data = NULL;
    mat->rows = 0;
    mat->cols = 0;
}