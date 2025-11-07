#ifndef UTILS_RUNGE_H
#define UTILS_RUNGE_H

void set_k(double xn, double yn, double *k1, double *k2, double *k3, double *k4, double h, double (*f) (double, double));
double delta_y(double k1, double k2, double k3, double k4);
int get_step(double error, double thr_error, int K, double *h, double y_small, double *y, double *x);
void make_step(double *xn, double *yn, double h, double (*f) (double, double));
double test_function_1 (double xn, double yn);
double test_function_sol_1 (double xn);
double test_function_2 (double xn, double yn);
double test_function_sol_2 (double xn);

#endif
