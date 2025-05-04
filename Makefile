CC = g++

CompileParms = -c -Wall -g -std=c++11 -O2

OBJS = standaard.o tankplanner.o main.o rman.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o TankPlanner

clean:
	rm -f *.o TankPlanner

rman.o: rman.c rman.h
	gcc -O2 --std=c99 -Wall -g -c rman.c 

standaard.o: standaard.cc standaard.h
	$(CC) $(CompileParms)  standaard.cc

tankplanner.o: tankplanner.cc standaard.h constantes.h tankplanner.h rman.h
	$(CC) $(CompileParms)  tankplanner.cc

main.o: main.cc standaard.h constantes.h tankplanner.h
	$(CC) $(CompileParms)  main.cc

