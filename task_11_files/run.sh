#!/bin/bash
clear && clear

rm -rf plots
mkdir -p plots

case "$(uname)" in
    Darwin) LIB=libtask11.dylib ;;
    *)      LIB=libtask11.so    ;;
esac

gcc -shared -fPIC -O2 \
    ../utils/utils_task11.c \
    -lm -o "$LIB" \
    || exit 1

for input_file in examples/set_*.txt; do
    name=$(basename "${input_file}" .txt)
    save_path="plots/${name}.png"
    if python3 plot.py "${input_file}" --save "${save_path}"; then
        echo "Saved ${save_path}"
    else
        echo "Skipped ${save_path}: plot.py failed" >&2
    fi
done
