"""
Интерактивный редактор параметров r для рационального сплайна Эрмита.
Вычисление сплайна делегируется libtask11.dylib (C-код).

Запуск: python3 plot_interactive.py examples/set_1.txt
"""

import sys
import ctypes
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.widgets as mw

# ─── загрузка C-библиотеки ────────────────────────────────────────────────────

import platform
LIB_PATH = "./libtask11.dylib" if platform.system() == "Darwin" else "./libtask11.so"

dp = ctypes.POINTER(ctypes.c_double)

def load_lib():
    lib = ctypes.CDLL(LIB_PATH)
    lib.rational_eval.restype  = None
    lib.rational_eval.argtypes = [
        dp, dp, dp, dp,       # xs, ys, dys, rs
        ctypes.c_int,         # n_nodes
        ctypes.c_double,      # x
        dp, dp,               # out_s, out_ds
    ]
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

# ─── чтение/запись файла ──────────────────────────────────────────────────────

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
    return (np.array(xs, dtype=float), np.array(ys, dtype=float),
            np.array(dys, dtype=float), np.array(rs, dtype=float))


def write_file(path, xs, ys, dys, rs):
    header = []
    with open(path) as fh:
        for line in fh:
            if line.strip() and not line.strip().startswith("#"):
                break
            header.append(line)
    with open(path, "w") as fh:
        fh.writelines(header)
        fh.write(f"#{'x':>16}  {'f':>18}  {'df':>18}  {'r':>6}\n")
        for i in range(len(xs)):
            fh.write(
                f"  {xs[i]:>18.10f}  {ys[i]:>18.10e}"
                f"  {dys[i]:>18.10e}  {rs[i]:>6.2f}\n"
            )
    print(f"Saved {path}")

# ─── главная часть ────────────────────────────────────────────────────────────

def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "examples/set_1.txt"
    lib  = load_lib()

    xs, ys, dys, rs = read_file(path)
    n     = len(xs)
    n_seg = n - 1
    r_cur = rs[:n_seg].copy()   # только n-1 значений (последнее не нужно)

    x_grid = np.linspace(xs[0], xs[-1], 500)

    # ── компоновка ────────────────────────────────────────────────────────────
    slider_rows   = (n_seg + 1) // 2
    slider_h      = 0.028
    slider_gap    = 0.008
    sliders_total = slider_rows * (slider_h + slider_gap) + 0.05 + slider_h + 0.02

    fig = plt.figure(figsize=(13, 8))
    fig.subplots_adjust(left=0.07, right=0.97,
                        top=0.93,  bottom=sliders_total + 0.04, hspace=0.08)

    ax_s  = fig.add_subplot(2, 1, 1)
    ax_ds = fig.add_subplot(2, 1, 2, sharex=ax_s)

    rs_full      = np.append(r_cur, r_cur[-1])
    sy, dsy      = eval_spline_c(lib, xs, ys, dys, rs_full, x_grid)
    line_s,      = ax_s.plot( x_grid, sy,  color="tab:red", lw=1.8, label="S(x)")
    line_ds,     = ax_ds.plot(x_grid, dsy, color="tab:red", lw=1.8, label="S'(x)")
    ax_s.scatter( xs, ys,                         color="black", s=28, zorder=5)
    node_sc_ds   = ax_ds.scatter(xs, np.interp(xs, x_grid, dsy),
                                 color="black", s=18, zorder=5)

    node_labels = []
    for i in range(n):
        ann = ax_s.annotate(str(i), xy=(xs[i], ys[i]),
                            xytext=(0, 7), textcoords="offset points",
                            ha="center", va="bottom", fontsize=7.5, color="black")
        node_labels.append(ann)

    for ax in (ax_s, ax_ds):
        ax.grid(True, alpha=0.3)
    ax_s.set_ylabel("S(x)",   fontsize=11)
    ax_ds.set_ylabel("S'(x)", fontsize=11)
    ax_ds.set_xlabel("x",     fontsize=11)
    title_obj = ax_s.set_title(path, fontsize=10)

    # ── слайдеры ──────────────────────────────────────────────────────────────
    sliders = []
    for seg in range(n_seg):
        row  = seg // 2
        col  = seg %  2
        left = 0.07 + col * 0.50
        bot  = sliders_total - (row + 1) * (slider_h + slider_gap) + 0.01
        ax_sl = fig.add_axes([left, bot, 0.40, slider_h])
        sl    = mw.Slider(ax_sl,
                          label=f"r[{seg}]",
                          valmin=-0.9, valmax=12.0,
                          valinit=r_cur[seg],
                          valstep=0.05,
                          color="tab:blue")
        sl.label.set_fontsize(8)
        sl.valtext.set_fontsize(8)
        sliders.append(sl)

    # общий ползунок — в самом низу
    ax_all = fig.add_axes([0.07, 0.005, 0.74, slider_h])
    sl_all = mw.Slider(ax_all, label="r  ALL",
                       valmin=-0.9, valmax=12.0,
                       valinit=float(r_cur[0]),
                       valstep=0.05, color="tab:orange")
    sl_all.label.set_fontsize(8)
    sl_all.valtext.set_fontsize(8)

    ax_btn = fig.add_axes([0.88, 0.005, 0.10, slider_h])
    btn    = mw.Button(ax_btn, "Save", color="lightgreen")

    # ── колбэки ───────────────────────────────────────────────────────────────
    def update(_):
        for seg, sl in enumerate(sliders):
            r_cur[seg] = sl.val
        rs_f        = np.append(r_cur, r_cur[-1])
        sy_n, dsy_n = eval_spline_c(lib, xs, ys, dys, rs_f, x_grid)
        line_s.set_ydata(sy_n)
        line_ds.set_ydata(dsy_n)
        node_sc_ds.set_offsets(np.c_[xs, np.interp(xs, x_grid, dsy_n)])
        ax_s.relim();  ax_s.autoscale_view()
        ax_ds.relim(); ax_ds.autoscale_view()
        uniq  = np.unique(np.round(r_cur, 3))
        r_str = (f"{uniq[0]:.2f}" if len(uniq) == 1
                 else f"[{uniq.min():.2f}…{uniq.max():.2f}]")
        title_obj.set_text(f"{path}  —  r={r_str}")
        fig.canvas.draw_idle()

    def save(_):
        write_file(path, xs, ys, dys, np.append(r_cur, r_cur[-1]))

    def update_all(val):
        for sl in sliders:
            sl.set_val(val)   # обновит r_cur через update

    for sl in sliders:
        sl.on_changed(update)
    sl_all.on_changed(update_all)
    btn.on_clicked(save)

    plt.show()


if __name__ == "__main__":
    main()
