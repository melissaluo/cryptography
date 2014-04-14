#include <iostream>

using namespace std;

#define S_NUM 256 /*** 00 to ff ***/
#define P_NUM 4 /*** 4 bytes ***/
#define Nb 4 	/*** Nb: number of columns in state ***/
#define Nk 4 /*** Nk: number of 32-bit words in the key ***/
#define Nr 10 /*** Nr: number of rounds ***/

typedef int Word;

void errorhandle(const char* msg);

void tableCheck(char *filename);
void inversesbox();
void inversecheck();

unsigned char hextochar(char c1, char c2);
void modprod(char *p1, char *p2);
void polyCheck(char *p, unsigned char p_arr[], int len);
unsigned char bigdotbyte(unsigned char a, unsigned char b);
void keyexpand(char *key,char *tablename, char flag);
void keyExpansion();
unsigned char rcon(int i);
unsigned char sbox(unsigned char c);
unsigned char invsbox(unsigned char c);
void rotate(unsigned char *in);
void schedule_core(unsigned char *in, int i);
void output();
void aesencrypt(char *key, char *tablename, istream& in);
void inputcheck(istream& in);
void AddRoundKey(int round_no);
void SubBytes();
void ShiftRows();
void MixColumns();
void outputword(unsigned char *w, int round_no, char *step_log);


void aesdecrypt(char *key, char *tablename, istream& in);
void invinputcheck(istream& in);
void invAddRoundKey(int round_no);
void invoutputword(unsigned char *w, int round_no, const char *step_log);
void invSubBytes();
void invShiftRows();
void invMixColumns();




