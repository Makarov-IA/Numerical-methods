#ifndef UTILS_INTERPOLATION_H
#define UTILS_INTERPOLATION_H

double (*select_function(int set_number))(double);
const char *select_function_name(int set_number);

void fill_uniform_nodes(double *nodes, int n, double a, double b);
void fill_chebyshev_nodes(double *nodes, int n, double a, double b);

void fill_vandermonde(double *matrix, const double *nodes, int n);
int solve_linear_system(double *matrix, double *rhs, double *solution, int n);

double eval_polynomial(const double *coeff, int n, double x);
double eval_lagrange_simple(const double *nodes, const double *values, int n, double x);

void print_table(const char *title, double (*func)(double),
                 const double *nodes, int n_nodes, int n_eval, double a, double b);
int write_table_data(const char *path, double (*func)(double),
                     const double *nodes, int n_nodes, int n_eval, double a, double b);
int write_nodes_data(const char *path, double (*func)(double),
                     const double *nodes, int n_nodes);

#endif
