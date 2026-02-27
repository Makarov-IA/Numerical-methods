#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils_interpolation.h"
#include "utils_task9.h"

#define EPS_NODES 1e-12

typedef enum {
    NODES_UNIFORM = 0,
    NODES_CHEBYSHEV = 1,
    NODES_RANDOM = 2
} NodeType;

typedef struct {
    const char *name;
    NodeType type;
} NodeConfig;

typedef struct {
    const char *name;
    const char *derivative_name;
    double (*func)(double);
    double (*derivative)(double);
} TestFunctionConfig;

static const NodeConfig NODE_CONFIGS[] = {
    {"uniform", NODES_UNIFORM},
    {"chebyshev", NODES_CHEBYSHEV},
    {"random", NODES_RANDOM},
};

static int cmp_double(const void *lhs, const void *rhs) {
    const double a = *(const double *)lhs;
    const double b = *(const double *)rhs;
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    return 0;
}

static double func_1(double x) {
    return 1.0 / (1.0 + 25.0 * x * x);
}

static double dfunc_1(double x) {
    const double den = 1.0 + 25.0 * x * x;
    return -50.0 * x / (den * den);
}

static double func_2(double x) {
    return sin(2.0 * M_PI * x);
}

static double dfunc_2(double x) {
    return 2.0 * M_PI * cos(2.0 * M_PI * x);
}

static double func_3(double x) {
    return exp(x);
}

static double dfunc_3(double x) {
    return exp(x);
}

static double func_4(double x) {
    const double t = x * (1.0 - x);
    return t * t * exp(x);
}

static double dfunc_4(double x) {
    const double t = x * (1.0 - x);
    const double dt = 1.0 - 2.0 * x;
    const double g = t * t;
    const double dg = 2.0 * t * dt;
    return (dg + g) * exp(x);
}

static double func_5(double x) {
    return cos(6.0 * M_PI * x);
}

static double dfunc_5(double x) {
    return -6.0 * M_PI * sin(6.0 * M_PI * x);
}

static double func_6(double x) {
    return log(1.0 + x);
}

static double dfunc_6(double x) {
    return 1.0 / (1.0 + x);
}

static double func_7(double x) {
    return sqrt(1.0 + x);
}

static double dfunc_7(double x) {
    return 1.0 / (2.0 * sqrt(1.0 + x));
}

static const TestFunctionConfig *select_test_function(int set_number) {
    static const TestFunctionConfig configs[] = {
        {"1/(1+25*x^2)", "-50*x/(1+25*x^2)^2", func_1, dfunc_1},
        {"sin(2*pi*x)", "2*pi*cos(2*pi*x)", func_2, dfunc_2},
        {"exp(x)", "exp(x)", func_3, dfunc_3},
        {"x^2*(1-x)^2*exp(x)", "exp(x)*(2*x*(1-x)*(1-2*x)+x^2*(1-x)^2)", func_4, dfunc_4},
        {"cos(6*pi*x)", "-6*pi*sin(6*pi*x)", func_5, dfunc_5},
        {"log(1+x)", "1/(1+x)", func_6, dfunc_6},
        {"sqrt(1+x)", "1/(2*sqrt(1+x))", func_7, dfunc_7},
    };

    if (set_number < 1 || set_number > (int)(sizeof(configs) / sizeof(configs[0]))) {
        return NULL;
    }

    return &configs[set_number - 1];
}

static int fill_random_nodes(double *nodes, int n, double a, double b) {
    for (int attempt = 0; attempt < 1000; ++attempt) {
        for (int i = 0; i < n; ++i) {
            const double u = (double)rand() / (double)RAND_MAX;
            nodes[i] = a + (b - a) * u;
        }
        qsort(nodes, (size_t)n, sizeof(double), cmp_double);

        int ok = 1;
        for (int i = 1; i < n; ++i) {
            if (nodes[i] - nodes[i - 1] < EPS_NODES) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            return 0;
        }
    }
    return 1;
}

