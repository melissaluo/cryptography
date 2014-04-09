all: main.o hexdump.o encodebase64.o decodebase64.o
	g++ -g -o hw1 main.o hexdump.o encodebase64.o decodebase64.o

hw1: main.o hexdump.o encodebase64.o decodebase64.o
	g++ -g -o hw1 main.o hexdump.o encodebase64.o decodebase64.o

hexdump.o: hexdump.cpp main.h
	g++ -c -g -Wall hexdump.cpp main.h

encodebase64.o: encodebase64.cpp main.h
	g++ -c -g -Wall encodebase64.cpp main.h

decodebase64.o: decodebase64.cpp main.h
	g++ -c -g -Wall decodebase64.cpp main.h

main.o: main.cpp main.h
	g++ -c -g -Wall main.cpp main.h

clean:
	$(RM) hw1 *~ *.o
