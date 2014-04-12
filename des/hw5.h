#include <iostream>
#include "table.h"
#include <vector>

using namespace std;


/*hw5.cpp*/
void errorhandle(const char* msg);

/*encrypt.cpp*/
void encrypt(char* key, char* tablefile,istream& f);
void inputProcess(istream& f);
void keySchedule(char* key,const Table& table);
void IPinput(const Table& table);
void mainProcess(const Table& table);

/*decrypt.cpp*/
void decrypt(char* key, char* tablefile,istream& f);

/*encrypt3.cpp*/
void encrypt3(char* key, char* tablefile,istream& f);

/*decrypt3.cpp*/
void decrypt3(char* key, char* tablefile,istream& f);
