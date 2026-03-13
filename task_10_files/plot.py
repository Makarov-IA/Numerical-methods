import argparse

try:
    import matplotlib.pyplot as plt
except ModuleNotFoundError as exc:
    raise SystemExit(
        "matplotlib is required for plotting (pip install matplotlib)."
    ) from exc


def read_pairs(path):
    xs, ys = [], []
    with open(path, "r", encoding="utf-8") as file:
        for line in file:
            parts = line.strip().split()
            if not parts:
                continue
            xs.append(float(parts[0]))
            ys.append(float(parts[1]))
    return xs, ys


def get_limits(values):
    left = min(values)
    right = max(values)
    span = right - left

    if span == 0.0:
        pad = max(1.0, 0.1 * abs(left))
    else:
        pad = 0.1 * span

    return left - pad, right + pad


def main():
    parser = argparse.ArgumentParser(description="Plot weighted spline from data_plot files.")
    parser.add_argument("--curve", default="data_plot/curve.txt", help="curve file")
    parser.add_argument("--nodes", default="data_plot/nodes.txt", help="nodes file")
    parser.add_argument("--save", default="", help="save figure to PNG path")
    parser.add_argument("--show", action="store_true", help="show interactive window")
    args = parser.parse_args()

    curve_x, curve_y = read_pairs(args.curve)
    node_x, node_y = read_pairs(args.nodes)

    fig, ax = plt.subplots(figsize=(10, 6))
    ax.plot(curve_x, curve_y, label="weighted spline", linewidth=2)
    ax.scatter(node_x, node_y, label="input points", color="tab:red", s=40, zorder=3)
    ax.set_xlim(*get_limits(node_x))
    ax.set_ylim(*get_limits(node_y))
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_title("Weighted Spline")
    ax.grid(True)
    ax.legend()

    fig.tight_layout()

    if args.save:
        fig.savefig(args.save, dpi=150)

    if args.show or not args.save:
        plt.show()


if __name__ == "__main__":
    main()
