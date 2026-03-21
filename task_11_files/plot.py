import argparse
import platform
import ctypes

import matplotlib.pyplot as plt
import numpy as np

# ─── C library ───────────────────────────────────────────────────────────────

LIB_PATH = "./libtask11.dylib" if platform.system() == "Darwin" else "./libtask11.so"
dp = ctypes.POINTER(ctypes.c_double)


def load_lib():
    lib = ctypes.CDLL(LIB_PATH)
    lib.rational_eval.restype  = None
    lib.rational_eval.argtypes = [dp, dp, dp, dp, ctypes.c_int,
                                   ctypes.c_double, dp, dp]
    return lib


def np2c(arr):
    return arr.ctypes.data_as(dp)


def eval_spline_c(lib, xs, ys, dys, rs, x_grid):
    n   = len(xs)
    s   = ctypes.c_double()
    ds  = ctypes.c_double()
    cxs, cys, cdys, crs = np2c(xs), np2c(ys), np2c(dys), np2c(rs)
    out_s  = np.empty(len(x_grid))
    out_ds = np.empty(len(x_grid))
    for k, x in enumerate(x_grid):
        lib.rational_eval(cxs, cys, cdys, crs, n, x,
                          ctypes.byref(s), ctypes.byref(ds))
        out_s[k]  = s.value
        out_ds[k] = ds.value
    return out_s, out_ds

# ─── file reading ─────────────────────────────────────────────────────────────

def read_file(path):
    xs, ys, dys, rs = [], [], [], []
    with open(path) as fh:
        for line in fh:
            s = line.strip()
            if not s or s.startswith("#"):
                continue
            parts = s.split()
            if len(parts) == 4:
                xs.append(float(parts[0]))
                ys.append(float(parts[1]))
                dys.append(float(parts[2]))
                rs.append(float(parts[3]))
    return (np.array(xs), np.array(ys), np.array(dys), np.array(rs))

# ─── main ─────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(description="Plot rational Hermite spline (task 11).")
    parser.add_argument("input", nargs="?", default="examples/set_1.txt")
    parser.add_argument("--save", default="")
    args = parser.parse_args()

    lib = load_lib()
    xs, ys, dys, rs = read_file(args.input)
    n = len(xs)

    x_grid = np.linspace(xs[0], xs[-1], 500)
    sx, dsx = eval_spline_c(lib, xs, ys, dys, rs, x_grid)

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(11, 8), sharex=True)
    fig.subplots_adjust(hspace=0.06)

    # ── top: spline ───────────────────────────────────────────────────────────
    ax1.plot(x_grid, sx, color="tab:red", lw=1.8, label="S(x)")
    ax1.scatter(xs, ys, color="black", s=30, zorder=5, label=f"nodes (n={n})")

    # r annotations above each node (last node has no segment — skip it)
    for i in range(n - 1):
        ax1.annotate(
            f"[{i}] {rs[i]:.2f}",
            xy=(xs[i], ys[i]),
            xytext=(0, 8),
            textcoords="offset points",
            ha="center",
            va="bottom",
            fontsize=7.5,
            color="tab:blue",
        )

    ax1.set_ylabel("S(x)", fontsize=12)
    ax1.set_title(f"Rational Hermite spline  (n={n})\n{args.input}", fontsize=11)
    ax1.legend(fontsize=9, loc="best")
    ax1.grid(True, alpha=0.35)

    # ── bottom: derivative ────────────────────────────────────────────────────
    ax2.plot(x_grid, dsx, color="tab:red", lw=1.8, label="S'(x)")
    ax2.scatter(xs, np.interp(xs, x_grid, dsx), color="black", s=20, zorder=5)
    ax2.set_xlabel("x", fontsize=12)
    ax2.set_ylabel("S'(x)", fontsize=12)
    ax2.legend(fontsize=9, loc="best")
    ax2.grid(True, alpha=0.35)

    fig.tight_layout()

    if args.save:
        fig.savefig(args.save, dpi=150, bbox_inches="tight")
    else:
        plt.show()

    plt.close(fig)


if __name__ == "__main__":
    main()
