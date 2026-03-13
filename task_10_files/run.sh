clear && clear

mkdir -p data_plot plots

for example_name in example_1 example_2; do
    input_path="examples/${example_name}.txt"
    plot_path="plots/${example_name}.png"
    ./task_10 "${input_path}"
    if python3 plot.py --save "${plot_path}"; then
        echo "Saved ${plot_path}"
    else
        echo "Skipped ${plot_path}: failed to run plot.py (check matplotlib)." >&2
    fi
done
