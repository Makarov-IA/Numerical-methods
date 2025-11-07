task_5: task_5.o ../utils/utils_Runge.o
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror task_5.o ../utils/utils_Runge.o -o task_5 

task_5.o: task_5.c 
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c task_5.c -o task_5.o

../utils/utils_Runge.o: ../utils/utils_Runge.c
	gcc -Wall -Wextra -pedantic -Wunused-variable -Werror -c ../utils/utils_Runge.c -o ../utils/utils_Runge.o

clean:
	rm -f task_5 *.o ../utils/*.o