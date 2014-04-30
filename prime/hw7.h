#include <openssl/bn.h>

using namespace std;

void errorhandle(const char* msg);


/* primegenerator.cpp */
void primeGenerator(char *maxval);
void Eratosthenes(int N, unsigned char* prime_array, int bytes_num);
void setbit(unsigned char *prime_array, int n);
int getbit(unsigned char *prime_array, int n);


/* traildivision.cpp */
void traildiv(char *number, char *primesfile);
bool TrailDivision(BIGNUM *bn_number, char *primesfilestr, bool output);

/* millerrabin.cpp */
void millerrabin(char *number, char *maxitr, char *primesfile);
bool MillerRabin(BIGNUM *bn_number, int maxitr, char *primesfilestr, char output);

/* randomsearch.cpp */
void rndsearch(char *numbitsstr,char *maxitrstr, char *primesfilestr, char *rndfilestr);
BIGNUM* RndOddNum(int k, FILE *rndfile);

/* maurer.cpp */
void maurer(char *numbitsstr, char *maxitrstr, char *primesfilestr);
unsigned char RndByte(FILE *rndfile);
BIGNUM *Maurer(int k);


