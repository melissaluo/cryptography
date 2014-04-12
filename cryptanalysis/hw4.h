#include <iostream>
#include <fstream>/*fstream*/
#include <vector>
#include <string.h>
#include <map>
#include <utility>/*pair*/
#define STDOUT 0
#define CIPHER 1
#define start_index 4

#define kp 0.0658/*for EICs computation*/
#define N 26

typedef std::pair<char,int> mypair;

typedef struct solvetype{
	int i;/*the first index*/
	int j;/*the second index*/
	char* text;/*matched string content*/
}matched;

void errorhandle(const char*);
void keygen(int,int,std::vector<std::string>);
void crypt(char*,std::istream&);
void invkey(char*);
void perputationgenerator(char*);
void swap(char*, char*);
void histo(int, int, std::istream&);
void sortA(std::vector<mypair>,int);
void printer(int,std::vector<mypair>);
void solve(int,std::fstream&);

matched make_matched(int, int, char*, int);
//void KasiskiMethod(int,int,char*);
void KasiskiMethod(int,char*);
bool searchcipher(int,char*,int);
//void KasiskiPrinter(int);
void KasiskiPrinter();
void avgindexcoin(int,int,char*);
void avgindexcoinPrinter(int, int, std::vector<mypair>,double, double*);
void AutocorrelationMethod(int, int, char*);
void AutocorrelationPrinter(int, int[]);











