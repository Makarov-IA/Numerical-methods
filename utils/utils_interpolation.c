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
        default:
            return "unknown";
    }
}

void fill_uniform_nodes(double *nodes, int n) {
    double h = 1.0 / (double)(n - 1);
    for (int i = 0; i < n; ++i) {
        nodes[i] = h * (double)i;
    }
}

void fill_chebyshev_nodes(double *nodes, int n) {
    for (int i = 0; i < n; ++i) {
        double t = cos(M_PI * (2.0 * (double)i + 1.0) / (2.0 * (double)n));
        nodes[i] = 0.5 * (1.0 + t);
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
    for (int i = 0; i < n; ++i) {
        int pivot = i;
        double max_val = fabs(matrix[i * n + i]);
        for (int r = i + 1; r < n; ++r) {
            double val = fabs(matrix[r * n + i]);
            if (val > max_val) {
                max_val = val;
                pivot = r;
            }
        }

        if (max_val < 1e-14) {
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
        if (fabs(matrix[i * n + i]) < 1e-14) {
            return 1;
        }
        solution[i] = sum / matrix[i * n + i];
    }

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

void print_table(const char *title, double (*func)(double),
                 const double *nodes, int n_nodes, int n_eval) {
    double *values = (double *)malloc(n_nodes * sizeof(double));
    double *matrix = (double *)malloc(n_nodes * n_nodes * sizeof(double));
    double *rhs = (double *)malloc(n_nodes * sizeof(double));
    double *coeff = (double *)malloc(n_nodes * sizeof(double));
    double *weights = (double *)malloc(n_nodes * sizeof(double));

    if (!values || !matrix || !rhs || !coeff || !weights) {
        fprintf(stderr, "Error: memory allocation failed\n");
        free(values);
        free(matrix);
        free(rhs);
        free(coeff);
        free(weights);
        return;
    }

    for (int i = 0; i < n_nodes; ++i) {
        values[i] = func(nodes[i]);
        rhs[i] = values[i];
    }

    fill_vandermonde(matrix, nodes, n_nodes);

    if (solve_linear_system(matrix, rhs, coeff, n_nodes) != 0) {
        fprintf(stderr, "Error: system is singular or ill-conditioned\n");
        free(values);
        free(matrix);
        free(rhs);
        free(coeff);
        free(weights);
        return;
    }

    fill_barycentric_weights(nodes, weights, n_nodes);

    printf("%s\n", title);
    printf("%12s %22s %22s %22s\n", "x", "exact", "SLAE", "Lagrange");

    if (n_eval == 1) {
        double x = 0.0;
        double exact = func(x);
        double approx_slae = eval_polynomial(coeff, n_nodes, x);
        double approx_lagrange = eval_lagrange_barycentric(nodes, values, weights, n_nodes, x);
        printf("%12.6f %22.12e %22.12e %22.12e\n", x, exact, approx_slae, approx_lagrange);
    } else {
        double step = 1.0 / (double)(n_eval - 1);
        for (int i = 0; i < n_eval; ++i) {
            double x = step * (double)i;
            double exact = func(x);
            double approx_slae = eval_polynomial(coeff, n_nodes, x);
            double approx_lagrange = eval_lagrange_barycentric(nodes, values, weights, n_nodes, x);
            printf("%12.6f %22.12e %22.12e %22.12e\n", x, exact, approx_slae, approx_lagrange);
        }
    }

    printf("\n");

    free(values);
    free(matrix);
    free(rhs);
    free(coeff);
    free(weights);
}