static int build_hermite_coefficients(const double *nodes, const double *values,
                                      const double *deriv, int n,
                                      double *coeff_a, double *coeff_b) {
    for (int i = 0; i < n - 1; ++i) {
        const double h = nodes[i + 1] - nodes[i];
        if (h < EPS_NODES) {
            return 1;
        }
        coeff_a[i] = (deriv[i + 1] + deriv[i]) / (h * h)
                   - 2.0 * (values[i + 1] - values[i]) / (h * h * h);
        coeff_b[i] = 3.0 * (values[i + 1] - values[i]) / (h * h)
                   - (2.0 * deriv[i] + deriv[i + 1]) / h;
    }
    return 0;
}

static int find_interval(const double *nodes, int n, double x) {
    if (x <= nodes[0]) {
        return 0;
    }
    if (x >= nodes[n - 1]) {
        return n - 2;
    }

    int left = 0;
    int right = n - 2;
    while (left <= right) {
        const int mid = left + (right - left) / 2;
        if (x < nodes[mid]) {
            right = mid - 1;
        } else if (x > nodes[mid + 1]) {
            left = mid + 1;
        } else {
            return mid;
        }
    }
    return n - 2;
}

static double eval_hermite_piecewise(const double *nodes, const double *values,
                                     const double *deriv, const double *coeff_a,
                                     const double *coeff_b, int n, double x) {
    const int i = find_interval(nodes, n, x);
    const double dx = x - nodes[i];
    return coeff_a[i] * dx * dx * dx
         + coeff_b[i] * dx * dx
         + deriv[i] * dx
         + values[i];
}

static int build_comparison_points(const double *nodes, int n, double *points) {
    int k = 0;
    for (int i = 0; i < n - 1; ++i) {
        const double h = nodes[i + 1] - nodes[i];
        points[k++] = nodes[i];
        points[k++] = nodes[i] + h / 3.0;
        points[k++] = nodes[i] + 2.0 * h / 3.0;
    }
    points[k++] = nodes[n - 1];
    return k;
}

static int fill_nodes_for_case(const NodeConfig *config, double *nodes, int n_nodes,
                               double a, double b, unsigned int seed) {
    if (config->type == NODES_UNIFORM) {
        fill_uniform_nodes(nodes, n_nodes, a, b);
        return 0;
    }

    if (config->type == NODES_CHEBYSHEV) {
        fill_chebyshev_nodes(nodes, n_nodes, a, b);
        qsort(nodes, (size_t)n_nodes, sizeof(double), cmp_double);
        return 0;
    }

    srand(seed);
    return fill_random_nodes(nodes, n_nodes, a, b);
}

