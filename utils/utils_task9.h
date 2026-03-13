#ifndef UTILS_TASK9_H
#define UTILS_TASK9_H

int task9_run(int n_nodes, double a, double b, unsigned int seed);
int task9_run_for_set(int set_number, int n_nodes, double a, double b, unsigned int seed);
int task9_get_test_function(int set_number, const char **name, double (**func)(double));
void task9_sort_nodes(double *nodes, int n);
int task9_fill_random_nodes(double *nodes, int n, double a, double b, unsigned int seed);
int task9_find_interval(const double *nodes, int n, double x);
int task9_build_comparison_points(const double *nodes, int n, double *points);

#endif
