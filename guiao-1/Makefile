CC = gcc
FLAGS= -std=c99 -Wextra -O2 -D_GNU_SOURCE 

all: g1 g1-2 allprog1 allprog2 
	
g1: guiao1-1.c 
	$(CC) -o guiao-1-1.o -c guiao1-1.c

g1-2: guiao1-2.c 
	$(CC) -o g1-2.o -c guiao1-2.c

allprog1: guiao-1-1.o  
	$(CC) guiao-1-1.o -o guiao-1\ exercicio-1 -lm

allprog2: g1-2.o
	$(CC) g1-2.o -o guiao-1\ exercicio-2 -lm
	
memcheck:
	valgrind --leak-check=yes ./guiao-1\ exercicio-1
	valgrind --leak-check=yes ./guiao-1\ exercicio-2

clean:
	rm -rf *.o
	rm *.csv 