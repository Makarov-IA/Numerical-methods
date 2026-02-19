#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils_interpolation.h"

#define EPS 1e-12

static double func_1(double x) {
    return 1.0 / (1.0 + 25.0 * x * x);
}

static double func_2(double x) {
    return sin(2.0 * M_PI * x);
}

static double func_3(double x) {
    return exp(x);
}

static double func_4(double x) {
    double t = x * (1.0 - x);
    return t * t * exp(x);
}

static double func_5(double x) {
    return 1.0 / (1.0 + 25.0 * x * x);
}

double (*select_function(int set_number))(double) {
    switch (set_number) {
        case 1:
            return func_1;
        case 2:
            return func_2;
        case 3:
            return func_3;
        case 4:
            return func_4;
        case 5:
            return func_5;
        default:
            return NULL;
    }
}

const char *select_function_name(int set_number) {
    switch (set_number) {
        case 1:
            return "1/(1+25*x^2)";
        case 2:
            return "sin(2*pi*x)";
        case 3:
            return "exp(x)";
        case 4:
            return "x^2*(1-x)^2*exp(x)";
        case 5:
            return "Runge: 1/(1+25*x^2)";
        default:
            return "unknown";
    }
}

void fill_uniform_nodes(double *nodes, int n, double a, double b) {
    double h = (b - a) / (double)(n - 1);
    for (int i = 0; i < n; ++i) {
        nodes[i] = a + h * (double)i;
    }
}

void fill_chebyshev_nodes(double *nodes, int n, double a, double b) {
    for (int i = 0; i < n; ++i) {
        double t = cos(M_PI * (2.0 * (double)i + 1.0) / (2.0 * (double)n));
        nodes[i] = 0.5 * (a + b) + 0.5 * (b - a) * t;
    }
}

void fill_vandermonde(double *matrix, const double *nodes, int n) {
    for (int i = 0; i < n; ++i) {
        double power = 1.0;
        for (int j = 0; j < n; ++j) {
            matrix[i * n + j] = power;
            power *= nodes[i];
        }
    }
}

int solve_linear_system(double *matrix, double *rhs, double *solution, int n) {
    const double pivot_eps = 1e-30;
    double *scale = (double *)malloc(n * sizeof(double));
    if (!scale) {
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        double row_max = 0.0;
        for (int j = 0; j < n; ++j) {
            double val = fabs(matrix[i * n + j]);
            if (val > row_max) {
                row_max = val;
            }
        }
        if (row_max < pivot_eps) {
            free(scale);
            return 1;
        }
        scale[i] = row_max;
    }

    for (int i = 0; i < n; ++i) {
        int pivot = i;
        double max_ratio = fabs(matrix[i * n + i]) / scale[i];
        for (int r = i + 1; r < n; ++r) {
            double ratio = fabs(matrix[r * n + i]) / scale[r];
            if (ratio > max_ratio) {
                max_ratio = ratio;
                pivot = r;
            }
        }

        if (fabs(matrix[pivot * n + i]) < pivot_eps) {
            free(scale);
            return 1;
        }

        if (pivot != i) {
            for (int c = i; c < n; ++c) {
                double tmp = matrix[i * n + c];
                matrix[i * n + c] = matrix[pivot * n + c];
                matrix[pivot * n + c] = tmp;
            }
            {
                double tmp = rhs[i];
                rhs[i] = rhs[pivot];
                rhs[pivot] = tmp;
            }
            {
                double tmp = scale[i];
                scale[i] = scale[pivot];
                scale[pivot] = tmp;
            }
        }

        for (int r = i + 1; r < n; ++r) {
            double factor = matrix[r * n + i] / matrix[i * n + i];
            matrix[r * n + i] = 0.0;
            for (int c = i + 1; c < n; ++c) {
                matrix[r * n + c] -= factor * matrix[i * n + c];
            }
            rhs[r] -= factor * rhs[i];
        }
    }

    for (int i = n - 1; i >= 0; --i) {
        double sum = rhs[i];
        for (int c = i + 1; c < n; ++c) {
            sum -= matrix[i * n + c] * solution[c];
        }
        if (fabs(matrix[i * n + i]) < pivot_eps) {
            free(scale);
            return 1;
        }
        solution[i] = sum / matrix[i * n + i];
    }

    free(scale);
    return 0;
}

double eval_polynomial(const double *coeff, int n, double x) {
    double result = coeff[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        result = result * x + coeff[i];
    }
    return result;
}

void fill_barycentric_weights(const double *nodes, double *weights, int n) {
    for (int i = 0; i < n; ++i) {
        double w = 1.0;
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                continue;
            }
            w /= (nodes[i] - nodes[j]);
        }
        weights[i] = w;
    }
}

double eval_lagrange_barycentric(const double *nodes, const double *values,
                                 const double *weights, int n, double x) {
    double numerator = 0.0;
    double denominator = 0.0;

    for (int i = 0; i < n; ++i) {
        double diff = x - nodes[i];
        if (fabs(diff) < EPS) {
            return values[i];
        }
        double term = weights[i] / diff;
        numerator += term * values[i];
        denominator += term;
    }

    return numerator / denominator;
}

static double map_to_reference(double x, double a, double b) {
    return (2.0 * x - (a + b)) / (b - a);
}

