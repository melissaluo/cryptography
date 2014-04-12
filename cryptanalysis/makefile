all: hw4.o keygen.o crypt.o histo.o solve.o
	g++ -g -o hw4 hw4.o keygen.o crypt.o histo.o solve.o

hw4: hw4.o keygen.o crypt.o histo.o solve.o
	g++ -g -o hw4 hw4.o keygen.o crypt.o histo.o solve.o

solve.o: solve.cpp hw4.h
	g++ -c -g -Wall solve.cpp hw4.h

histo.o: histo.cpp hw4.h
	g++ -c -g -Wall histo.cpp hw4.h

crypt.o: crypt.cpp hw4.h
	g++ -c -g -Wall crypt.cpp hw4.h

keygen.o: keygen.cpp
	g++ -c -g -Wall keygen.cpp

hw4.o: hw4.cpp hw4.h
	g++ -c -g -Wall hw4.cpp hw4.h

clean:
	$(RM) hw4 *~ *.o
