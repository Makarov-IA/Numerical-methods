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
    double h, lambda, norm_of_vector, result_of_mult;
    int method, set_number;
    char path[256];
    FILE* file;

    assert(argc >= 4);
    assert(sscanf(argv[1], "%d", &N)==1);
    assert(sscanf(argv[2], "%d", &method)==1);
    assert(sscanf(argv[3], "%d", &set_number));
    
    double (*right_part)(double);
    double (*p_part)(double);
    double (*theoretical_solution)(double);
    double * right_part_vector = (double*)malloc((N-1)*sizeof(double));
    double * vector = (double*)malloc((N-1)*sizeof(double));
    double * solution = (double*)malloc((N-1)*sizeof(double));
    double *a, *b, *c;
    //Парсим путь до файла, куда будем кидать точки
    if (method == 1) {
        snprintf(path, sizeof path, "data_graph/furier/%d.txt", set_number);    
        if (set_number >= 3) {
            fprintf(stderr, "Error: set >= 3 if for progonka\n");
            free(solution);
            free(vector);
            free(right_part_vector);
            exit(1);
        }
    }
    else if (method == 2) {
        snprintf(path, sizeof path, "data_graph/progonka/%d.txt", set_number);
    }
    else {
        fprintf(stderr, "Error: incorrect method\n");
        free(solution);
        free(vector);
        free(right_part_vector);
        exit(1);
    }
    file = fopen(path, "w");

    //В 2.91 y1=y0, yN-1=yN => y'0 = 0, y'N-1
    h = 1.0/(N-2); // Размер шага = длина интервала/количество точке разбиения 

    //Создание сета решения
    switch (set_number) {
        case 1:
            right_part = right_part_1;
            theoretical_solution = theoretical_solution_1;
            p_part = p_part_1;
            break;
        case 2:
            right_part = right_part_2;
            theoretical_solution = theoretical_solution_2;
            p_part = p_part_2;
            break;
        case 3:
            right_part = right_part_3;
            theoretical_solution = theoretical_solution_3;
            p_part = p_part_3;
            break;
    }
    for (int i=0; i < N-1; i++) {
        right_part_vector[i] = right_part(i*h);
    }

    //Создание матрицы
    if (method==2) {
        a = (double*)malloc((N-2)*sizeof(double));
        b = (double*)malloc((N-1)*sizeof(double));
        c = (double*)malloc((N-2)*sizeof(double));
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
            b[i] += p_part(i*h);
        }

    }

    //Инициализируем нулями
    for (int i=0; i <N-1; i++) 
        solution[i] = 0;

    //Метод Фурье
    //Нахождение собственных векторов
    if (method == 1) {
        for (int k=1; k<N; k++) {
            double * vector = (double*)malloc((N-1)*sizeof(double));
            lambda = 4*pow(sin(M_PI*(k-1)/(2*N-2)),2)/pow(h,2) + p_part(0.0);
            for (int i=0; i<N-1; i++) {
                vector[i] = cos(M_PI*(k-1)*(2*i+1)/(2*N-2));
            }
            result_of_mult = multiply_vector_and_vector(vector, right_part_vector, N-1);
            norm_of_vector = multiply_vector_and_vector(vector, vector, N-1);
            for (int i=0; i<N-1; i++) 
                solution[i] += cos(M_PI*(k-1)*(2*i+1)/(2*N-2))*result_of_mult/norm_of_vector/lambda;
        }
        for (int k=0; k<N-1; k++)
            fprintf(file, "%lf %lf %lf\n", h*k, solution[k], theoretical_solution(k*h));
    }
    if (method==2) {
        solution = progonka(a, b, c, N-1, right_part_vector);
        for (int k=0; k<N-1; k++)
            fprintf(file, "%lf %lf %lf\n", h*k, solution[k], theoretical_solution(k*h));
        free(a); free(b); free(c);
    }

    free(solution); free(vector); free(right_part_vector);

    return 0;
}
