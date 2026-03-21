#!/bin/bash
clear && clear

input="examples/${1}"

# компилируем shared library из C-кода
case "$(uname)" in
    Darwin) LIB=libtask11.dylib ;;
    *)      LIB=libtask11.so    ;;
esac

gcc -shared -fPIC -O2 \
    ../utils/utils_task11.c \
    -lm -o "$LIB" \
    || exit 1

python3 plot_interactive.py "$input"
