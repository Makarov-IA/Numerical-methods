diff=1e+0
gnuplot -e "

set term pngcairo size 1600,1200 enhanced font ',12';
set output 'plots/${set_number}.png';

set grid;
set xlabel 'x';
set ylabel 't';
set zlabel 'u';
set hidden3d;
set key outside;

set multiplot layout 2,2 title 'set\_number=${set_number} Nx=${number_of_points_x} Nt=${number_of_points_t}' font ',14';

set view 60,40; splot 'data_graph/${set_number}.txt' using 2:1:(\$3) with points pt 7 ps 0.8 lc rgb 'blue' title 'u(t,x)', \
                            'data_graph/${set_number}.txt' using 2:1:(\$4+${diff}) with points pt 7 ps 0.8 lc rgb 'red' title 'theoretical (+eps)';

set view 30,120; splot 'data_graph/${set_number}.txt' using 2:1:(\$3) with points pt 7 ps 0.8 lc rgb 'blue' title 'u(t,x)', \
                              'data_graph/${set_number}.txt' using 2:1:(\$4+${diff}) with points pt 7 ps 0.8 lc rgb 'red' title 'theoretical (+eps)';

set view 80,220; splot 'data_graph/${set_number}.txt' using 2:1:(\$3) with points pt 7 ps 0.8 lc rgb 'blue' title 'u(t,x)', \
                              'data_graph/${set_number}.txt' using 2:1:(\$4+${diff}) with points pt 7 ps 0.8 lc rgb 'red' title 'theoretical (+eps)';

set view 20,310; splot 'data_graph/${set_number}.txt' using 2:1:(\$3) with points pt 7 ps 0.8 lc rgb 'blue' title 'u(t,x)', \
                              'data_graph/${set_number}.txt' using 2:1:(\$4+${diff}) with points pt 7 ps 0.8 lc rgb 'red' title 'theoretical (+eps)';

unset multiplot;
"
