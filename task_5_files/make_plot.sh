set -euo pipefail
mkdir -p plots
gnuplot -e "set term pngcairo size 1200,900 enhanced font ',14' dashed; \
set output sprintf('plots/%s.png','${number_of_function}'); \
set datafile commentschars '#'; \
set multiplot layout 2,1 title 'Runge method' font ',14'; \
set grid; set xzeroaxis; set yzeroaxis; \
set xlabel 'x'; set ylabel 'y'; \
plot sprintf('data_graph/%s.txt','${number_of_function}') u 1:2 w lp lw 2 pt 7 ps 1.1 t 'Calculated', \
     sprintf('data_graph/%s.txt','${number_of_function}') u 1:3 w lp lw 2 dt 2 pt 7 ps 1.1 t 'Real'; \
set xlabel 'x'; set ylabel 'h'; \
plot sprintf('data_graph/%s.txt','${number_of_function}') u 1:4 w lp lw 2 pt 7 ps 1.1 t 'h'; \
unset multiplot; set output"
