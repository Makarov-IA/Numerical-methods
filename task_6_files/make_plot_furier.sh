P=$(printf "%.3f" "$P")

gnuplot -e "

set term pngcairo size 1200,700 enhanced font ',14';
set output 'plots/furier/${right_part_number}_${P}.png';
set grid;

set xlabel 'x';
set ylabel 'y';

set xzeroaxis; set yzeroaxis;

set title 'right\_part\_number=${right_part_number} P=${P}';
plot \
  'data_graph/furier/${right_part_number}_${P}.txt' using 1:2 with lines lw 2 title 'Solution',\
  'data_graph/furier/${right_part_number}_${P}.txt' using 1:3 with lines lw 1 dt 2 title 'Theoretical solution',\
"
