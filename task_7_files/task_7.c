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
    double r;
    double *a, *b, *c, *d;

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

    if (schema_type == 1) {
        u = solve_explicitly(initial, number_of_points_x, number_of_points_t, T, f_func, p_func);
    } 
    else {
        // Неявная схема: (U^{n+1} - U^n)/tau = U_xx^{n+1} + p^{n+1} U^{n+1} + f^{n+1}
        r = tau/(h*h);
        a = (double*)malloc((number_of_points_x-1) * sizeof(double));
        b = (double*)malloc(number_of_points_x * sizeof(double));
        c = (double*)malloc((number_of_points_x-1) * sizeof(double));
        d = (double*)malloc(number_of_points_x * sizeof(double));
        u = (double*)malloc(number_of_points_x * number_of_points_t * sizeof(double));
        for (int i = 0; i < number_of_points_x; i++) {
            u[i] = initial[i];
        }

        for (int j = 1; j < number_of_points_t; j++) {
            double t_next = j * tau;

            // Левая граница
            b[0] = 1.0 + 2.0 * r - tau * p_func(t_next, 0.0);
            c[0] = -2.0 * r;
            d[0] = u[(j-1)*number_of_points_x] + tau * f_func(t_next, 0.0);

            // Внутренние узлы
            for (int i = 1; i < number_of_points_x-1; i++) {
                a[i-1] = -r;
                b[i] = 1.0 + 2.0 * r - tau * p_func(t_next, i * h);
                c[i] = -r;
                d[i] = u[(j-1)*number_of_points_x + i] + tau * f_func(t_next, i * h);
            }

            // Правая граница
            a[number_of_points_x-2] = -2.0 * r;
            b[number_of_points_x-1] = 1.0 + 2.0 * r - tau * p_func(t_next, 1.0);
            d[number_of_points_x-1] = u[(j-1)*number_of_points_x + number_of_points_x-1] + tau * f_func(t_next, 1.0);

            // Решаем трёхдиагональную систему
            progonka(a, b, c, number_of_points_x, d, &u[j*number_of_points_x]);
        }

        free(a); free(b); free(c); free(d);
    }
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

    printf("Max difference on set %d schema_type %d: %lf\n", set_number, schema_type, max_diff);

    fclose(file);
    free(initial);
    free(u);

    return 0;
}
