#-y'' + py = f
N=100
method=1 #1 - метод Фурье, 2 - метод прогонки
right_part_number=2 #Варианты от [1, ..., 2]
P=1 # это p из формулы, в случае Фурье это значение, в случае прогонки номер функции для задания P 
./task_6 ${N} ${method} ${right_part_number} ${P}
right_part_number=${right_part_number} P=${P} bash make_plot_furier.sh

N=100
method=1 #1 - метод Фурье, 2 - метод прогонки
right_part_number=1 #Варианты от [1, ..., 2]
P=1 # это p из формулы, в случае Фурье это значение, в случае прогонки номер функции для задания P 
./task_6 ${N} ${method} ${right_part_number} ${P}
right_part_number=${right_part_number} P=${P} bash make_plot_furier.sh
