CC = gcc
FLAGS= -std=c99 -Wextra -O2 -D_GNU_SOURCE 

all: g2.o commits.o data.o generic.o lang.o queries.o repos.o users.o g3.o testes.o allprog1 allprog2
	

g2.o: src/g2.c 
	$(CC)  -o obj/g2.o -c src/g2.c -lncurses 

commits.o: src/commits.c src/commits.h 
	$(CC)  -o obj/commits.o -c src/commits.c -lncurses 

data.o: src/data.c src/data.h 
	$(CC)   -o obj/data.o -c src/data.c -lncurses 

generic.o: src/generic.c src/generic.h 
	$(CC)   -o obj/generic.o -c src/generic.c -lncurses  

lang.o: src/lang.c src/lang.h 
	$(CC)  -o obj/lang.o -c src/lang.c -lncurses  

queries.o: src/queries.c src/queries.h 
	$(CC)  -o obj/queries.o -c src/queries.c -lncurses  

repos.o: src/repos.c src/repos.h 
	$(CC)  -o obj/repos.o -c src/repos.c -lncurses  

users.o: src/users.c src/users.h 
	$(CC)  -o obj/users.o -c src/users.c -lncurses  

g3.o: src/g3.c src/g3.h 
	$(CC)  -o obj/g3.o -c src/g3.c -lncurses  

testes.o: src/testes.c  
	$(CC)  -o obj/testes.o -c src/testes.c -lncurses  

allprog1:
	$(CC)  -o guiao-3 obj/g2.o obj/commits.o obj/data.o obj/generic.o obj/lang.o obj/queries.o obj/repos.o obj/users.o  obj/g3.o      -lncurses  

allprog2:
	$(CC)  -o testes obj/testes.o obj/commits.o obj/data.o obj/generic.o obj/lang.o obj/queries.o obj/repos.o obj/users.o  obj/g3.o      -lncurses  

clean:
	rm obj/*.o
