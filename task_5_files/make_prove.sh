gnuplot -e "

set term pngcairo size 1200,700 enhanced font ',14';
set output 'plots/${number_of_function}_prove.png';
set grid;

set xlabel 'ln(1/h)';
set ylabel 'ln(1/|y(1)-y_{n}|)';

set xzeroaxis; set yzeroaxis;

set title 'Runge method';
plot \
  'data_graph/${number_of_function}_prove.txt' u 1:2 w lp lw 2 pt 7 ps 1.1 t 'Calculated',\
  x t 'y=x+1' lc rgb 'green',\
  2*x + 1 t 'y=2*x+2' lc rgb 'black',\
  3*x + 2 t 'y=3*x+3' lc rgb 'orange',\
  4*x + 3 t 'y=4*x+4' lc rgb 'blue';
" 2>/dev/null #Отрубаем warning
