mkdir -p plots
gnuplot -e "set term pngcairo size 1200,900 enhanced font ',14' dashed; \
set output sprintf('plots/%s.png','${number_of_function}'); \
set multiplot layout 2,1 title 'Runge method' font ',14'; \
set grid; set xzeroaxis; set yzeroaxis; \
set xlabel 'x'; set ylabel 'y'; \
plot sprintf('data_graph/%s.txt','${number_of_function}') every ::1 using 1:2 with lines lw 4 title 'Calculated', \
     sprintf('data_graph/%s.txt','${number_of_function}') every ::1 using 1:3 with lines lw 3 dt 2 title 'Real'; \
set xlabel 'x'; set ylabel 'h'; \
plot sprintf('data_graph/%s.txt','${number_of_function}') every ::1 using 1:4 with lines lw 3  lc 'red' title 'h'; \
unset multiplot; set output"
