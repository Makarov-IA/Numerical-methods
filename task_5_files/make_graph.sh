gnuplot -e "

set term pngcairo size 1200,700 enhanced font ',14';
set output 'plots/plot_number_of_function_${number_of_function}.png';
set grid;

set xlabel 'x';
set ylabel 'y';

set xzeroaxis; set yzeroaxis;

set title 'NumberOfFunction=${number_of_function}';
plot \
  'data_graph/number_of_function_${number_of_function}.txt' using 1:2 with lines lw 2 title 'CalculatedSol',\
  'data_graph/number_of_function_${number_of_function}.txt' using 1:3 with lines lw 2 title 'RealSol'
"


gnuplot -e "

set term pngcairo size 1200,700 enhanced font ',14';
set output 'plots/plot_number_of_function_${number_of_function}H.png';
set grid;

set xlabel 'x';
set ylabel 'h';

set xzeroaxis; set yzeroaxis;

set title 'HForNumberOfFunction=${number_of_function}';
plot \
  'data_graph/number_of_function_${number_of_function}.txt' using 1:4 with lines lw 2 title 'h'
"