static int run_case(const TestFunctionConfig *test, const NodeConfig *config,
                    int n_nodes, double a, double b, unsigned int seed) {
    double *nodes = NULL;
    double *values = NULL;
    double *deriv = NULL;
    double *matrix = NULL;
    double *rhs = NULL;
    double *coeff = NULL;
    double *coeff_a = NULL;
    double *coeff_b = NULL;
    double *cmp_points = NULL;
    FILE *table_file = NULL;
    FILE *nodes_file = NULL;
    char table_path[256];
    char nodes_path[256];
    int status = 1;

    const int n_cmp = 3 * n_nodes - 2;

    nodes = (double *)malloc((size_t)n_nodes * sizeof(double));
    values = (double *)malloc((size_t)n_nodes * sizeof(double));
    deriv = (double *)malloc((size_t)n_nodes * sizeof(double));
    matrix = (double *)malloc((size_t)n_nodes * (size_t)n_nodes * sizeof(double));
    rhs = (double *)malloc((size_t)n_nodes * sizeof(double));
    coeff = (double *)malloc((size_t)n_nodes * sizeof(double));
    coeff_a = (double *)malloc((size_t)(n_nodes - 1) * sizeof(double));
    coeff_b = (double *)malloc((size_t)(n_nodes - 1) * sizeof(double));
    cmp_points = (double *)malloc((size_t)n_cmp * sizeof(double));

    if (!nodes || !values || !deriv || !matrix || !rhs || !coeff || !coeff_a || !coeff_b || !cmp_points) {
        fprintf(stderr, "Error: memory allocation failed\n");
        goto cleanup;
    }

    if (fill_nodes_for_case(config, nodes, n_nodes, a, b, seed) != 0) {
        fprintf(stderr, "Error: failed to build %s nodes\n", config->name);
        goto cleanup;
    }

    for (int i = 0; i < n_nodes; ++i) {
        values[i] = test->func(nodes[i]);
        deriv[i] = test->derivative(nodes[i]);
        rhs[i] = values[i];
    }

    fill_vandermonde(matrix, nodes, n_nodes);
    if (solve_linear_system(matrix, rhs, coeff, n_nodes) != 0) {
        fprintf(stderr, "Error: Vandermonde system is singular\n");
        goto cleanup;
    }

    if (build_hermite_coefficients(nodes, values, deriv, n_nodes, coeff_a, coeff_b) != 0) {
        fprintf(stderr, "Error: failed to build Hermite coefficients\n");
        goto cleanup;
    }

    snprintf(table_path, sizeof(table_path), "data_plot/%s.txt", config->name);
    snprintf(nodes_path, sizeof(nodes_path), "data_plot/%s_nodes.txt", config->name);
    table_file = fopen(table_path, "w");
    nodes_file = fopen(nodes_path, "w");
    if (!table_file || !nodes_file) {
        fprintf(stderr, "Error: cannot open output files for %s nodes\n", config->name);
        goto cleanup;
    }

    printf("%s nodes\n", config->name);
    printf("%14s %22s %22s %22s %22s\n", "x", "f(x)", "P_{n-1}(x)", "H(x)", "E_n(x)");

    const int actual_cmp = build_comparison_points(nodes, n_nodes, cmp_points);
    double max_diff = 0.0;

    for (int i = 0; i < actual_cmp; ++i) {
        const double x = cmp_points[i];
        const double fx = test->func(x);
        const double px = eval_polynomial(coeff, n_nodes, x);
        const double hx = eval_hermite_piecewise(nodes, values, deriv, coeff_a, coeff_b, n_nodes, x);
        const double en = fabs(px - hx);
        if (en > max_diff) {
            max_diff = en;
        }

        printf("%14.8f %22.12e %22.12e %22.12e %22.12e\n", x, fx, px, hx, en);
        fprintf(table_file, "%.20f %.20f %.20f %.20f %.20f\n", x, fx, px, hx, en);
    }
    printf("max E_n(x) = %.12e\n\n", max_diff);

    for (int i = 0; i < n_nodes; ++i) {
        fprintf(nodes_file, "%.20f %.20f %.20f\n", nodes[i], values[i], deriv[i]);
    }

    status = 0;

cleanup:
    if (table_file) {
        fclose(table_file);
    }
    if (nodes_file) {
        fclose(nodes_file);
    }
    free(nodes);
    free(values);
    free(deriv);
    free(matrix);
    free(rhs);
    free(coeff);
    free(coeff_a);
    free(coeff_b);
    free(cmp_points);
    return status;
}

int task9_run_for_set(int set_number, int n_nodes, double a, double b, unsigned int seed) {
    const TestFunctionConfig *test = select_test_function(set_number);

    if (!test) {
        fprintf(stderr, "Error: incorrect set_number=%d (expected 1..7)\n", set_number);
        return 1;
    }

    if (n_nodes < 2) {
        fprintf(stderr, "Error: n_nodes must be >= 2\n");
        return 1;
    }
    if (a >= b) {
        fprintf(stderr, "Error: require a < b\n");
        return 1;
    }
    if ((set_number == 6 || set_number == 7) && a <= -1.0) {
        fprintf(stderr, "Error: for set %d require interval with a > -1\n", set_number);
        return 1;
    }

    printf("Set %d\n", set_number);
    printf("Function: f(x) = %s\n", test->name);
    printf("Derivative: f'(x) = %s\n", test->derivative_name);
    printf("Interval [%.6f, %.6f], nodes=%d, seed=%u\n\n", a, b, n_nodes, seed);

    const size_t n_cases = sizeof(NODE_CONFIGS) / sizeof(NODE_CONFIGS[0]);
    for (size_t i = 0; i < n_cases; ++i) {
        if (run_case(test, &NODE_CONFIGS[i], n_nodes, a, b, seed) != 0) {
            return 1;
        }
    }

    return 0;
}

int task9_run(int n_nodes, double a, double b, unsigned int seed) {
    return task9_run_for_set(1, n_nodes, a, b, seed);
}
