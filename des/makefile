all: hw5.o table.o encrypt.o
	g++ -g -o hw5 hw5.o table.o encrypt.o

hw5: hw5.o table.o encrypt.o
	g++ -g -o hw5 hw5.o table.o encrypt.o

encrypt.o:
	g++ -c -g -Wall encrypt.cpp

table.o:
	g++ -c -g -Wall table.cpp table.h hw5.h

hw5.o: 
	g++ -c -g -Wall hw5.cpp table.h hw5.h

clean:
	$(RM) hw5 *~ *.o
