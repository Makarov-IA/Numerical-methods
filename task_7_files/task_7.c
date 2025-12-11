#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include "../utils/utils_matrix.h"
#include "../utils/second_order_pd.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

int main(int argc, char *argv[]) {
    int schema_type, set_number;
    int number_of_points_x, number_of_points_t;
    double T;
    double h, tau, t, x;
    char path[256];
    FILE *file;
    double *initial;
    double *u;
    double (*u_theor)(double, double);
    double max_diff;
    double u_num, u_th, diff;

    assert(argc >= 6);
    assert(sscanf(argv[1], "%d", &schema_type) == 1);
    assert(sscanf(argv[2], "%d", &set_number) == 1);
    assert(sscanf(argv[3], "%lf", &T) == 1);
    assert(sscanf(argv[4], "%d", &number_of_points_x) == 1);
    assert(sscanf(argv[5], "%d", &number_of_points_t) == 1);

    // Выбор функций p(t,x), f(t,x) и начальных условий u(0,x)
    double (*p_func)(double, double);
    double (*f_func)(double, double);
    double (*u0)(double);

    switch (set_number) {
        case 1:
            p_func = p_pd_1;
            f_func = f_pd_1;
            u0 = u0_pd_1;
            u_theor = theoretical_pd_1;
            break;
        case 2:
            p_func = p_pd_2;
            f_func = f_pd_2;
            u0 = u0_pd_2;
            u_theor = theoretical_pd_2;
            break;
    }

    h = 1.0/(double)(number_of_points_x - 1);
    tau = T/(double)(number_of_points_t - 1);

    initial = (double*)malloc(number_of_points_x * sizeof(double));
    for (int i = 0; i < number_of_points_x; i++) {
        initial[i] = u0(h * i);
    }

    u = solve_explicitly(initial, number_of_points_x, number_of_points_t, T, f_func, p_func);
    snprintf(path, sizeof path, "data_graph/%d.txt", set_number);
    file = fopen(path, "w");

    max_diff = 0.0;
    for (int j = 0; j < number_of_points_t; j++) {
        t = tau * j;
        for (int i = 0; i < number_of_points_x; i++) {
            x = h * i;
            u_num = u[j * number_of_points_x + i];
            u_th = u_theor(t, x);
            diff = fabs(u_num - u_th);
            if (diff > max_diff) max_diff = diff;
            fprintf(file, "%lf %lf %lf %lf\n", t, x, u_num, u_th);
        }
    }

    printf("Max difference on set %d: %lf\n", set_number, max_diff);

    fclose(file);
    free(initial);
    free(u);

    return 0;
}
