#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include "utils_Runge.h"

void set_k(double xn, double yn, double *k1, double *k2, double *k3, \
    double *k4, double h, double (*f) (double, double, double*), \
    double *additional_args_to_function) {
    //Set k_i according to Runge method
    *k1 = h*f(xn, yn, additional_args_to_function);
    *k2 = h*f(xn + 0.5*h, yn + 0.5*(*k1), additional_args_to_function);
    *k3 = h*f(xn + 0.5*h, yn + 0.5*(*k2), additional_args_to_function);
    *k4 = h*f(xn + h, yn + (*k3), additional_args_to_function);
}

double delta_y(double k1, double k2, double k3, double k4) {
    //Calc delta accroding to Runge method
    return 1.0/6.0*(k1 + 2*k2 + 2*k3 + k4);
}

int get_step(double error, double thr_error, int K, double *h, double y_small, double *y, double *x) {
    //If error is too big set the smaller step. don't make it 
    if (fabs(error) > thr_error) {
        *h = *h/2.0;
        return 0;
    }
    //If error is too small set the bigger step. do it
    else if (fabs(error) < thr_error/K) {
        *x = *x + *h;
        *y = y_small;
        *h = *h*2.0;
        return 1;
    }
    //It ok with the step. do it
    else {
        *y = y_small;
        *x = *x + *h;
        return 1;
    }
}

void make_step(double *xn, double *yn, double h, double (*f) \
(double, double, double*), double *additional_args_to_function) {
    double k1, k2, k3, k4;
    set_k(*xn, *yn, &k1, &k2, &k3, &k4, h, f, additional_args_to_function);
    *yn = *yn + delta_y(k1, k2, k3, k4);
    *xn = *xn + h;
}

double test_function_1 (double xn, double yn, double *additional_args_to_function) {
    (void) yn;
    (void) additional_args_to_function;
    return -sin(xn);
}

double test_function_sol_1 (double xn, double *additional_args_to_function) {
    (void) additional_args_to_function;
    return cos(xn);
}

double test_function_2 (double xn, double yn, double *additional_args_to_function) {
    (void) yn;
    (void) additional_args_to_function;
    return 3*pow(xn,2) + 4*xn + 3;
}

double test_function_sol_2 (double xn, double *additional_args_to_function) {
    (void) additional_args_to_function;
    return pow(xn,3) + 2*pow(xn,2) + 3*xn + 4;
}

double test_function_3 (double xn, double yn, double *additional_args_to_function) {
    (void) yn;
    double A = additional_args_to_function[0];
    double alpha = additional_args_to_function[1];
    double beta = additional_args_to_function[2];
    return A*beta*pow((xn-alpha),beta-1)*exp(A*pow((xn-alpha),beta));
}

double test_function_sol_3 (double xn, double *additional_args_to_function) {
    double A = additional_args_to_function[0];
    double alpha = additional_args_to_function[1];
    double beta = additional_args_to_function[2];
    return exp(A*pow((xn-alpha),beta));
}

