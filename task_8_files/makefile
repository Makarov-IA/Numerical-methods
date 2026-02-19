task_8: task_8.o ../utils/utils_interpolation.o
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror task_8.o ../utils/utils_interpolation.o -o task_8

task_8.o: task_8.c
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c task_8.c -o task_8.o

../utils/utils_interpolation.o: ../utils/utils_interpolation.c
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/utils_interpolation.c -o ../utils/utils_interpolation.o

clean:
	rm -f task_8 *.o ../utils/*.o
