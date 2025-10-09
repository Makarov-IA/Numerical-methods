task_2: task_2.o ../utils/utils_matrix.o
	gcc -Wall -Wextra -pedantic -Werror task_2.o ../utils/utils_matrix.o -o task_2 

task_2.o: task_2.c 
	gcc -Wall -Wextra -pedantic -Werror -c task_2.c -o task_2.o

../utils/utils_matrix.o: ../utils/utils_matrix.c
	gcc -Wall -Wextra -pedantic -Werror -c ../utils/utils_matrix.c -o ../utils/utils_matrix.o

clean:
	rm -f task_2 *.o ../utils/*.o