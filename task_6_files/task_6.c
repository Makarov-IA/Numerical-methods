#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include "../utils/utils_matrix.h" 
#include "../utils/second_order_ode.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

int main(int argc, char *argv[]) {
    
    int N;
    double h, lambda, norm_of_vector, result_of_mult, P; \
    int method, right_part_number;
    char path[256];
    FILE* file;

    assert(argc >= 5);
    assert(sscanf(argv[1], "%d", &N)==1);
    assert(sscanf(argv[2], "%d", &method)==1);
    assert(sscanf(argv[3], "%d", &right_part_number));
    assert(sscanf(argv[4], "%lf", &P));
    
    double (*right_part)(int);
    double * right_part_vector = (double*)malloc((N-1)*sizeof(double));
    double * a = (double*)malloc((N-1)*sizeof(double));
    double * b = (double*)malloc((N-2)*sizeof(double));
    double * c = (double*)malloc((N-2)*sizeof(double));
    double * vector = (double*)malloc((N-1)*sizeof(double));
    double * solution = (double*)malloc((N-1)*sizeof(double));
    snprintf(path, sizeof path, "data_graph/%d_%lf.txt", right_part_number, P);
    file = fopen(path, "w");
    //Создание матрицы
    //В 2.91 y1=y0, yN-1=yN => y'0 = 0, y'N-1
    h = 1.0/(N-2); // Размер шага = длина интервала/количество точке разбиения 
    for (int i=0; i<N-1; i++) {
        if (i!=0 && i!=N-2) 
            b[i] = 2/pow(h, 2);
        else if (i==0 || i==N-2) {
            b[i] = 1/pow(h, 2);
            }
        if (i < N-2) {
            a[i] = -1/pow(h,2);
            c[i] = -1/pow(h,2);
        }
    }
    //Инициализируем нулями
    for (int i=0; i <N-1; i++) 
        solution[i] = 0;

    //Создание правой части
    switch (right_part_number) {
        case 1:
            right_part = right_part_1;
            break;
    }
    for (int i=0; i < N-1; i++) {
        right_part_vector[i] = right_part(i);
    }

    //Метод Фурье
    //Нахождение собственных векторов
    if (method == 1) {
        for (int k=1; k<N; k++) {
            double * vector = (double*)malloc((N-1)*sizeof(double));
            lambda = 4*pow(sin(M_PI*(k-1)/(2*N-2)),2)/pow(h,2) + P;
            for (int i=0; i<N-1; i++) {
                vector[i] = cos(M_PI*(k-1)*(2*i+1)/(2*N-2));
            }
            result_of_mult = multiply_vector_and_vector(vector, right_part_vector, N-1);
            norm_of_vector = multiply_vector_and_vector(vector, vector, N-1);
            for (int i=0; i<N-1; i++) 
                solution[i] += cos(M_PI*(k-1)*(2*i+1)/(2*N-2))*result_of_mult/norm_of_vector/lambda;
        }
        for (int k=1; k<N-1; k++)
            fprintf(file, "%lf %lf \n", h*k, solution[k]);
    }

    free(solution);
    free(vector);
    free(right_part_vector);

    return 0;
}
