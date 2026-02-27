task_9: task_9.o ../utils/utils_task9.o ../utils/utils_interpolation.o
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror task_9.o ../utils/utils_task9.o ../utils/utils_interpolation.o -lm -o task_9

task_9.o: task_9.c
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c task_9.c -o task_9.o

../utils/utils_task9.o: ../utils/utils_task9.c ../utils/utils_task9.h ../utils/utils_interpolation.h
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/utils_task9.c -o ../utils/utils_task9.o

../utils/utils_interpolation.o: ../utils/utils_interpolation.c ../utils/utils_interpolation.h
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/utils_interpolation.c -o ../utils/utils_interpolation.o

clean:
	rm -f task_9 *.o ../utils/*.o
