#-y'' + py = f
for method in {1..2}; do #1 - метод Фурье, 2 - метод прогонки
    for set_number in {1..2}; do #От 1 до 2 - общие и на Фурье, и на прогонку
        N=101
        ./task_6 ${N} ${method} ${set_number}
        if [ "$method" -eq 1 ]; then
            N=${N} set_number=${set_number} dir=furier bash make_plot.sh
        else 
            N=${N} set_number=${set_number} dir=progonka bash make_plot.sh
        fi
    done
done

N=101
set_number=3 #От 4 только на прогонку
method=2
./task_6 ${N} ${method} ${set_number}
if [ "$method" -eq 1 ]; then
    N=${N} set_number=${set_number} dir=furier bash make_plot.sh
else 
    N=${N} set_number=${set_number} dir=progonka bash make_plot.sh
fi

N=101
set_number=4 #От 4 только на прогонку
method=2
./task_6 ${N} ${method} ${set_number}
if [ "$method" -eq 1 ]; then
    N=${N} set_number=${set_number} dir=furier bash make_plot.sh
else 
    N=${N} set_number=${set_number} dir=progonka bash make_plot.sh
fi

N=101
set_number=5 #От 4 только на прогонку
method=2
./task_6 ${N} ${method} ${set_number}
if [ "$method" -eq 1 ]; then
    N=${N} set_number=${set_number} dir=furier bash make_plot.sh
else 
    N=${N} set_number=${set_number} dir=progonka bash make_plot.sh
fi

N=101
set_number=5 #От 4 только на прогонку
method=1
./task_6 ${N} ${method} ${set_number}
if [ "$method" -eq 1 ]; then
    N=${N} set_number=${set_number} dir=furier bash make_plot.sh
else 
    N=${N} set_number=${set_number} dir=progonka bash make_plot.sh
fi

# N=101
# set_number=6 #От 4 только на прогонку
# method=2
# ./task_6 ${N} ${method} ${set_number}
# if [ "$method" -eq 1 ]; then
#     N=${N} set_number=${set_number} dir=furier bash make_plot.sh
# else 
#     N=${N} set_number=${set_number} dir=progonka bash make_plot.sh
# fi