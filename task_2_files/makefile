task_2: task_2.o ../utils/utils.o
	gcc task_2.o ../utils/utils.o -o task_2 

task_2.o: task_2.c 
	gcc -c task_2.c -o task_2.o

../utils/utils.o: ../utils/utils.c
	gcc -c ../utils/utils.c -o ../utils/utils.o

clean:
	rm -f task_2 *.o ../utils/*.o