#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils_interpolation.h"
#include "utils_task10.h"

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
    double (*func)(double);
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

static double func_2(double x) {
    return sin(2.0 * M_PI * x);
}

static double func_3(double x) {
    return exp(x);
}

static double func_4(double x) {
    const double t = x * (1.0 - x);
    return t * t * exp(x);
}

static double func_5(double x) {
    return cos(6.0 * M_PI * x);
}

static double func_6(double x) {
    return log(1.0 + x);
}

static double func_7(double x) {
    return sqrt(1.0 + x);
}

static const TestFunctionConfig *select_test_function(int set_number) {
    static const TestFunctionConfig configs[] = {
        {"1/(1+25*x^2)", func_1},
        {"sin(2*pi*x)", func_2},
        {"exp(x)", func_3},
        {"x^2*(1-x)^2*exp(x)", func_4},
        {"cos(6*pi*x)", func_5},
        {"log(1+x)", func_6},
        {"sqrt(1+x)", func_7},
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

static int build_natural_cubic_spline(const double *nodes, const double *values, int n,
                                      double *coeff_b, double *coeff_c, double *coeff_d) {
    double *h = NULL;
    double *alpha = NULL;
    double *l = NULL;
    double *mu = NULL;
    double *z = NULL;
    double *c = NULL;
    int status = 1;

    h = (double *)malloc((size_t)(n - 1) * sizeof(double));
    alpha = (double *)calloc((size_t)n, sizeof(double));
    l = (double *)calloc((size_t)n, sizeof(double));
    mu = (double *)calloc((size_t)n, sizeof(double));
    z = (double *)calloc((size_t)n, sizeof(double));
    c = (double *)calloc((size_t)n, sizeof(double));

    if (!h || !alpha || !l || !mu || !z || !c) {
        goto cleanup;
    }

    for (int i = 0; i < n - 1; ++i) {
        h[i] = nodes[i + 1] - nodes[i];
        if (h[i] < EPS_NODES) {
            goto cleanup;
        }
    }

    if (n == 2) {
        coeff_b[0] = (values[1] - values[0]) / h[0];
        coeff_c[0] = 0.0;
        coeff_d[0] = 0.0;
        status = 0;
        goto cleanup;
    }

    for (int i = 1; i < n - 1; ++i) {
        alpha[i] = 3.0 * (values[i + 1] - values[i]) / h[i]
                 - 3.0 * (values[i] - values[i - 1]) / h[i - 1];
    }

    l[0] = 1.0;
    mu[0] = 0.0;
    z[0] = 0.0;

    for (int i = 1; i < n - 1; ++i) {
        l[i] = 2.0 * (nodes[i + 1] - nodes[i - 1]) - h[i - 1] * mu[i - 1];
        if (fabs(l[i]) < EPS_NODES) {
            goto cleanup;
        }
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }

    l[n - 1] = 1.0;
    z[n - 1] = 0.0;
    c[n - 1] = 0.0;

    for (int j = n - 2; j >= 0; --j) {
        c[j] = z[j] - mu[j] * c[j + 1];
        coeff_b[j] = (values[j + 1] - values[j]) / h[j]
                   - h[j] * (2.0 * c[j] + c[j + 1]) / 3.0;
        coeff_c[j] = c[j];
        coeff_d[j] = (c[j + 1] - c[j]) / (3.0 * h[j]);
    }

    status = 0;

cleanup:
    free(h);
    free(alpha);
    free(l);
    free(mu);
    free(z);
    free(c);
    return status;
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

static double eval_spline_piecewise(const double *nodes, const double *values,
                                    const double *coeff_b, const double *coeff_c,
                                    const double *coeff_d, int n, double x) {
    const int i = find_interval(nodes, n, x);
    const double dx = x - nodes[i];
    return values[i]
         + coeff_b[i] * dx
         + coeff_c[i] * dx * dx
         + coeff_d[i] * dx * dx * dx;
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
    double *matrix = NULL;
    double *rhs = NULL;
    double *coeff = NULL;
    double *coeff_b = NULL;
    double *coeff_c = NULL;
    double *coeff_d = NULL;
    double *cmp_points = NULL;
    FILE *table_file = NULL;
    FILE *nodes_file = NULL;
    char table_path[256];
    char nodes_path[256];
    int status = 1;

    const int n_cmp = 3 * n_nodes - 2;

    nodes = (double *)malloc((size_t)n_nodes * sizeof(double));
    values = (double *)malloc((size_t)n_nodes * sizeof(double));
    matrix = (double *)malloc((size_t)n_nodes * (size_t)n_nodes * sizeof(double));
    rhs = (double *)malloc((size_t)n_nodes * sizeof(double));
    coeff = (double *)malloc((size_t)n_nodes * sizeof(double));
    coeff_b = (double *)malloc((size_t)(n_nodes - 1) * sizeof(double));
    coeff_c = (double *)malloc((size_t)(n_nodes - 1) * sizeof(double));
    coeff_d = (double *)malloc((size_t)(n_nodes - 1) * sizeof(double));
    cmp_points = (double *)malloc((size_t)n_cmp * sizeof(double));

    if (!nodes || !values || !matrix || !rhs || !coeff || !coeff_b || !coeff_c || !coeff_d || !cmp_points) {
        fprintf(stderr, "Error: memory allocation failed\n");
        goto cleanup;
    }

    if (fill_nodes_for_case(config, nodes, n_nodes, a, b, seed) != 0) {
        fprintf(stderr, "Error: failed to build %s nodes\n", config->name);
        goto cleanup;
    }

    for (int i = 0; i < n_nodes; ++i) {
        values[i] = test->func(nodes[i]);
        rhs[i] = values[i];
    }

    fill_vandermonde(matrix, nodes, n_nodes);
    if (solve_linear_system(matrix, rhs, coeff, n_nodes) != 0) {
        fprintf(stderr, "Error: Vandermonde system is singular\n");
        goto cleanup;
    }

    if (build_natural_cubic_spline(nodes, values, n_nodes, coeff_b, coeff_c, coeff_d) != 0) {
        fprintf(stderr, "Error: failed to build cubic spline coefficients\n");
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
    printf("%14s %22s %22s %22s %22s\n", "x", "f(x)", "P_{n-1}(x)", "S(x)", "E_n(x)");

    const int actual_cmp = build_comparison_points(nodes, n_nodes, cmp_points);
    double max_diff = 0.0;

    for (int i = 0; i < actual_cmp; ++i) {
        const double x = cmp_points[i];
        const double fx = test->func(x);
        const double px = eval_polynomial(coeff, n_nodes, x);
        const double sx = eval_spline_piecewise(nodes, values, coeff_b, coeff_c, coeff_d, n_nodes, x);
        const double en = fabs(px - sx);
        if (en > max_diff) {
            max_diff = en;
        }

        printf("%14.8f %22.12e %22.12e %22.12e %22.12e\n", x, fx, px, sx, en);
        fprintf(table_file, "%.20f %.20f %.20f %.20f %.20f\n", x, fx, px, sx, en);
    }
    printf("max E_n(x) = %.12e\n\n", max_diff);

    for (int i = 0; i < n_nodes; ++i) {
        fprintf(nodes_file, "%.20f %.20f\n", nodes[i], values[i]);
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
    free(matrix);
    free(rhs);
    free(coeff);
    free(coeff_b);
    free(coeff_c);
    free(coeff_d);
    free(cmp_points);
    return status;
}

int task10_run_for_set(int set_number, int n_nodes, double a, double b, unsigned int seed) {
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
    printf("Interpolation: global polynomial vs natural cubic spline\n");
    printf("Interval [%.6f, %.6f], nodes=%d, seed=%u\n\n", a, b, n_nodes, seed);

    const size_t n_cases = sizeof(NODE_CONFIGS) / sizeof(NODE_CONFIGS[0]);
    for (size_t i = 0; i < n_cases; ++i) {
        if (run_case(test, &NODE_CONFIGS[i], n_nodes, a, b, seed) != 0) {
            return 1;
        }
    }

    return 0;
}

int task10_run(int n_nodes, double a, double b, unsigned int seed) {
    return task10_run_for_set(1, n_nodes, a, b, seed);
}
