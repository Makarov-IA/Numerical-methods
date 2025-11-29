#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <assert.h>
#include "utils_matrix.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

double * multiply_matrix_and_vector(double* matrix, int n, int m, double* vector, int k) {
    //n - число строк матрицы 
    //m - число столбцов матрицы
    //k - рамзерность вектора
    double row_by_column_result = 0;
    double * result = (double*)malloc(k * sizeof(double));
    
    assert(m == k);

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

int progonka(double* a, double* b, double* c, int n, double* d, double* x) {
    
    double *al = (double*)malloc(sizeof(double)*n);
    double *be = (double*)malloc(sizeof(double)*n);

    if (fabs(b[0]) <= 1e-8) {
        fprintf(stderr, "Error: division by 0, can't find only one solution\n");
        exit(1);
    }
    if (n == 1) {
        x[0] = d[0] / b[0];
        free(al); free(be);
        return 0;
    }

    al[0] = -c[0] / b[0];
    be[0] =  d[0] / b[0];

    for (int i = 1; i <= n-2; ++i) {
        double g = b[i] + a[i-1]*al[i-1];
        if (fabs(g) <= 1e-8) {
            fprintf(stderr, "Error: division by 0, can't find only one solution\n");
            return 1;
        }
        al[i] = -c[i] / g;
        be[i] = (d[i] - a[i-1]*be[i-1]) / g;
    }

    {
        int i = n-1;
        double g = b[i] + a[i-1]*al[i-1];
        if (fabs(g) <= 1e-8) {
            fprintf(stderr, "Error: division by 0, can't find only one solution\n");
            return 1;
        }
        be[i] = (d[i] - a[i-1]*be[i-1]) / g;
    }

    x[n-1] = be[n-1];
    for (int i = n-2; i >= 0; --i)
        x[i] = al[i]*x[i+1] + be[i];

    free(al); free(be);
    return 0;
}
