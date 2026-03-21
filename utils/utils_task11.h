#ifndef UTILS_TASK11_H
#define UTILS_TASK11_H

/* Evaluate rational Hermite spline at a single point x.
 * xs, ys, dys, rs — node arrays of length n_nodes.
 * Results written to *out_s (value) and *out_ds (derivative). */
void rational_eval(const double *xs, const double *ys,
                   const double *dys, const double *rs,
                   int n_nodes, double x,
                   double *out_s, double *out_ds);

#endif
