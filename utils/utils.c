#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <assert.h>
#include "utils.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

double * multiply_matrix_and_vector(double* matrix, int n, int m, double* vector, int k) {
    //n - число строк матрицы 
    //m - число столбцов матрицы
    //k - рамзерность вектора
    assert(m == k);
    double row_by_column_result = 0;
    double * result = (double*)malloc(k * sizeof(double));

    for (int i=0; i<n; i++) {
        row_by_column_result = 0;
        for (int j=0; j<m; j++) {
            row_by_column_result += matrix[i*m+j]*vector[j];
        }
        result[i] = row_by_column_result;
    }

    return result;
}

void print_matrix(double* matrix, int n, int m, int limit_n, int limit_m) {
    //n - число строк
    //m - число столбцов
    //limit_n - сколько строк вывести
    //limit_m - сколько столбцов вывести
    for (int i=0; i<min(n,limit_n); i++) {
        for (int j=0; j<min(m,limit_m); j++) {
            printf(" %+.1e ", matrix[i*m+j]);
        }
        printf("\n");
    }
}

void print_vector_horizontally(double* vector, int n, int limit_n) {
    //n - размерность вектора
    //limit_n - сколлько компонент вывести
    for (int i=0; i<min(n,limit_n); i++) {
        printf(" %+.3e ", vector[i]);
    }
    if (min(n,limit_n) != n) {
        printf(" ... ");
    }
    printf("\n");

}

double norm(double * vector, int n) {
    //n - размерность вектора
    double norm_ = DBL_MIN;

    for (int i=0; i<n; i++) {
        norm_ = max(norm_, fabs(vector[i]));
    }
    return norm_;
}

double multiply_vector_and_vector(double* vector_1, double* vector_2, int n) {
    //n - размерность вектора
    double result = 0;

    for (int i=0; i<n; i++) {
        result += vector_1[i]*vector_2[i];
    }
    return result;
}

double * make_gram_matrix(double ** all_vectors, int N, int n) {
    //N - число векторов
    //n - размерность вектора
    double * result = (double*)malloc(N*N*sizeof(double));
    for (int i=0; i < N; i++) {
        for (int j=0; j < N; j++) {
            result[i*(N)+j] = multiply_vector_and_vector(all_vectors[i], all_vectors[j], n);
        }
    }
    return result;
}