import argparse
import os
import sys

try:
    import matplotlib.pyplot as plt
except Exception as exc:
    print(f"Error: matplotlib is required ({exc})")
    sys.exit(1)


def read_table(path):
    xs, exact, slae, lagrange = [], [], [], []
    diff = []
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            parts = line.split()
            if len(parts) < 4:
                continue
            xs.append(float(parts[0]))
            exact.append(float(parts[1]))
            slae.append(float(parts[2]))
            lagrange.append(float(parts[3]))
            if len(parts) >= 5:
                diff.append(float(parts[4]))
    return xs, exact, slae, lagrange, diff


def read_nodes(path):
    xs, ys = [], []
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            parts = line.split()
            if len(parts) < 2:
                continue
            xs.append(float(parts[0]))
            ys.append(float(parts[1]))
    return xs, ys


def main():
    parser = argparse.ArgumentParser(description="Plot interpolation results.")
    parser.add_argument("--set", type=int, required=True, help="test set number")
    parser.add_argument(
        "--nodes", choices=["uniform", "chebyshev"], required=True, help="nodes type"
    )
    parser.add_argument("--data-dir", default="data_plot", help="directory with data")
    parser.add_argument("--save", default="", help="save figure to PNG path")
    parser.add_argument("--show", action="store_true", help="show interactive window")
    args = parser.parse_args()

    table_path = os.path.join(args.data_dir, f"set_{args.set}_{args.nodes}.txt")
    nodes_path = os.path.join(args.data_dir, f"set_{args.set}_{args.nodes}_nodes.txt")

    if not os.path.exists(table_path):
        print(f"Error: {table_path} not found")
        sys.exit(1)
    if not os.path.exists(nodes_path):
        print(f"Error: {nodes_path} not found")
        sys.exit(1)

    xs, exact, slae, lagrange, _diff = read_table(table_path)
    nx, ny = read_nodes(nodes_path)

    plt.figure(figsize=(10, 6))
    plt.plot(xs, exact, label="Exact", linewidth=2)
    plt.plot(xs, slae, label="SLAE", linewidth=2, linestyle="--")
    plt.plot(xs, lagrange, label="Lagrange", linewidth=2, linestyle=":")
    plt.scatter(nx, ny, label="Nodes", s=24, zorder=3)
    plt.title(f"Interpolation: set {args.set} ({args.nodes} nodes)")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.grid(True)
    plt.legend()

    if args.save:
        os.makedirs(os.path.dirname(args.save) or ".", exist_ok=True)
        plt.savefig(args.save, dpi=150)

    if args.show or not args.save:
        plt.show()


if __name__ == "__main__":
    main()
