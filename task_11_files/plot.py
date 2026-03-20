import argparse

import matplotlib.pyplot as plt
import numpy as np


def read_meta(path):
    meta = {}
    with open(path, "r", encoding="utf-8") as fh:
        for line in fh:
            line = line.strip()
            if "=" in line:
                key, val = line.split("=", 1)
                meta[key.strip()] = val.strip()
    return meta


def main():
    parser = argparse.ArgumentParser(
        description="Plot Hermite tension spline (task 11)."
    )
    parser.add_argument("--curve", default="data_plot/curve.txt")
    parser.add_argument("--nodes", default="data_plot/nodes.txt")
    parser.add_argument("--meta", default="data_plot/meta.txt")
    parser.add_argument("--weights", default="data_plot/weights.txt")
    parser.add_argument("--save", default="")
    parser.add_argument("--show", action="store_true")
    args = parser.parse_args()

    meta = read_meta(args.meta)
    n_nodes = meta.get("n_nodes", "?")
    src = meta.get("input", "")

    curve = np.loadtxt(args.curve)  # x  S(x)  S'(x)
    nodes = np.loadtxt(args.nodes)  # x  y
    weights = np.loadtxt(args.weights)  # x  sigma

    x = curve[:, 0]
    sx = curve[:, 1]
    dsx = curve[:, 2]

    nx = nodes[:, 0]
    ny = nodes[:, 1]
    wx = weights[:, 0]
    sigma = weights[:, 1]

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(11, 8), sharex=True)
    fig.subplots_adjust(hspace=0.06)

    # ── top: spline ───────────────────────────────────────────────────────────
    ax1.plot(x, sx, color="tab:red", lw=1.8, label="S(x)")
    ax1.scatter(nx, ny, color="black", s=30, zorder=5, label=f"nodes (n={n_nodes})")

    # sigma annotations above each node
    for i in range(len(wx)):
        ax1.annotate(
            f"{sigma[i]:.2f}",
            xy=(wx[i], ny[i]),
            xytext=(0, 8),
            textcoords="offset points",
            ha="center",
            va="bottom",
            fontsize=7.5,
            color="tab:blue",
        )

    ax1.set_ylabel("S(x)", fontsize=12)
    title = f"Hermite spline with tension  (n={n_nodes})"
    if src:
        title += f"\n{src}"
    ax1.set_title(title, fontsize=11)
    ax1.legend(fontsize=9, loc="best")
    ax1.grid(True, alpha=0.35)

    # ── bottom: derivative ────────────────────────────────────────────────────
    ax2.plot(x, dsx, color="tab:red", lw=1.8, label="S'(x)")

    # scatter node derivatives: sigma[i]*dy[i] — these ARE the spline slope at nodes
    # we don't have dy separately, but eval_ds at node == sigma*dy, already in dsx grid
    # just draw vertical ticks at node positions
    node_dsx = np.interp(nx, x, dsx)
    ax2.scatter(nx, node_dsx, color="black", s=20, zorder=5)

    ax2.set_xlabel("x", fontsize=12)
    ax2.set_ylabel("S'(x)", fontsize=12)
    ax2.legend(fontsize=9, loc="best")
    ax2.grid(True, alpha=0.35)

    fig.tight_layout()

    if args.save:
        fig.savefig(args.save, dpi=150, bbox_inches="tight")
    if args.show or not args.save:
        plt.show()

    plt.close(fig)


if __name__ == "__main__":
    main()
