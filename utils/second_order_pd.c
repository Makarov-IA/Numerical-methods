#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "second_order_pd.h"

// Решение u_t = u_xx + p*u + f, Neumann: u_x(t,0)=0, u_x(t,1)=0
double* solve_explicitly(double* inital_conditions, int number_of_points_x, \
    int number_of_points_t, double T, \
    double(*f)(double, double),double(*p)(double, double)) {

    double tau = T/(double) (number_of_points_t-1);
    double h = 1.0/(double) (number_of_points_x-1);
    double hh = h*h;
    int row_prev, row_cur;

    // Проверка базовой устойчивости явной схемы теплопроводности
    if (tau/hh > 0.5) {
        fprintf(stderr, "Warning: explicit scheme may be unstable (tau/h^2 = %f)\n", tau/hh);
    }

    double * u = (double*)malloc((number_of_points_t * number_of_points_x)*sizeof(double));

    // Добавляем начальные значения
    for (int i = 0; i < number_of_points_x; i++) {
        u[i] = inital_conditions[i];
    }

    // Идем дальше по времени
    for (int j = 1; j < number_of_points_t; j++) {
        row_prev = (j-1) * number_of_points_x;
        row_cur = j * number_of_points_x;

        // Для x=0 граничное условие
        u[row_cur] = u[row_prev] +
            tau * ( 2.0 * (u[row_prev + 1] - u[row_prev]) / hh
            + p(tau * j, 0.0) * u[row_prev]
            + f(tau * j, 0.0));

        // Для x=1 граничное условие
        u[row_cur + number_of_points_x - 1] = u[row_prev + number_of_points_x - 1] +
            tau * ( 2.0 * (u[row_prev + number_of_points_x - 2] - u[row_prev + number_of_points_x - 1]) / hh
            + p(tau * j, 1.0) * u[row_prev + number_of_points_x - 1]
            + f(tau * j, 1.0));

        // Внутренние узлы
        for (int k = 1; k < number_of_points_x - 1; k++) {
            u[row_cur + k] = u[row_prev + k] +
                tau * ((u[row_prev + k + 1] - 2 * u[row_prev + k] + u[row_prev + k - 1]) / hh
                + p(tau * j, k * h) * u[row_prev + k]
                + f(tau * j, k * h));
        }
    }
    return u;
}

// Набор 1: p=0, f=0, u0=cos(pi x)
double p_pd_1(double t, double x) {
    (void)t; (void)x; return 0.0;
}

double f_pd_1(double t, double x) {
    (void)t; (void)x; return 0.0;
}

double u0_pd_1(double x) {
    return cos(M_PI * x);
}

double theoretical_pd_1(double t, double x) {
    return cos(M_PI * x) * exp(-M_PI * M_PI * t);
}

// Набор 2: p=0, f=0, u0=cos(2*pi*x) => точное решение cos(2*pi*x)*exp(-4*pi^2*t)
double p_pd_2(double t, double x) {
    (void)t; (void)x; return 0.0;
}

double f_pd_2(double t, double x) {
    (void)t; (void)x; return 0.0;
}

double u0_pd_2(double x) {
    return cos(2.0 * M_PI * x);
}

double theoretical_pd_2(double t, double x) {
    return cos(2.0 * M_PI * x) * exp(-4.0 * M_PI * M_PI * t);
}

// Набор 3: p = x + t, u = e^{-t} cos(pi x), f = u*(pi^2 - 1 - x - t)
double p_pd_3(double t, double x) {
    return x + t;
}

double f_pd_3(double t, double x) {
    double u = exp(-t) * cos(M_PI * x);
    return u * (M_PI * M_PI - 1.0 - x - t);
}

double u0_pd_3(double x) {
    return cos(M_PI * x);
}

double theoretical_pd_3(double t, double x) {
    return exp(-t) * cos(M_PI * x);
}

// Набор 4: p = 1, u = cos(t) cos(2 pi x), f = u_t - u_xx - p*u
double p_pd_4(double t, double x) {
    (void)t; (void)x; return 1.0;
}

double f_pd_4(double t, double x) {
    double cos_2pix = cos(2.0 * M_PI * x);
    double u = cos(t) * cos_2pix;
    double u_t = -sin(t) * cos_2pix;
    return u_t + 4.0 * M_PI * M_PI * u - u;
}

double u0_pd_4(double x) {
    return cos(2.0 * M_PI * x);
}

double theoretical_pd_4(double t, double x) {
    return cos(t) * cos(2.0 * M_PI * x);
}
