task_7: task_7.o ../utils/utils_matrix.o ../utils/second_order_pd.o
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror task_7.o ../utils/utils_matrix.o ../utils/second_order_pd.o -o task_7 -lm

task_7.o: task_7.c
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c task_7.c -o task_7.o

../utils/utils_matrix.o: ../utils/utils_matrix.c
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/utils_matrix.c -o ../utils/utils_matrix.o

../utils/second_order_pd.o: ../utils/second_order_pd.c
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/second_order_pd.c -o ../utils/second_order_pd.o

clean:
	rm -f task_7 *.o ../utils/*.o
