task_10: task_10.o ../utils/utils_task10.o ../utils/utils_matrix.o
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror task_10.o ../utils/utils_task10.o ../utils/utils_matrix.o -lm -o task_10

task_10.o: task_10.c
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c task_10.c -o task_10.o

../utils/utils_task10.o: ../utils/utils_task10.c ../utils/utils_task10.h ../utils/utils_matrix.h
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/utils_task10.c -o ../utils/utils_task10.o

../utils/utils_matrix.o: ../utils/utils_matrix.c ../utils/utils_matrix.h
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/utils_matrix.c -o ../utils/utils_matrix.o

clean:
	rm -f task_10 *.o ../utils/*.o
