task_3: task_3.o ../utils/utils_matrix.o
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror task_3.o ../utils/utils_matrix.o -o task_3 

task_3.o: task_3.c 
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c task_3.c -o task_3.o

../utils/utils_matrix.o: ../utils/utils_matrix.c
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/utils_matrix.c -o ../utils/utils_matrix.o

clean:
	rm -f task_3 *.o ../utils/*.o