task_3: task_3.o ../utils/utils.o
	gcc task_3.o ../utils/utils.o -o task_3 

task_3.o: task_3.c 
	gcc -c task_3.c -o task_3.o

../utils/utils.o: ../utils/utils.c
	gcc -c ../utils/utils.c -o ../utils/utils.o

clean:
	rm -f task_3 *.o ../utils/*.o