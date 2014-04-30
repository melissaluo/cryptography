#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <openssl/bn.h>
#include "hw7.h"

using namespace std;

bool TrailDivision(BIGNUM *bn_number, char *primesfilestr, bool output){
	char *out;
	
	bool flag = true;
	
	/* open the primesfile */
	FILE *file;
	if(primesfilestr == NULL || *primesfilestr == '\0'){
		errorhandle("malformed command!");
	}
	file = fopen(primesfilestr, "rb");
	
	if(file == NULL){
		errorhandle("malformed primesfile!");
	}
	
	unsigned char c[4];
	/* skip the first one */
	fread(c,sizeof(unsigned char),4,file);
	memset(c,0,4);
	
	BIGNUM* bn_temp = BN_new();
	
	BIGNUM* bn_ret = BN_new();
	
	BIGNUM* bn_two = BN_new();
	BN_set_word(bn_two, 2);
	
	BN_CTX *ctx = BN_CTX_new();
	
	/* read 4 bytes a time */
	while(!feof(file)){
		fread(c,sizeof(unsigned char),4,file);
		
		/* convert byte array to an integer */
		
		BN_bin2bn(c, 4, bn_temp);
		memset(c,0,4);

		/* test primality to number */
		if(BN_is_zero(bn_temp)){
			break;
		}
		
		BN_exp(bn_ret, bn_temp, bn_two, ctx);

		if(BN_cmp(bn_ret,bn_number) < 0){
			BN_div(NULL, bn_ret, bn_number, bn_temp, ctx);
			
			if(BN_is_zero(bn_ret)){
				flag = false;
				break;
			}
		}else{
			break;
		}
		
	}
	
	fclose(file);
	
	/* output result */
	out = BN_bn2dec(bn_temp);
	
	if(output == true){
		if(flag == false){
			cout << "n is composite by trial division (mod " << out << " = 0)" << endl;
		}else{
			if(BN_cmp(bn_ret,bn_number) <= 0){
				cout << "n passes trial division test (not enough primes)" << endl;
			}else{
				cout << "n passes trial division test" << endl;
			}
		}
	}
	return flag;
}

void traildiv(char *number, char *primesfilestr){
	/* read the number */
	if( number == NULL || (*number) == '\0' ){
		errorhandle("malformed number!");
	}
	
	
	BIGNUM *bn_number = BN_new();
	if(!BN_dec2bn(&bn_number, number)){
		errorhandle("invalid number!");
	}
	TrailDivision(bn_number, primesfilestr, true);

}
