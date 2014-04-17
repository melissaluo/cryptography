all: hw6.o tablecheck.o modprod.o keyexpand.o encrypt.o decrypt.o inverse.o
	g++ -g -o hw6 hw6.o tablecheck.o modprod.o keyexpand.o encrypt.o decrypt.o inverse.o

hw6: hw6.o tablecheck.o modprod.o keyexpand.o encrypt.o decrypt.o inverse.o
	g++ -g -o hw6 hw6.o tablecheck.o modprod.o keyexpand.o encrypt.o decrypt.o inverse.o

inverse.o:
	g++ -c -g -Wall inverse.cpp hw6.h

decrypt.o:
	g++ -c -g -Wall decrypt.cpp hw6.h

encrypt.o:
	g++ -c -g -Wall encrypt.cpp hw6.h

keyexpand.o:
	g++ -c -g -Wall keyexpand.cpp hw6.h

modprod.o:
	g++ -c -g -Wall modprod.cpp hw6.h

tablecheck.o:
	g++ -c -g -Wall tablecheck.cpp hw6.h

hw6.o: 
	g++ -c -g -Wall hw6.cpp hw6.h

clean:
	$(RM) hw6 *~ *.o
