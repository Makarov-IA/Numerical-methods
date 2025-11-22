gnuplot -e "

set term pngcairo size 1200,700 enhanced font ',14';
set output 'plots/${dir}/${set_number}.png';
set grid;

set xlabel 'x';
set ylabel 'y';

set xzeroaxis; set yzeroaxis;

set title 'set\_number=${set_number} N=${N}';
plot \
  'data_graph/${dir}/${set_number}.txt' using 1:2 with lines lw 4 lc rgb 'blue' title 'Solution',\
  'data_graph/${dir}/${set_number}.txt' using 1:3 with lines lw 3 dt 2 lc rgb 'red' title 'Theoretical solution',\
"
