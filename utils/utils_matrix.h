#ifndef UTILS_MATRIX_H
#define UTILS_MATRIX_H

double * multiply_matrix_and_vector(double* matrix, int n, int m, double* vector, int k);
double multiply_vector_and_vector(double* vector_1, double* vector_2, int n);
void print_matrix(double* matrix, int n, int m, int limit_n, int limit_m);
void print_vector_horizontally(double* vector, int n, int limit_n);
double * make_gram_matrix(double ** all_vectors, int N, int n);
double norm(double * vector, int n);
double * progonka(double* a, double* b, double* c, int n, double* d);

#endif
