#pragma once

#include "matrix.h"

Matrix *multiply(Matrix *m1, Matrix *m2);
Matrix *add(Matrix *m1, Matrix *m2);
Matrix *subtract(Matrix *m1, Matrix *m2);
Matrix *dot(Matrix *m1, Matrix *m2);
Matrix *apply(double (*func)(double), Matrix *m);
Matrix *scale(Matrix *m, double n);
Matrix *addScalar(Matrix *m, double n);
Matrix *transpose(Matrix *m);
