#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <openssl/bn.h>
#include "hw7.h"

using namespace std;

bool MillerRabin(BIGNUM *bn_number, int maxitr, char *primesfilestr, char output){
		
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
	
		
	BIGNUM *bn_one = BN_new();
	BN_one(bn_one);
	
	BIGNUM *bn_number_1 = BN_new();
	BN_sub(bn_number_1, bn_number, bn_one);
	
		
	BIGNUM* bn_two = BN_new();
	BN_set_word(bn_two, 2);
	
	BN_ULONG two = 2;
	
	BN_CTX *ctx = BN_CTX_new();
		
	BIGNUM* y = BN_new();
		
	char *out;
	
	/* output number */
	if(output == '1'){
		out = BN_bn2dec(bn_number);
		cout << "n = "<< out << endl;
		out = BN_bn2dec(bn_number_1);
		cout << "  n-1 = " << out << endl;
	}else if(output == '2'){
		
		out = BN_bn2dec(bn_number_1);
		cout << "    n-1 = " << out << endl;
	}
	
	/* 1) write n-1 = 2^s*r such that r is odd */
	
	int s=0;
	BIGNUM* bn_pows = BN_new();
	BN_one(bn_pows);
	BIGNUM* bn_r = BN_new();	
	BIGNUM *dv = BN_new();
	
	while(BN_cmp(bn_pows,bn_number_1) < 0){
		BN_div(dv,bn_r,bn_number_1,bn_pows,ctx);

		if(BN_is_zero(bn_r) && BN_is_odd(dv)){
			break;
		}else{
			s++;
			BN_mul_word(bn_pows, two);
		}
	}
	
	if(output == '1'){
		cout << "  s = " << s << endl;
	
		out = BN_bn2dec(dv);
		cout << "  r = " << out << endl;
	}else if(output == '2'){
		cout << "    s = " << s << endl;
	
		out = BN_bn2dec(dv);
		cout << "    r = " << out << endl;
	}
	
	bool flag = true;
	int a=0;
	
	/* do iteration */
	for(int i=1;i<=maxitr;i++){
		a = prime_array[i-1];
		
		BIGNUM* bn_a = BN_new();
		BN_set_word(bn_a, a);
		
		/* y = a^r mode number */
		BN_mod_exp(y,  bn_a, dv,  bn_number, ctx);
		out = BN_bn2dec(y);
		
		if(output == '1'){
			if(BN_cmp(y, bn_number_1) == 0){
				cout << "  Itr " << i << " of " << maxitr << ", a = " << a << ", y = " << out << " (which is n-1)" << endl;
			}else{
				cout << "  Itr " << i << " of " << maxitr << ", a = " << a << ", y = " << out << endl;
			}
		}else if(output == '2'){
			if(BN_cmp(y, bn_number_1) == 0){
				cout << "    Itr " << i << " of " << maxitr << ", a = " << a << ", y = " << out << " (which is n-1)" << endl;
			}else{
				cout << "    Itr " << i << " of " << maxitr << ", a = " << a << ", y = " << out << endl;
			}
		}

		if((BN_cmp(y, bn_one) != 0) && (BN_cmp(y, bn_number_1) != 0)){
			int j=1;
			for(j=1;j<=(s-1) && (BN_cmp(y, bn_number_1) != 0);j++){
				
				/* y = y^2 mode number */
				BN_mod_exp(y,y,bn_two,bn_number,ctx);
				out = BN_bn2dec(y);
				
				if(output == '1'){
					if(BN_cmp(y, bn_number_1) == 0){
						cout << "    j = " << j << " of " << s-1 << ", y = " << out << " (which is n-1)" << endl;
					}else{
						cout << "    j = " << j << " of " << s-1 << ", y = " << out << endl;
					}
				}else if(output == '2'){
					if(BN_cmp(y, bn_number_1) == 0){
						cout << "      j = " << j << " of " << s-1 << ", y = " << out << " (which is n-1)" << endl;
					}else{
						cout << "      j = " << j << " of " << s-1 << ", y = " << out << endl;
					}
				}
				
				if(BN_cmp(y, bn_one) == 0){
					flag = false;
					break;
				}
			}
			
			if(BN_cmp(y, bn_number_1) != 0){
				flag = false;
				break;
			}
		}
	}
	
	if(output == '1'){
		if(flag){
			cout << "Miller-Rabin declares n to be a prime number" << endl;
		}else{
			cout << "Miller-Rabin found a strong witness " << a << endl;
		}
	}else if(output == '2'){
		if(flag){
			cout << "  Miller-Rabin declares n to be a prime number" << endl;
		}else{
			cout << "  Miller-Rabin found a strong witness " << a << endl;
		}
	}
	
	return flag;
}

void millerrabin(char *numberstr, char *maxitrstr, char *primesfilestr){

	/* read the maxitr */
	if( maxitrstr == NULL || (*maxitrstr) == '\0' ){
		errorhandle("malformed maxitrstr!");
	}
		
	int maxitr = atoi(maxitrstr);
	if(maxitr <= 0){
		errorhandle("invalid maxitr!");
	}
	
	/* read the number */
	if( numberstr == NULL || (*numberstr) == '\0' ){
		errorhandle("malformed number!");
	}
	
	BIGNUM *bn_number = BN_new();
	if(!BN_dec2bn(&bn_number, numberstr)){
		errorhandle("invalid number!");
	}
	
	MillerRabin(bn_number, maxitr, primesfilestr, '1');

}
