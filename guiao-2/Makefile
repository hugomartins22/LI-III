CC = gcc
FLAGS= -std=c99 -Wextra -O2 -D_GNU_SOURCE 

all: g2.o struct.o allprog1
	
g2.o: src/g2.c 
	$(CC) -o obj/g2.o -c src/g2.c

struct.o: src/struct.c src/struct.h
	$(CC) -o obj/struct.o -c src/struct.c

allprog1:
	$(CC) -o guiao-2 obj/g2.o obj/struct.o

clean:
	rm obj/*.o
