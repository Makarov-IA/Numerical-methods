clear&&clear

schema_type=1      # 1 - явная схема
T=1.0
number_of_points_x=21
number_of_points_t=1001

for set_number in 1 2; do
    ./task_7 ${schema_type} ${set_number} ${T} ${number_of_points_x} ${number_of_points_t}
    set_number=${set_number} number_of_points_x=${number_of_points_x} number_of_points_t=${number_of_points_t} bash make_plot.sh
done
