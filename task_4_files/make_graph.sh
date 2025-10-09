gnuplot -e "
set term pngcairo size 1200,700 enhanced font ',14';
set output 'plot_schema_${number_of_schema}.png';
set grid;

set xlabel 'ln(1/n)';
set ylabel 'ln(1/|y(1)-y_{n}|)';

set xzeroaxis; set yzeroaxis;
set xrange [0:*]; set yrange [0:*];

set title 'schema=0';
plot \
  'data_graph/tech_graph_${number_of_schema}_0.txt' using 1:2 with lines title 'A=1', \
  'data_graph/tech_graph_${number_of_schema}_1.txt' using 1:2 with lines title 'A=5', \
  'data_graph/tech_graph_${number_of_schema}_2.txt' using 1:2 with lines title 'A=10', \
  'data_graph/tech_graph_${number_of_schema}_3.txt' using 1:2 with lines title 'A=100';
"
