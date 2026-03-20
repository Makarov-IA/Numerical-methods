#include <stdio.h>
#include <stdlib.h>
#include "utils_task11.h"

#define CURVE_POINTS 500

/* ─── input data ──────────────────────────────────────────────────────────── */

typedef struct {
    double *x;
    double *y;
    double *dy;
    double *r;   /* per-node rational parameter; r[i] used for segment i */
    int     n;
} InputData11;

static void init_data(InputData11 *d) {
    d->x = d->y = d->dy = d->r = NULL;
    d->n = 0;
}

static void free_data(InputData11 *d) {
    free(d->x); free(d->y); free(d->dy); free(d->r);
    init_data(d);
}

static int add_row(InputData11 *d, double x, double y, double dy, double r) {
    int n = d->n + 1;
    d->x  = (double*)realloc(d->x,  (size_t)n * sizeof(double));
    d->y  = (double*)realloc(d->y,  (size_t)n * sizeof(double));
    d->dy = (double*)realloc(d->dy, (size_t)n * sizeof(double));
    d->r  = (double*)realloc(d->r,  (size_t)n * sizeof(double));
    if (!d->x || !d->y || !d->dy || !d->r) return -1;
    d->x [d->n] = x;
    d->y [d->n] = y;
    d->dy[d->n] = dy;
    d->r [d->n] = r;
    d->n = n;
    return 0;
}

static int read_input(const char *path, InputData11 *d) {
    FILE  *f = fopen(path, "r");
    char   line[256];
    char  *ptr;
    double x, y, dy, r;

    if (!f) { fprintf(stderr, "Cannot open %s\n", path); return -1; }
    init_data(d);
    while (fgets(line, sizeof(line), f)) {
        ptr = line;
        while (*ptr == ' ' || *ptr == '\t') ++ptr;
        if (*ptr == '#' || *ptr == '\0' || *ptr == '\n' || *ptr == '\r') continue;
        if (sscanf(ptr, "%lf %lf %lf %lf", &x, &y, &dy, &r) == 4)
            add_row(d, x, y, dy, r);
    }
    fclose(f);
    if (d->n < 2) { fprintf(stderr, "Need at least 2 nodes in %s\n", path); return -1; }
    return 0;
}

/* ─── rational Hermite spline ─────────────────────────────────────────────
 *
 * On segment i, with t = (x - x_i) / h_i, tm = 1 - t:
 *
 *   P_i(t) = f_{i+1}*t³ + (r*f_{i+1} - h*d_{i+1})*t²*tm
 *                        + (r*f_i    + h*d_i   )*t*tm²
 *                        + f_i*tm³
 *
 *   Q_i(t) = 1 + (r - 3)*t*tm
 *
 *   S_i(x) = P_i(t) / Q_i(t)
 *
 * r=3  →  Q=1  →  reduces to classical cubic Hermite spline.
 * r > -1 required for Q > 0.
 * ─────────────────────────────────────────────────────────────────────── */

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

static double eval_s(const InputData11 *d, double x) {
    int    i   = find_interval(d->x, d->n, x);
    double h   = d->x[i+1] - d->x[i];
    double t   = (x - d->x[i]) / h;
    double tm  = 1.0 - t;
    double r   = d->r[i];
    double fi  = d->y[i],  fi1 = d->y[i+1];
    double di  = d->dy[i], di1 = d->dy[i+1];
    double A   = fi1;
    double B   = r*fi1 - h*di1;
    double C   = r*fi  + h*di;
    double D   = fi;
    double P   = A*t*t*t + B*t*t*tm + C*t*tm*tm + D*tm*tm*tm;
    double Q   = 1.0 + (r - 3.0)*t*tm;
    return P / Q;
}

static double eval_ds(const InputData11 *d, double x) {
    int    i   = find_interval(d->x, d->n, x);
    double h   = d->x[i+1] - d->x[i];
    double t   = (x - d->x[i]) / h;
    double tm  = 1.0 - t;
    double r   = d->r[i];
    double fi  = d->y[i],  fi1 = d->y[i+1];
    double di  = d->dy[i], di1 = d->dy[i+1];
    double A   = fi1;
    double B   = r*fi1 - h*di1;
    double C   = r*fi  + h*di;
    double D   = fi;
    double P   = A*t*t*t + B*t*t*tm + C*t*tm*tm + D*tm*tm*tm;
    double Q   = 1.0 + (r - 3.0)*t*tm;
    double Pp  = 3.0*A*t*t + B*t*(2.0 - 3.0*t) + C*tm*(1.0 - 3.0*t) - 3.0*D*tm*tm;
    double Qp  = (r - 3.0)*(1.0 - 2.0*t);
    return (Pp*Q - P*Qp) / (Q*Q) / h;
}

int task11_run(const char *input_path) {
    InputData11  d;
    FILE        *fcurve, *fnodes, *fweights, *fmeta;
    int          i;
    double       x;

    if (read_input(input_path, &d) != 0) return 1;

    printf("Loaded %d nodes from %s\n\n", d.n, input_path);
    printf("%-16s %-18s %-18s %-8s\n", "x", "f(x)", "f'(x)", "r");
    for (i = 0; i < d.n; ++i)
        printf("%-16.8f %-18.10e %-18.10e %-8.4f\n",
               d.x[i], d.y[i], d.dy[i], d.r[i]);
    printf("\n");

    fcurve   = fopen("data_plot/curve.txt",   "w");
    fnodes   = fopen("data_plot/nodes.txt",   "w");
    fweights = fopen("data_plot/weights.txt", "w");
    fmeta    = fopen("data_plot/meta.txt",    "w");

    /* curve: x  S(x)  S'(x) */
    for (i = 0; i < CURVE_POINTS; ++i) {
        x = d.x[0] + (d.x[d.n-1] - d.x[0]) * (double)i / (double)(CURVE_POINTS - 1);
        fprintf(fcurve, "%.16f %.16f %.16f\n", x, eval_s(&d, x), eval_ds(&d, x));
    }

    for (i = 0; i < d.n; ++i)
        fprintf(fnodes,   "%.16f %.16f\n", d.x[i], d.y[i]);

    /* weights: x  r  (r[n-1] unused in spline but kept for plot annotations) */
    for (i = 0; i < d.n; ++i)
        fprintf(fweights, "%.16f %.16f\n", d.x[i], d.r[i]);

    fprintf(fmeta, "n_nodes=%d\n", d.n);
    fprintf(fmeta, "a=%.8f\n",     d.x[0]);
    fprintf(fmeta, "b=%.8f\n",     d.x[d.n-1]);
    fprintf(fmeta, "input=%s\n",   input_path);

    fclose(fcurve); fclose(fnodes); fclose(fweights); fclose(fmeta);
    printf("Saved data_plot/curve.txt, nodes.txt, weights.txt, meta.txt\n");

    free_data(&d);
    return 0;
}
