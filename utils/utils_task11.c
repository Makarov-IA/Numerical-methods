#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils_task11.h"

#define CURVE_POINTS 500

/* ─── input data ──────────────────────────────────────────────────────────── */

typedef struct {
    double *x;
    double *y;
    double *dy;
    double *sigma;
    int     n;
} InputData11;

static void init_data(InputData11 *d) {
    d->x = d->y = d->dy = d->sigma = NULL;
    d->n = 0;
}

static void free_data(InputData11 *d) {
    free(d->x); free(d->y); free(d->dy); free(d->sigma);
    init_data(d);
}

static int add_row(InputData11 *d, double x, double y, double dy, double sigma) {
    int n = d->n + 1;
    d->x     = (double*)realloc(d->x,     (size_t)n * sizeof(double));
    d->y     = (double*)realloc(d->y,     (size_t)n * sizeof(double));
    d->dy    = (double*)realloc(d->dy,    (size_t)n * sizeof(double));
    d->sigma = (double*)realloc(d->sigma, (size_t)n * sizeof(double));
    if (!d->x || !d->y || !d->dy || !d->sigma) return -1;
    d->x    [d->n] = x;
    d->y    [d->n] = y;
    d->dy   [d->n] = dy;
    d->sigma[d->n] = sigma;
    d->n = n;
    return 0;
}

static int read_input(const char *path, InputData11 *d) {
    FILE   *f = fopen(path, "r");
    char    line[256];
    char   *ptr;
    double  x, y, dy, sigma;

    if (!f) { fprintf(stderr, "Cannot open %s\n", path); return -1; }
    init_data(d);
    while (fgets(line, sizeof(line), f)) {
        ptr = line;
        while (*ptr == ' ' || *ptr == '\t') ++ptr;
        if (*ptr == '#' || *ptr == '\0' || *ptr == '\n' || *ptr == '\r') continue;
        if (sscanf(ptr, "%lf %lf %lf %lf", &x, &y, &dy, &sigma) == 4)
            add_row(d, x, y, dy, sigma);
    }
    fclose(f);
    if (d->n < 2) { fprintf(stderr, "Need at least 2 nodes in %s\n", path); return -1; }
    return 0;
}

/* ─── spline primitives ───────────────────────────────────────────────────── */

static int find_interval(const double *nd, int n, double x) {
    int lo = 0, hi = n-2, mid;
    if (x <= nd[0])   return 0;
    if (x >= nd[n-1]) return n-2;
    while (lo <= hi) {
        mid = lo + (hi-lo)/2;
        if      (x < nd[mid])   hi = mid-1;
        else if (x > nd[mid+1]) lo = mid+1;
        else return mid;
    }
    return n-2;
}

/*
 * Cubic Hermite with per-node sigma:
 *   ml = sigma[i]*dy[i],  mr = sigma[i+1]*dy[i+1]
 *   S_i(x) = ca*dx^3 + cb*dx^2 + ml*dx + y[i]
 */
static void build_coeffs(const InputData11 *d, double *ca, double *cb) {
    int i;
    for (i = 0; i < d->n-1; ++i) {
        double h  = d->x[i+1] - d->x[i];
        double ml = d->sigma[i]   * d->dy[i];
        double mr = d->sigma[i+1] * d->dy[i+1];
        ca[i] = (ml+mr)/(h*h) - 2.0*(d->y[i+1]-d->y[i])/(h*h*h);
        cb[i] = 3.0*(d->y[i+1]-d->y[i])/(h*h) - (2.0*ml+mr)/h;
    }
}

static double eval_s(const InputData11 *d,
                     const double *ca, const double *cb, double x) {
    int    i  = find_interval(d->x, d->n, x);
    double dx = x - d->x[i];
    return ca[i]*dx*dx*dx + cb[i]*dx*dx + d->sigma[i]*d->dy[i]*dx + d->y[i];
}

static double eval_ds(const InputData11 *d,
                      const double *ca, const double *cb, double x) {
    int    i  = find_interval(d->x, d->n, x);
    double dx = x - d->x[i];
    return 3.0*ca[i]*dx*dx + 2.0*cb[i]*dx + d->sigma[i]*d->dy[i];
}

/* ─── main entry point ────────────────────────────────────────────────────── */

int task11_run(const char *input_path) {
    InputData11  d;
    double      *ca, *cb;
    FILE        *fcurve, *fnodes, *fweights, *fmeta;
    int          i, n_seg;

    if (read_input(input_path, &d) != 0) return 1;
    n_seg = d.n - 1;

    ca = (double*)malloc((size_t)n_seg * sizeof(double));
    cb = (double*)malloc((size_t)n_seg * sizeof(double));
    if (!ca || !cb) {
        free(ca); free(cb); free_data(&d); return 1;
    }

    build_coeffs(&d, ca, cb);

    printf("Loaded %d nodes from %s\n\n", d.n, input_path);
    printf("%-16s %-18s %-18s %-10s\n", "x", "f(x)", "f'(x)", "sigma");
    for (i = 0; i < d.n; ++i)
        printf("%-16.8f %-18.10e %-18.10e %-10.4f\n",
               d.x[i], d.y[i], d.dy[i], d.sigma[i]);
    printf("\n");

    fcurve   = fopen("data_plot/curve.txt",   "w");
    fnodes   = fopen("data_plot/nodes.txt",   "w");
    fweights = fopen("data_plot/weights.txt", "w");
    fmeta    = fopen("data_plot/meta.txt",    "w");

    /* curve: x  S(x)  S'(x) */
    for (i = 0; i < CURVE_POINTS; ++i) {
        double x = d.x[0] + (d.x[d.n-1] - d.x[0])
                   * (double)i / (double)(CURVE_POINTS - 1);
        fprintf(fcurve, "%.16f %.16f %.16f\n",
                x, eval_s(&d, ca, cb, x), eval_ds(&d, ca, cb, x));
    }

    /* nodes: x  y */
    for (i = 0; i < d.n; ++i)
        fprintf(fnodes, "%.16f %.16f\n", d.x[i], d.y[i]);

    /* weights: x  sigma */
    for (i = 0; i < d.n; ++i)
        fprintf(fweights, "%.16f %.16f\n", d.x[i], d.sigma[i]);

    fprintf(fmeta, "n_nodes=%d\n",   d.n);
    fprintf(fmeta, "a=%.8f\n",       d.x[0]);
    fprintf(fmeta, "b=%.8f\n",       d.x[d.n-1]);
    fprintf(fmeta, "input=%s\n",     input_path);

    fclose(fcurve); fclose(fnodes); fclose(fweights); fclose(fmeta);
    printf("Saved data_plot/curve.txt, nodes.txt, weights.txt, meta.txt\n");

    free(ca); free(cb); free_data(&d);
    return 0;
}
