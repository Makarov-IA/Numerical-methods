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
    int schema_type, set_number, N;
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
        case 3:
            p_func = p_pd_3;
            f_func = f_pd_3;
            u0 = u0_pd_3;
            u_theor = theoretical_pd_3;
            break;
        case 4:
            p_func = p_pd_4;
            f_func = f_pd_4;
            u0 = u0_pd_4;
            u_theor = theoretical_pd_4;
            break;
    }

    h = 1.0/(double)(number_of_points_x - 2);
    tau = T/(double)(number_of_points_t - 1);

    //Инициализируем
    initial = (double*)malloc(number_of_points_x * sizeof(double));
    for (int i = 1; i < number_of_points_x-1; i++) {
        initial[i] = u0(h * i-h/2);
    }
    initial[0] = initial[1];
    initial[number_of_points_x-1] = initial[number_of_points_x-2];

    if (schema_type == 1) {
        u = solve_explicitly(initial, number_of_points_x, number_of_points_t, T, f_func, p_func);
    } 
    else {
        N = number_of_points_x-2;
        r = tau/(h*h);
        a = (double*)malloc((N-1) * sizeof(double));
        b = (double*)malloc(N * sizeof(double));
        c = (double*)malloc((N-1) * sizeof(double));
        d = (double*)malloc(N * sizeof(double));
        u = (double*)malloc(number_of_points_x * number_of_points_t * sizeof(double));
        for (int i = 0; i < number_of_points_x; i++) {
            u[i] = initial[i];
        }

        for (int j = 1; j < number_of_points_t; j++) {
            double t_next = j * tau;

            // Левая граница (Нейман: u_x=0 -> ghost u_{-1}=u_{1})
            b[0] = 1.0 + r - tau * p_func(t_next, h/2);
            c[0] = -r;
            d[0] = u[(j-1)*number_of_points_x+1] + tau * f_func(t_next, h/2);

            // Внутренние узлы
            for (int i = 1; i < N-1; i++) {
                a[i-1] = -r;
                b[i] = 1.0 + 2.0 * r - tau * p_func(t_next, i * h+h/2);
                c[i] = -r;
                d[i] = u[(j-1)*number_of_points_x + 1 + i] + tau * f_func(t_next, i * h+h/2);
            }

            // Правая граница (Нейман: u_N = u_{N-2})
            a[N-2] = -r;
            b[N-1] = 1.0 + r - tau * p_func(t_next, 1.0-h/2);
            d[N-1] = u[(j-1)*number_of_points_x + N] + tau * f_func(t_next, 1.0 - h/2);



            // Решаем трёхдиагональную систему
            progonka(a, b, c, N, d, &u[j*number_of_points_x+1]);
            u[j*number_of_points_x] = u[j*number_of_points_x + 1];
            u[j*number_of_points_x + number_of_points_x - 1] = u[j*number_of_points_x + number_of_points_x - 2];

        }

        free(a); free(b); free(c); free(d);
    }
    snprintf(path, sizeof path, "data_graph/%d.txt", set_number);
    file = fopen(path, "w");

    max_diff = 0.0;
    for (int j = 0; j < number_of_points_t; j++) {
        t = tau * j;
        for (int i = 1; i < number_of_points_x-2; i++) {
            x = h * i - h/2;
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
