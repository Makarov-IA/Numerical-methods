clear && clear

n_nodes=150
n_eval=1000

mkdir -p data_graph data_plot plots

show_plots=0     # 1 - show interactive windows, 0 - save only

for set_number in 1 2 3 4 5; do
    ./task_8 ${set_number} ${n_nodes} ${n_eval} > data_graph/set_${set_number}_n${n_nodes}.txt
    echo "Saved data_graph/set_${set_number}_n${n_nodes}.txt"

    save_path="plots/set_${set_number}_uniform.png"
    python3 plot.py --set ${set_number} --nodes uniform --save "${save_path}" $( [ ${show_plots} -eq 1 ] && echo --show )
    save_path="plots/set_${set_number}_chebyshev.png"
    python3 plot.py --set ${set_number} --nodes chebyshev --save "${save_path}" $( [ ${show_plots} -eq 1 ] && echo --show )
done
