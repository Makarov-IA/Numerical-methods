set -euo pipefail

if grep -qF 'Our solution exactly matches the real func!' "data_graph/${number_of_function}_prove.txt"; then
  exit 1
fi

data_file="data_graph/${number_of_function}_prove.txt"
plot_file="plots/${number_of_function}_prove.png"

read -r x0 y0 < "${data_file}"

gnuplot -e "
set term pngcairo size 1200,700 enhanced font ',14';
set output '${plot_file}';
set grid;

set xlabel 'ln(1/h)';
set ylabel 'ln(1/|y(1)-y_{n}|)';

set xzeroaxis;
set yzeroaxis;

set title 'Runge method';

a1 = ${y0} - 1*${x0};
a2 = ${y0} - 2*${x0};
a3 = ${y0} - 3*${x0};
a4 = ${y0} - 4*${x0};

plot \
  '${data_file}' u 1:2 w lp lw 2 pt 7 ps 1.1 t 'Calculated',\
  1*x + a1 t 'k=1' lc rgb 'green',\
  2*x + a2 t 'k=2' lc rgb 'black',\
  3*x + a3 t 'k=3' lc rgb 'orange',\
  4*x + a4 t 'k=4' lc rgb 'blue';
"
