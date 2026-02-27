clear && clear

n_nodes=11
set_numbers="1 2 3 4 5 6 7"
seed=42

mkdir -p data_graph data_plot plots

show_plots=1   # 1 - show interactive windows, 0 - save only

for set_number in ${set_numbers}; do
    if [ "${set_number}" = "1" ] || [ "${set_number}" = "3" ]; then
        a=-1.0
        b=1.0
    else
        a=0.0
        b=1.0
    fi

    ./task_9 "${n_nodes}" "${a}" "${b}" "${seed}" "${set_number}" > "data_graph/set_${set_number}_n${n_nodes}.txt"
    echo "Saved data_graph/set_${set_number}_n${n_nodes}.txt"

    for nodes_type in uniform chebyshev random; do
        cp "data_plot/${nodes_type}.txt" "data_plot/set_${set_number}_n${n_nodes}_${nodes_type}.txt"
        cp "data_plot/${nodes_type}_nodes.txt" "data_plot/set_${set_number}_n${n_nodes}_${nodes_type}_nodes.txt"
        save_path="plots/set_${set_number}_n${n_nodes}_${nodes_type}.png"
        if python3 plot.py --nodes "${nodes_type}" --save "${save_path}" $( [ ${show_plots} -eq 1 ] && echo --show ); then
            echo "Saved ${save_path}"
        else
            echo "Skipped ${save_path}: failed to run plot.py (check matplotlib)." >&2
        fi
    done
done

rm -rf data_plot
