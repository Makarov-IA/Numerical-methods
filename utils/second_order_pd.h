#ifndef SECOND_ORDER_PD_H
#define SECOND_ORDER_PD_H

double* solve_explicitly(double* inital_conditions, int number_of_points_x, \
    int number_of_points_t, double T, \
    double(*f)(double, double),double(*p)(double, double));

// Примеры наборов p(t,x), f(t,x) и u(0,x)
double p_pd_1(double t, double x);
double f_pd_1(double t, double x);
double u0_pd_1(double x);
double theoretical_pd_1(double t, double x);

double p_pd_2(double t, double x);
double f_pd_2(double t, double x);
double u0_pd_2(double x);
double theoretical_pd_2(double t, double x);

double p_pd_3(double t, double x);
double f_pd_3(double t, double x);
double u0_pd_3(double x);
double theoretical_pd_3(double t, double x);

double p_pd_4(double t, double x);
double f_pd_4(double t, double x);
double u0_pd_4(double x);
double theoretical_pd_4(double t, double x);

#endif
