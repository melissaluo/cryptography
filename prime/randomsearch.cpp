#include <string.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/bn.h>
#include "hw7.h"

using namespace std;

/* Random-Search(k,maxitr):
    1) n = RndOddNum(k)
    2) if (TrialDivision(n) == "fail") goto step (1)
    3) if (Miller-Rabin(n,maxitr) == "prime") return(n)
       goto step (1)
*/

void rndsearch(char *numbitsstr,char *maxitrstr, char *primesfilestr, char *rndfilestr){
	char *out;
	/* read the numbits */
	if( numbitsstr == NULL || (*numbitsstr) == '\0' ){
		errorhandle("malformed numbits!");
	}
		
	int k = atoi(numbitsstr);
	
	/* read the maxitr */
	if( maxitrstr == NULL || (*maxitrstr) == '\0' ){
		errorhandle("malformed maxitr!");
	}
		
	int maxitr = atoi(maxitrstr);
	
	/* read the file */
	FILE *file;
	unsigned char c[4];
	if(primesfilestr == NULL || *primesfilestr == '\0'){
		errorhandle("malformed command!");
	}
	file = fopen(primesfilestr,"rb");
	if(file == NULL){
		errorhandle("malformed primesfile!");
	}
	fread(c,sizeof(unsigned char),4,file);
	memset(c,0,4);
	int tempint;
	
	int *prime_array = new int[maxitr];
	/* read maxitr prime numbers */
	for(int itr=0;itr<maxitr;itr++){
		fread(c,sizeof(unsigned char),4,file);
		/* convert byte array to an integer */
		tempint = 0;
		for(int i=0;i<4;i++){
			tempint += c[3-i]*pow(256,i);
		}
		prime_array[itr] = tempint;
		memset(c,0,4);
	}	
	fclose(file);
	

	/* RndOddNum*/
	/* read the rndfile */
	FILE *rndfile;
	
	rndfile = fopen(rndfilestr,"rb");
	if(rndfile == NULL){
		errorhandle("malformed rndfile!");
	}
	
	BIGNUM* bn_number = BN_new();
	bn_number = RndOddNum(k, rndfile);
	out = BN_bn2dec(bn_number);
	
	/* TrialDivision(n) */
	int itr = 1;
	cout << "RANDOM-SEARCH: iteration " << itr << endl;
	cout << "  n = " << out << endl << "  ";
	
	/* open the primesfile */
	
	while(!TrailDivision(bn_number, primesfilestr, true)){
		bn_number = RndOddNum(k, rndfile);
		out = BN_bn2dec(bn_number);
		cout << "RANDOM-SEARCH: iteration " << ++itr << endl;
		cout << "  n = " << out << endl << "  ";
	}
	
	while(!MillerRabin(bn_number, maxitr, primesfilestr, '2')){
		bn_number = RndOddNum(k, rndfile);
	}
	
	fclose(rndfile);
}

BIGNUM* RndOddNum(int k, FILE *rndfile){
	//char *out;
	int x = (k-1)/8 + 1;
	unsigned char *binstr = new unsigned char[x];
	fread(binstr,sizeof(unsigned char),x,rndfile);
	binstr[x] = '\0';
	
	/* converts the data into a BIGNUM representation */
	BIGNUM* ret = BN_new();
	BN_bin2bn(binstr, x, ret);

	/* sets both bit 0 and bit k-1 to one,
	   sets all bits from k and above to 0 in the BIGNUM */
	BN_set_bit(ret,0);
	BN_set_bit(ret,k-1);
	
	for(int i=k;i< (8*x);i++){
		BN_clear_bit(ret,i);
	}
	
	delete[] binstr;
	return ret;
}
