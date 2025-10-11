gnuplot -e "
set term pngcairo size 1200,700 enhanced font ',14';
set output 'plot_schema_${number_of_schema}.png';
set grid;

set xlabel 'ln(1/h)';
set ylabel 'ln(1/|y(1)-y_{n}|)';

set xzeroaxis; set yzeroaxis;
set xrange [0:*]; set yrange [0:*];

set title 'schema=${number_of_schema}';
plot \
  'data_graph/tech_graph_${number_of_schema}_0.txt' using 1:2 with lines lw 2 title 'A=1',\
  'data_graph/tech_graph_${number_of_schema}_1.txt' using 1:2 with lines lw 2 title 'A=5',\
  'data_graph/tech_graph_${number_of_schema}_2.txt' using 1:2 with lines lw 2 title 'A=10',\
  'data_graph/tech_graph_${number_of_schema}_3.txt' using 1:2 with lines lw 2 title 'A=100',\
  x + 20 t 'y=x+C' lc rgb 'black',\
  2*x + 20 t 'y=2*x+C' lc rgb 'black',\
  3*x + 20 t 'y=3*x+C' lc rgb 'black';\
"
