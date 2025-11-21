task_6: task_6.o ../utils/utils_matrix.o ../utils/second_order_ode.o
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror task_6.o ../utils/utils_matrix.o ../utils/second_order_ode.o -o task_6 

task_6.o: task_6.c 
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c task_6.c -o task_6.o

../utils/utils_matrix.o: ../utils/utils_matrix.c
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/utils_matrix.c -o ../utils/utils_matrix.o

../utils/second_order_ode.o: ../utils/second_order_ode.c
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/second_order_ode.c -o ../utils/second_order_ode.o

clean:
	rm -f task_6 *.o ../utils/*.o