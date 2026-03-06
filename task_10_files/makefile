task_10: task_10.o ../utils/utils_task10.o ../utils/utils_interpolation.o
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror task_10.o ../utils/utils_task10.o ../utils/utils_interpolation.o -lm -o task_10

task_10.o: task_10.c
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c task_10.c -o task_10.o

../utils/utils_task10.o: ../utils/utils_task10.c ../utils/utils_task10.h ../utils/utils_interpolation.h
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/utils_task10.c -o ../utils/utils_task10.o

../utils/utils_interpolation.o: ../utils/utils_interpolation.c ../utils/utils_interpolation.h
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/utils_interpolation.c -o ../utils/utils_interpolation.o

clean:
	rm -f task_10 *.o ../utils/*.o
