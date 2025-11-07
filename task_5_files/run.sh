set -euo pipefail #to be conf that if it is failed it won't continue to run
clear&&clear

for is_fixed_h in {0..1}; do #if h is fixed (used to prove the oreder of method)
    
    number_of_function=1 #number of function to solve ODE with
    x_init=0 #initial point of x
    xR=10 #the right end point
    h_init=1 #initial step size
    ./task_5 ${number_of_function} ${x_init} ${xR} ${h_init} ${is_fixed_h}
    
    if [ "$is_fixed_h" -eq 0 ]; then
        number_of_function=${number_of_function} bash make_plot.sh #Draw plot if h is fixed
    else 
        number_of_function=${number_of_function} bash make_prove.sh #Draw prove else
    fi

    number_of_function=2
    x_init=0
    xR=3
    h_init=1.3
    ./task_5 ${number_of_function} ${x_init} ${xR} ${h_init} ${is_fixed_h}
    
    if [ "$is_fixed_h" -eq 0 ]; then
        number_of_function=${number_of_function} bash make_plot.sh 
    else 
        number_of_function=${number_of_function} bash make_prove.sh 
    fi

    number_of_function=3
    x_init=-5
    xR=5
    h_init=0.5
    A=-5 #addiditional param to the function
    alpha=0 #addiditional param to the function
    beta=2 #addiditional param to the function
    ./task_5 ${number_of_function} ${x_init} ${xR} ${h_init} ${is_fixed_h} \
    ${A} ${alpha} ${beta}
    
    if [ "$is_fixed_h" -eq 0 ]; then
        number_of_function=${number_of_function} bash make_plot.sh 
    else 
        number_of_function=${number_of_function} bash make_prove.sh 
    fi
done