CC = g++

CompileParms = -c -Wall -std=c++11 -O2

OBJS = standaard.o genereertankinfo.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o Genereer

clean:
	rm -f *.o Genereer

standaard.o: standaard.cc standaard.h
	$(CC) $(CompileParms)  standaard.cc

genereertankinfo.o: genereertankinfo.cc standaard.h
	$(CC) $(CompileParms)  genereertankinfo.cc

