#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "utils_matrix.h"
#include "utils_task10.h"

#define EPS 1e-12
#define CURVE_POINTS 500

typedef struct {
    double *x;
    double *y;
    double *w;
    int n;
    int has_left_derivative;
    int has_right_derivative;
    double left_derivative;
    double right_derivative;
} InputData;

void init_input_data(InputData *data) {
    data->x = NULL;
    data->y = NULL;
    data->w = NULL;
    data->n = 0;
    data->has_left_derivative = 0;
    data->has_right_derivative = 0;
    data->left_derivative = 0.0;
    data->right_derivative = 0.0;
}

void free_input_data(InputData *data) {
    free(data->x);
    free(data->y);
    free(data->w);
    init_input_data(data);
}

int add_input_row(InputData *data, double x, double y, double w) {
    double *new_x;
    double *new_y;
    double *new_w;
    int new_n;

    new_n = data->n + 1;

    new_x = (double *)realloc(data->x, (size_t)new_n * sizeof(double));
    data->x = new_x;

    new_y = (double *)realloc(data->y, (size_t)new_n * sizeof(double));
    data->y = new_y;

    new_w = (double *)realloc(data->w, (size_t)new_n * sizeof(double));
    data->w = new_w;

    data->x[data->n] = x;
    data->y[data->n] = y;
    data->w[data->n] = w;
    data->n = new_n;

    return 0;
}

int read_input_file(const char *input_path, InputData *data) {
    FILE *file;
    char line[256];
    char *ptr;
    int expected_n;
    double x;
    double y;
    double w;

    init_input_data(data);
    expected_n = -1;

    file = fopen(input_path, "r");

    while (fgets(line, sizeof(line), file)) {
        ptr = line;
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r') {
            ++ptr;
        }

        if (*ptr == '\0') {
            continue;
        }

        if (*ptr == '#') {
            if (sscanf(ptr, "#N = %d", &expected_n) == 1) {
                continue;
            }
            if (sscanf(ptr, "#f'_0 = %lf", &data->left_derivative) == 1) {
                data->has_left_derivative = 1;
                continue;
            }
            if (sscanf(ptr, "#f'_N = %lf", &data->right_derivative) == 1) {
                data->has_right_derivative = 1;
                continue;
            }
            continue;
        }

        sscanf(ptr, "%lf %lf %lf", &x, &y, &w);
        add_input_row(data, x, y, w);
    }

    fclose(file);
    return 0;
}

