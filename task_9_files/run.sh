clear && clear

n_nodes=11
a=-1.0
b=1.0
seed=42

mkdir -p data_graph data_plot plots

show_plots=0   # 1 - show interactive windows, 0 - save only

./task_9 "${n_nodes}" "${a}" "${b}" "${seed}" > "data_graph/task_9_n${n_nodes}.txt"
echo "Saved data_graph/task_9_n${n_nodes}.txt"

for nodes_type in uniform chebyshev random; do
    save_path="plots/${nodes_type}.png"
    if python3 plot.py --nodes "${nodes_type}" --save "${save_path}" $( [ ${show_plots} -eq 1 ] && echo --show ); then
        echo "Saved ${save_path}"
    else
        echo "Skipped ${save_path}: failed to run plot.py (check matplotlib)." >&2
    fi
done
