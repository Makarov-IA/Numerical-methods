task_2: task_2.o ../utils/utils.o
	gcc -Wall -Wextra -pedantic -Werror task_2.o ../utils/utils.o -o task_2 

task_2.o: task_2.c 
	gcc -Wall -Wextra -pedantic -Werror -c task_2.c -o task_2.o

../utils/utils.o: ../utils/utils.c
	gcc -Wall -Wextra -pedantic -Werror -c ../utils/utils.c -o ../utils/utils.o

clean:
	rm -f task_2 *.o ../utils/*.o