#include "utils_task11.h"

void rational_eval(const double *xs, const double *ys,
                   const double *dys, const double *rs,
                   int n, double x,
                   double *out_s, double *out_ds) {
    int    i, lo = 0, hi = n - 2, mid;
    double h, t, tm, r, A, B, C, D, P, Q, Pp, Qp;

    if      (x <= xs[0])   i = 0;
    else if (x >= xs[n-1]) i = n - 2;
    else {
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if      (x < xs[mid])   hi = mid - 1;
            else if (x > xs[mid+1]) lo = mid + 1;
            else { lo = mid; break; }
        }
        i = lo;
    }

    h  = xs[i+1] - xs[i];
    t  = (x - xs[i]) / h;
    tm = 1.0 - t;
    r  = rs[i];
    A  = ys[i+1];
    B  = r*ys[i+1] - h*dys[i+1];
    C  = r*ys[i]   + h*dys[i];
    D  = ys[i];
    P  = A*t*t*t + B*t*t*tm + C*t*tm*tm + D*tm*tm*tm;
    Q  = 1.0 + (r - 3.0)*t*tm;
    Pp = 3.0*A*t*t + B*t*(2.0 - 3.0*t) + C*tm*(1.0 - 3.0*t) - 3.0*D*tm*tm;
    Qp = (r - 3.0)*(1.0 - 2.0*t);

    *out_s  = P / Q;
    *out_ds = (Pp*Q - P*Qp) / (Q*Q) / h;
}
