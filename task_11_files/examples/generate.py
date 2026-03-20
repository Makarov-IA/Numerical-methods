"""
Генератор входных файлов для task_11 (рациональный сплайн Эрмита).

Задай функцию, производную, отрезок, число узлов и параметр r ниже,
затем запусти:
    python3 generate.py

Файл будет сохранён в examples/custom.txt (или задай OUTPUT_FILE).
"""

import math
import os

# ── настройки ────────────────────────────────────────────────────────────────

def f(x):
    return math.sin(x) * math.exp(-0.3 * x)

def df(x):
    return (math.cos(x) - 0.3 * math.sin(x)) * math.exp(-0.3 * x)

FUNC_NAME  = "sin(x)*exp(-0.3*x)"
A          = 0.0      # левый конец отрезка
B          = 10.0     # правый конец отрезка
N          = 15       # число узлов
R          = 3.0      # параметр r (одинаковый для всех сегментов)
            # r=3 -> кубический Эрмита; r>3 -> сплайн "прижимается"
            # можно задать список длиной N для разных r на каждом узле:
            # R = [3.0, 4.0, 3.0, ...]

OUTPUT_FILE = os.path.join(os.path.dirname(__file__), "custom.txt")

# ─────────────────────────────────────────────────────────────────────────────

def main():
    xs  = [A + (B - A) * i / (N - 1) for i in range(N)]
    ys  = [f(x)  for x in xs]
    dys = [df(x) for x in xs]

    if isinstance(R, (int, float)):
        rs = [float(R)] * N
    else:
        if len(R) != N:
            raise ValueError(f"R must be a scalar or list of length N={N}, got {len(R)}")
        rs = [float(v) for v in R]

    with open(OUTPUT_FILE, "w") as fh:
        fh.write(f"#N = {N-1}  // {N} nodes, {N-1} segments\n")
        fh.write(f"#func = {FUNC_NAME}\n")
        fh.write(f"#a = {A}  b = {B}\n")
        fh.write(f"# r > -1 required; r=3 -> classical cubic Hermite\n")
        fh.write(f"#{'x':>16}  {'f':>18}  {'df':>18}  {'r':>6}\n")
        for i in range(N):
            fh.write(f"  {xs[i]:>18.10f}  {ys[i]:>18.10e}  {dys[i]:>18.10e}  {rs[i]:>6.2f}\n")

    print(f"Written {OUTPUT_FILE}  ({N} nodes, r={R})")


if __name__ == "__main__":
    main()
