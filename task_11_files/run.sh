#!/bin/bash
clear && clear

rm -rf data_graph data_plot plots
mkdir -p data_graph data_plot plots

gcc -Wall -Wextra -pedantic -Wunused-variable -Werror \
    task_11.c ../utils/utils_task11.c ../utils/utils_interpolation.c \
    -lm -o task_11 \
    || exit 1

show_plots=0   # 1 - show interactive windows, 0 - save only

for input_file in examples/set_*.txt; do
    name=$(basename "${input_file}" .txt)

    ./task_11 "${input_file}" \
        > "data_graph/${name}.txt"
    echo "Saved data_graph/${name}.txt"

    save_path="plots/${name}.png"
    if python3 plot.py --save "${save_path}" \
        $( [ ${show_plots} -eq 1 ] && echo --show ); then
        echo "Saved ${save_path}"
    else
        echo "Skipped ${save_path}: failed to run plot.py" >&2
    fi
done
