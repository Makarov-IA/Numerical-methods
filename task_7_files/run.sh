clear&&clear

T=1.0
number_of_points_x=21
number_of_points_t=1001

for set_number in 1 2 3 4; do
    # явная схема
    ./task_7 1 ${set_number} ${T} ${number_of_points_x} ${number_of_points_t}
    mv data_graph/${set_number}.txt data_graph/${set_number}_explicit.txt
    set_number=${set_number}_explicit number_of_points_x=${number_of_points_x} number_of_points_t=${number_of_points_t} bash make_plot.sh

    # неявная схема
    ./task_7 2 ${set_number} ${T} ${number_of_points_x} ${number_of_points_t}
    mv data_graph/${set_number}.txt data_graph/${set_number}_implicit.txt
    set_number=${set_number}_implicit number_of_points_x=${number_of_points_x} number_of_points_t=${number_of_points_t} bash make_plot.sh
done
