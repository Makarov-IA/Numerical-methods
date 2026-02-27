import argparse
import os

try:
    import matplotlib.pyplot as plt
except ModuleNotFoundError as exc:
    raise SystemExit(
        "matplotlib is required for plotting (pip install matplotlib)."
    ) from exc


def read_table(path):
    xs, exact, global_poly, hermite, diff = [], [], [], [], []
    with open(path, "r", encoding="utf-8") as file:
        for line in file:
            parts = line.strip().split()
            if not parts:
                continue
            xs.append(float(parts[0]))
            exact.append(float(parts[1]))
            global_poly.append(float(parts[2]))
            hermite.append(float(parts[3]))
            diff.append(float(parts[4]))
    return xs, exact, global_poly, hermite, diff


def read_nodes(path):
    xs, ys = [], []
    with open(path, "r", encoding="utf-8") as file:
        for line in file:
            parts = line.strip().split()
            if not parts:
                continue
            xs.append(float(parts[0]))
            ys.append(float(parts[1]))
    return xs, ys


def main():
    parser = argparse.ArgumentParser(description="Plot task_9 interpolation results.")
    parser.add_argument(
        "--nodes",
        choices=["uniform", "chebyshev", "random"],
        required=True,
        help="nodes type",
    )
    parser.add_argument("--data-dir", default="data_plot", help="directory with data")
    parser.add_argument("--save", default="", help="save figure to PNG path")
    parser.add_argument("--show", action="store_true", help="show interactive window")
    args = parser.parse_args()

    table_path = os.path.join(args.data_dir, f"{args.nodes}.txt")
    nodes_path = os.path.join(args.data_dir, f"{args.nodes}_nodes.txt")

    xs, exact, global_poly, hermite, diff = read_table(table_path)
    nx, ny = read_nodes(nodes_path)

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), sharex=True)

    ax1.plot(xs, exact, label="f(x)", linewidth=2, marker="o", markersize=3)
    ax1.plot(
        xs,
        global_poly,
        label="P_{n-1}(x)",
        linewidth=1.8,
        linestyle="--",
        marker="s",
        markersize=3,
    )
    ax1.plot(
        xs,
        hermite,
        label="H(x)",
        linewidth=1.8,
        linestyle=":",
        marker="^",
        markersize=3,
    )
    ax1.scatter(nx, ny, label="nodes", s=40, zorder=3)
    ax1.set_ylabel("value")
    ax1.set_title(f"Task 9 interpolation ({args.nodes} nodes)")
    ax1.grid(True)
    ax1.set_ylim(
        min(exact) - (max(exact) - min(exact)) / 10,
        max(exact) + (max(exact) - min(exact)) / 10,
    )
    ax1.legend()

    ax2.plot(
        xs,
        diff,
        label="E_n(x)=|P-H|",
        color="tab:red",
        linewidth=2,
        marker="o",
        markersize=3,
    )
    ax2.set_xlabel("x")
    ax2.set_ylabel("E_n(x)")
    ax2.grid(True)
    ax2.legend()

    fig.tight_layout()

    if args.save:
        os.makedirs(os.path.dirname(args.save) or ".", exist_ok=True)
        fig.savefig(args.save, dpi=150)

    if args.show or not args.save:
        plt.show()


if __name__ == "__main__":
    main()