static int prepare_interpolation(double (*func)(double), const double *nodes, int n_nodes,
                                 double a, double b,
                                 double **values_out, double **coeff_out, double **weights_out) {
    double *values = (double *)malloc(n_nodes * sizeof(double));
    double *matrix = (double *)malloc(n_nodes * n_nodes * sizeof(double));
    double *rhs = (double *)malloc(n_nodes * sizeof(double));
    double *coeff = (double *)malloc(n_nodes * sizeof(double));
    double *weights = (double *)malloc(n_nodes * sizeof(double));
    double *scaled_nodes = (double *)malloc(n_nodes * sizeof(double));

    if (!values || !matrix || !rhs || !coeff || !weights || !scaled_nodes) {
        fprintf(stderr, "Error: memory allocation failed\n");
        free(values);
        free(matrix);
        free(rhs);
        free(coeff);
        free(weights);
        free(scaled_nodes);
        return 1;
    }

    for (int i = 0; i < n_nodes; ++i) {
        values[i] = func(nodes[i]);
        rhs[i] = values[i];
        scaled_nodes[i] = map_to_reference(nodes[i], a, b);
    }

    fill_vandermonde(matrix, scaled_nodes, n_nodes);

    if (solve_linear_system(matrix, rhs, coeff, n_nodes) != 0) {
        fprintf(stderr, "Error: system is singular or ill-conditioned\n");
        free(values);
        free(matrix);
        free(rhs);
        free(coeff);
        free(weights);
        free(scaled_nodes);
        return 1;
    }

    fill_barycentric_weights(nodes, weights, n_nodes);

    free(matrix);
    free(rhs);
    free(scaled_nodes);

    *values_out = values;
    *coeff_out = coeff;
    *weights_out = weights;
    return 0;
}

void print_table(const char *title, double (*func)(double),
                 const double *nodes, int n_nodes, int n_eval, double a, double b) {
    double *values = NULL;
    double *coeff = NULL;
    double *weights = NULL;

    if (prepare_interpolation(func, nodes, n_nodes, a, b, &values, &coeff, &weights) != 0) {
        return;
    }

    printf("%s\n", title);
    printf("%12s %22s %22s %22s %22s\n", "x", "exact", "SLAE", "Lagrange", "diff");

    if (n_eval == 1) {
        double x = a;
        double exact = func(x);
        double approx_slae = eval_polynomial(coeff, n_nodes, map_to_reference(x, a, b));
        double approx_lagrange = eval_lagrange_barycentric(nodes, values, weights, n_nodes, x);
        printf("%12.6f %22.12e %22.12e %22.12e %22.12e\n",
               x, exact, approx_slae, approx_lagrange, fabs(approx_slae - approx_lagrange));
    } else {
        double step = (b - a) / (double)(n_eval - 1);
        for (int i = 0; i < n_eval; ++i) {
            double x = a + step * (double)i;
            double exact = func(x);
            double approx_slae = eval_polynomial(coeff, n_nodes, map_to_reference(x, a, b));
            double approx_lagrange = eval_lagrange_barycentric(nodes, values, weights, n_nodes, x);
            printf("%12.6f %22.12e %22.12e %22.12e %22.12e\n",
                   x, exact, approx_slae, approx_lagrange, fabs(approx_slae - approx_lagrange));
        }
    }

    printf("\n");

    free(values);
    free(coeff);
    free(weights);
}

int write_table_data(const char *path, double (*func)(double),
                     const double *nodes, int n_nodes, int n_eval, double a, double b) {
    FILE *file = fopen(path, "w");
    if (!file) {
        fprintf(stderr, "Error: can't open %s\n", path);
        return 1;
    }

    double *values = NULL;
    double *coeff = NULL;
    double *weights = NULL;
    if (prepare_interpolation(func, nodes, n_nodes, a, b, &values, &coeff, &weights) != 0) {
        fclose(file);
        return 1;
    }

    if (n_eval == 1) {
        double x = a;
        double exact = func(x);
        double approx_slae = eval_polynomial(coeff, n_nodes, map_to_reference(x, a, b));
        double approx_lagrange = eval_lagrange_barycentric(nodes, values, weights, n_nodes, x);
        fprintf(file, "%.10f %.15e %.15e %.15e %.15e\n",
                x, exact, approx_slae, approx_lagrange, fabs(approx_slae - approx_lagrange));
    } else {
        double step = (b - a) / (double)(n_eval - 1);
        for (int i = 0; i < n_eval; ++i) {
            double x = a + step * (double)i;
            double exact = func(x);
            double approx_slae = eval_polynomial(coeff, n_nodes, map_to_reference(x, a, b));
            double approx_lagrange = eval_lagrange_barycentric(nodes, values, weights, n_nodes, x);
            fprintf(file, "%.10f %.15e %.15e %.15e %.15e\n",
                    x, exact, approx_slae, approx_lagrange, fabs(approx_slae - approx_lagrange));
        }
    }

    free(values);
    free(coeff);
    free(weights);
    fclose(file);
    return 0;
}

int write_nodes_data(const char *path, double (*func)(double),
                     const double *nodes, int n_nodes) {
    FILE *file = fopen(path, "w");
    if (!file) {
        fprintf(stderr, "Error: can't open %s\n", path);
        return 1;
    }

    for (int i = 0; i < n_nodes; ++i) {
        double x = nodes[i];
        fprintf(file, "%.10f %.15e\n", x, func(x));
    }

    fclose(file);
    return 0;
}
