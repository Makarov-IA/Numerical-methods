#ifndef UTILS_RUNGE_H
#define UTILS_RUNGE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include "utils_Runge.h"

void set_k(double xn, double yn, double *k1, double *k2, double *k3, \
    double *k4, double h, double (*f) (double, double, double*), \
    double *additional_args_to_function);
double delta_y(double k1, double k2, double k3, double k4);
int get_step(double error, double thr_error, int K, \
    double *h, double y_small, double *y, double *x);
void make_step(double *xn, double *yn, double h, double (*f) \
(double, double, double*), double *additional_args_to_function);
double test_function_1 (double xn, double yn, double *additional_args_to_function);
double test_function_sol_1 (double xn, double *additional_args_to_function);
double test_function_2 (double xn, double yn, double *additional_args_to_function);
double test_function_sol_2 (double xn, double *additional_args_to_function);
double test_function_3 (double xn, double yn, double *additional_args_to_function);
double test_function_sol_3 (double xn, double *additional_args_to_function);


#endif
