all: hw7.o primegenerator.o traildivision.o millerrabin.o randomsearch.o maurer.o
	g++ -g -o hw7 hw7.o primegenerator.o traildivision.o millerrabin.o randomsearch.o maurer.o -L/home/scf-22/csci551b/openssl/lib -lcrypto

hw7: hw7.o primegenerator.o traildivision.o millerrabin.o randomsearch.o maurer.o
	g++ -g -o hw7 hw7.o primegenerator.o traildivision.o millerrabin.o randomsearch.o maurer.o -L/home/scf-22/csci551b/openssl/lib -lcrypto

maurer.o:
	g++ -c -g -Wall -I/home/scf-22/csci551b/openssl/include maurer.cpp hw7.h 

randomsearch.o:
	g++ -c -g -Wall -I/home/scf-22/csci551b/openssl/include randomsearch.cpp hw7.h 

millerrabin.o:
	g++ -c -g -Wall -I/home/scf-22/csci551b/openssl/include millerrabin.cpp hw7.h 

traildivision.o:
	g++ -c -g -Wall -I/home/scf-22/csci551b/openssl/include traildivision.cpp hw7.h

primegenerator.o:
	g++ -c -g -Wall -I/home/scf-22/csci551b/openssl/include randomsearch.cpp primegenerator.cpp hw7.h

hw7.o: 
	g++ -c -g -Wall -I/home/scf-22/csci551b/openssl/include traildivision.cpp hw7.cpp hw7.h

clean:
	$(RM) hw7 *~ *.o