int build_weighted_spline(const InputData *data, double *b, double *coef_c, double *d) {
    double *lower;
    double *diag;
    double *upper;
    double *rhs;
    double *h;
    double *delta;
    double *segment_w;
    int n;
    int i;
    int status;

    n = data->n;
    status = 1;

    lower = (double *)malloc((size_t)(n - 1) * sizeof(double));
    diag = (double *)malloc((size_t)n * sizeof(double));
    upper = (double *)malloc((size_t)(n - 1) * sizeof(double));
    rhs = (double *)malloc((size_t)n * sizeof(double));
    h = (double *)malloc((size_t)(n - 1) * sizeof(double));
    delta = (double *)malloc((size_t)(n - 1) * sizeof(double));
    segment_w = (double *)malloc((size_t)(n - 1) * sizeof(double));

    for (i = 0; i < n - 1; ++i) {
        lower[i] = 0.0;
        upper[i] = 0.0;
        b[i] = 0.0;
        d[i] = 0.0;
    }

    for (i = 0; i < n; ++i) {
        diag[i] = 0.0;
        rhs[i] = 0.0;
        coef_c[i] = 0.0;
    }

    for (i = 0; i < n - 1; ++i) {
        h[i] = data->x[i + 1] - data->x[i];
        delta[i] = (data->y[i + 1] - data->y[i]) / h[i];
        segment_w[i] = data->w[i];
    }

    if (n == 2) {
        b[0] = delta[0];
        free(lower);
        free(diag);
        free(upper);
        free(rhs);
        free(h);
        free(delta);
        free(segment_w);
        return 0;
    }

    if (data->has_left_derivative) {
        diag[0] = 2.0 * segment_w[0] * h[0];
        upper[0] = segment_w[0] * h[0];
        rhs[0] = 3.0 * (segment_w[0] * delta[0] - data->left_derivative);
    } else {
        diag[0] = 1.0;
        rhs[0] = 0.0;
    }

    for (i = 1; i < n - 1; ++i) {
        lower[i - 1] = segment_w[i - 1] * h[i - 1];
        diag[i] = 2.0 * (segment_w[i - 1] * h[i - 1] + segment_w[i] * h[i]);
        upper[i] = segment_w[i] * h[i];
        rhs[i] = 3.0 * (segment_w[i] * delta[i] - segment_w[i - 1] * delta[i - 1]);
    }

    if (data->has_right_derivative) {
        lower[n - 2] = segment_w[n - 2] * h[n - 2];
        diag[n - 1] = 2.0 * segment_w[n - 2] * h[n - 2];
        rhs[n - 1] = 3.0 * (data->right_derivative - segment_w[n - 2] * delta[n - 2]);
    } else {
        diag[n - 1] = 1.0;
        rhs[n - 1] = 0.0;
    }

    progonka(lower, diag, upper, n, rhs, coef_c);
    for (i = 0; i < n - 1; ++i) {
        b[i] = delta[i] - h[i] * (2.0 * coef_c[i] + coef_c[i + 1]) / 3.0;
        d[i] = (coef_c[i + 1] - coef_c[i]) / (3.0 * h[i]);
    }

    free(lower);
    free(diag);
    free(upper);
    free(rhs);
    free(h);
    free(delta);
    free(segment_w);
    status = 0;
    return status;
}

double eval_spline(const InputData *data, const double *b, const double *c, const double *d, double x) {
    int i;
    double dx;

    i = 0;
    while (i < data->n - 2 && x > data->x[i + 1]) {
        ++i;
    }

    dx = x - data->x[i];
    return data->y[i] + b[i] * dx + c[i] * dx * dx + d[i] * dx * dx * dx;
}

int write_plot_data(const InputData *data, const double *b, const double *c, const double *d) {
    FILE *curve_file;
    FILE *nodes_file;
    int i;
    double x0;
    double x1;
    double x;
    double y;


    curve_file = fopen("data_plot/curve.txt", "w");
    nodes_file = fopen("data_plot/nodes.txt", "w");

    for (i = 0; i < data->n; ++i) {
        fprintf(nodes_file, "%.20f %.20f\n", data->x[i], data->y[i]);
    }

    x0 = data->x[0];
    x1 = data->x[data->n - 1];
    for (i = 0; i < CURVE_POINTS; ++i) {
        x = x0 + (x1 - x0) * (double)i / (double)(CURVE_POINTS - 1);
        y = eval_spline(data, b, c, d, x);
        fprintf(curve_file, "%.20f %.20f\n", x, y);
    }

    fclose(curve_file);
    fclose(nodes_file);
    return 0;
}

int task10_run_from_file(const char *input_path) {
    InputData data;
    double *b;
    double *c;
    double *d;
    int status;

    init_input_data(&data);
    status = 1;

    if (read_input_file(input_path, &data) != 0) {
        return 1;
    }

    b = (double *)malloc((size_t)(data.n - 1) * sizeof(double));
    c = (double *)malloc((size_t)data.n * sizeof(double));
    d = (double *)malloc((size_t)(data.n - 1) * sizeof(double));

    build_weighted_spline(&data, b, c, d);
    write_plot_data(&data, b, c, d);

    printf("Read %d points from %s\n", data.n, input_path);
    printf("Saved data_plot/nodes.txt and data_plot/curve.txt\n");
    status = 0;
    free(b);
    free(c);
    free(d);
    free_input_data(&data);
    return status;
}
