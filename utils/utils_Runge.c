#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include "utils_Runge.h"

void set_k(double xn, double yn, double *k1, double *k2, double *k3, \
    double *k4, double h, double (*f) (double, double)) {
    *k1 = h*f(xn, yn);
    *k2 = h*f(xn + 0.5*h, yn + 0.5*(*k1));
    *k3 = h*f(xn + 0.5*h, yn + 0.5*(*k2));
    *k4 = h*f(xn + h, yn + (*k3));
}

double delta_y(double k1, double k2, double k3, double k4) {
    return 1.0/6.0*(k1 + 2*k2 + 2*k3 + k4);
}

int get_step(double error, double thr_error, int K, double *h, double y_small, double *y, double *x) {
    
    if (fabs(error) > thr_error) {
        *h = *h/2.0;
        return 0;
    }

    else if (fabs(error) < thr_error/K) {
        *x = *x + *h;
        *y = y_small;
        *h = *h*2.0;
        return 1;
    }

    else {
        *y = y_small;
        *x = *x + *h;
        return 1;
    }
}

void make_step(double *xn, double *yn, double h, double (*f) (double, double)) {
    double k1, k2, k3, k4;
    set_k(*xn, *yn, &k1, &k2, &k3, &k4, h, f);
    *yn = *yn + delta_y(k1, k2, k3, k4);
    *xn = *xn + h;
}

double test_function_1 (double xn, double yn) {
    (void) yn;
    return -sin(xn);
}

double test_function_sol_1 (double xn) {
    return cos(xn);
}

double test_function_2 (double xn, double yn) {
    (void) yn;
    return 3*pow(xn,2) + 4*xn + 3;
}

double test_function_sol_2 (double xn) {
    return pow(xn,3) + 2*pow(xn,2) + 3*xn + 4;
}

