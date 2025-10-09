#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include "../utils/utils_matrix.h" 

#define BG_YELLOW  "\033[43m"
#define BG_WHITE   "\033[47m"
#define BG_MAGENTA "\033[45m"
#define BLACK   "\033[30m"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define BRIGHT_BLUE    "\033[94m"
#define DEBUG_COLOR    "\033[1;35m"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

int main(int argc, char *argv[]) {
    
    int N;
    double h, lambda, diff_normed, max_normed_diff, last_diff_normed;
    int limit_n, limit_m, limit_k;

    assert(argc >= 3);
    assert(sscanf(argv[1], "%d", &N)==1);
    assert(sscanf(argv[2], "%lf", &h)==1);
    assert(sscanf(argv[3], "%d", &limit_n)==1);
    assert(sscanf(argv[4], "%d", &limit_m)==1);
    assert(sscanf(argv[5], "%d", &limit_k)==1);

    if (h == 0) {
        h = 1/(double)(N);
    }
    double * matrix = (double*)malloc(pow((N-1),2)*sizeof(double));
    double * result_of_mult;
    double ** all_vectors = (double**)malloc((N-1)*sizeof(double*));
    double * gramm_matrix = (double*)malloc(pow((N-1),2)*sizeof(double));

    //Создание матрицы
    for (int i=0; i<N-1; i++) {
        for (int j=0; j<N-1; j++) {
            if (i==j) {
                matrix[i*(N-1)+j] = 2/pow(h, 2);
            }
            else if (j==i+1 || j==i-1) {
                matrix[i*(N-1)+j] = -1/pow(h,2);
            }
            else {
                matrix[i*(N-1)+j] = 0;
            }
        }
    }

    //Вывод матрицы и имен столбцов
    printf("\n " BG_YELLOW BLACK "Matrix" RESET "\n" RESET);
    print_matrix(matrix, N-1, N-1, limit_n, limit_m);
    printf(RESET "\n "BG_YELLOW BLACK  "Lambda" RESET  "     " \
        BG_YELLOW BLACK "Difference_normed" RESET " " \
        BG_YELLOW BLACK "Vector" RESET "\n");
    
    last_diff_normed = DBL_MAX;
    max_normed_diff = DBL_MIN;
    //Нахождение собственных векторов
    for (int k=1; k<N; k++) {
        double * vector = (double*)malloc((N-1)*sizeof(double));
        lambda = 2*(1 - cos(M_PI*k/N))/pow(h,2);
        for (int i=0; i<N-1; i++) {
            vector[i] = sin(M_PI*(i+1)*k/N);
            }

        result_of_mult = multiply_matrix_and_vector(matrix, N-1, N-1, vector, N-1);
        all_vectors[k-1] = vector;

        for (int i=0; i<N-1; i++) {

            result_of_mult[i] -= vector[i]*lambda;
            }

        diff_normed = norm(result_of_mult, N-1)/lambda;
        max_normed_diff = max(diff_normed, max_normed_diff);

        if (k <= limit_k) {
            printf(" %.3e ", lambda);
            printf(" %.3e ", diff_normed);
            if (diff_normed > last_diff_normed) {
                printf(RED "   ↑   " RESET);
            }
            else if (diff_normed < last_diff_normed) {
                printf(GREEN "   ↓   " RESET);
            }
            else {
                printf(BLUE "   =   " RESET);
            }
            print_vector_horizontally(vector, N-1, limit_m);
        }
        last_diff_normed = diff_normed;
    }

    if (min(N-1, limit_k) != N-1)
        printf(" ... \n");
    printf("\n " BRIGHT_BLUE "Max_normed_diff" RESET " " BG_MAGENTA "%.8e" RESET "\n", max_normed_diff);

    gramm_matrix = make_gram_matrix(all_vectors, N-1, N-1);
    
    double max_scalar_prod = 0;
    for (int i=0; i < N-1; i++) {
        for (int j=0; j < N-1; j++) {
            if (i != j) {
                max_scalar_prod = max(fabs(gramm_matrix[i*(N-1)+j])/N, max_scalar_prod);
            }
        }
    }

    printf("\n " BG_YELLOW BLACK "Gramm_matrix" RESET "\n");
    print_matrix(gramm_matrix, N-1, N-1, limit_n, limit_m);
    printf(RESET "\n " BRIGHT_BLUE "Max_abs_scalar_prod_normed" RESET " " BG_MAGENTA "%.8e" RESET "\n\n", max_scalar_prod);

    free(all_vectors);
    free(result_of_mult);
    free(matrix);
    free(gramm_matrix);

    return 0;
}