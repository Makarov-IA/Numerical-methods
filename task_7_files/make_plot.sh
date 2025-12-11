gnuplot -e "

set term pngcairo size 1200,800 enhanced font ',12';
set output 'plots/${set_number}.png';
set grid;

set xlabel 'x';
set ylabel 't';
set zlabel 'u';

set title 'set\_number=${set_number} Nx=${number_of_points_x} Nt=${number_of_points_t}';
set hidden3d;
set key outside;

splot \
  'data_graph/${set_number}.txt' using 2:1:3 with lines lc rgb 'blue' title 'u(t,x)', \
  'data_graph/${set_number}.txt' using 2:1:4 with lines lc rgb 'red' dt 2 title 'theoretical';
"
