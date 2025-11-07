clear&&clear

number_of_function=1
x_init=0
xR=10
h_init=1
./task_5 ${number_of_function} ${x_init} ${xR} ${h_init}
number_of_function=${number_of_function} bash make_graph.sh 

number_of_function=2
x_init=0
xR=3
h_init=1.3
./task_5 ${number_of_function} ${x_init} ${xR} ${h_init}
number_of_function=${number_of_function} bash make_graph.sh 

number_of_function=3
x_init=-5
xR=5
h_init=0.5
A=-5
alpha=0
beta=2
./task_5 ${number_of_function} ${x_init} ${xR} ${h_init} ${A} ${alpha} ${beta}
number_of_function=${number_of_function} bash make_graph.sh 