task_4: task_4.o ../utils/utils_schema.o
	gcc -Wall -Wextra -pedantic -Werror task_4.o ../utils/utils_schema.o -o task_4 

task_4.o: task_4.c 
	gcc -Wall -Wextra -pedantic -Werror -c task_4.c -o task_4.o

../utils/utils_schema.o: ../utils/utils_schema.c
	gcc -Wall -Wextra -pedantic -Werror -c ../utils/utils_schema.c -o ../utils/utils_schema.o

clean:
	rm -f task_4 *.o ../utils/*.o