#include <string.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/bn.h>
#include "hw7.h"
#define round(X) (((X) >= 0) ? (int)((X)+0.5) : (int)((X)-0.5))

static int level = 0;
char *primesfile;
FILE *rndfile;

BIGNUM *Maurer(int k){
	char *out;
	BIGNUM* bn_number = BN_new();
	
	cout << "Maurer: level " << level << ", k=" << k << endl;
	/* step 1 */
	if(k <= 20){
		while(1){
			/* step 1.1 */
			bn_number = RndOddNum(k, rndfile);
			
			out = BN_bn2dec(bn_number);
			cout << "  step 1.1, n = " << out << endl;
			
			/* step 1.2 */
			cout << "    ";
			if(TrailDivision(bn_number, primesfile, true)){
				if(level == 0){
					out = BN_bn2dec(bn_number);
					cout << endl << "Maurer's Algorithm found an "<< BN_num_bits(bn_number) << "-bit prime:" << endl;
					cout << "  n = " << out << endl; 
				}
				return bn_number;
			}
		}
	}
	
	/* step 2 */
	const int m = 20;
	/* step 3 */
	
	double r;
	
	/* step 4 */
	if(k <= 2*m){
		r = 0.5;
		cout << "  step 4, r = " << round(100*r) << "%" << endl;
	}else{
		while(1){
			unsigned char tempc = RndByte(rndfile);
			r = tempc / 255.0;
			r = 0.5 + r / 2.0;
			if (k*(1-r) > m){
				cout << "  step 4: random byte = " << (int)tempc << ", r = " << round(100*r) << "%" << endl;
				break;
			} 
		}
	}
	
	
	/* step 5 */
	level++;
	BIGNUM *bn_q = Maurer((floor(r*k)+1));
	level--;
	out = BN_bn2dec(bn_q);
	cout << "Maurer: back to level " << level << ", k=" << k << ", q=" << out << endl;
	
	/* step 6 */
	int num_bits_in_q  = BN_num_bits(bn_q);
	BIGNUM *I = BN_new();
	
	BIGNUM *bn_a = BN_new();
	BN_set_word(bn_a,2);
	
	BIGNUM *bn_p = BN_new();
	BN_set_word(bn_p,k-2);
	
	BN_CTX *ctx = BN_CTX_new();
	
	/* I = floor(2^(k-2) / bn_q) */
	BN_exp(I, bn_a, bn_p, ctx);
	BN_div(I, NULL, I, bn_q, ctx);
	
	/* step 7 */
	int itr = 1;
	while(1){
		/* step 7.1 */
		BIGNUM *bn_R = RndOddNum(k+1-num_bits_in_q, rndfile);
		
		/*  R = (R mod I) + I + 1 */
		BN_mod(bn_R, bn_R, I, ctx);
		
		BN_add(bn_R, bn_R, I);
		
		BN_ULONG one = 1;
		BN_ULONG two = 2;
		
		BN_add_word(bn_R, one);

		/* n = 2Rq+1 */
		BN_mul(bn_number, bn_R, bn_q, ctx);
		BN_mul_word(bn_number, two);
		BN_add_word(bn_number, one);
		
		out = BN_bn2dec(bn_R);
		cout << "  step 7, itr " << itr << ": R = " << out << ", n = ";
		out = BN_bn2dec(bn_number);
		cout << out << endl;
		
		/* step 7.2 */
		BIGNUM *bn_a = BN_new();
		BIGNUM *bn_b = BN_new();
		BIGNUM *bn_one = BN_new();
		BN_one(bn_one);
		BIGNUM *bn_two = BN_new();
		BN_set_word(bn_two, 2);
		
		BIGNUM *bn_number_1 = BN_new();
		BN_sub(bn_number_1, bn_number, bn_one);
		
		cout << "    ";
		if (TrailDivision(bn_number, primesfile, true)){
			int num_bits_in_n = BN_num_bits(bn_number);
			
			/* step 7.2.1 */
			do{
				bn_a = RndOddNum(num_bits_in_n, rndfile);
			}while(BN_cmp(bn_a, bn_one) <= 0 || BN_cmp(bn_a, bn_number_1) >= 0);
			out = BN_bn2dec(bn_a);
			cout << "  step 7.2.1, itr " << itr << ": a = " << out << endl;
			
			/* step 7.2.2 */
			BN_mod_exp(bn_b, bn_a, bn_number_1, bn_number, ctx);
			
			if(BN_cmp(bn_b, bn_one) == 0){
				BIGNUM *bn_2R = BN_new();
				BN_mul(bn_2R, bn_R, bn_two, ctx);
				BN_mod_exp(bn_b, bn_a, bn_2R, bn_number, ctx);
				
				BIGNUM *bn_d = BN_new();
				BN_sub_word(bn_b,1);
				BN_gcd(bn_d, bn_b, bn_number, ctx);
				if(BN_cmp(bn_d, bn_one) == 0){
					if(level == 0){
						out = BN_bn2dec(bn_number);
						cout << endl << "Maurer's Algorithm found an "<< BN_num_bits(bn_number) << "-bit prime:" << endl;
						cout << "  n = " << out << endl; 
					}
					return bn_number;
				}
			}
		}
		itr++;
	}
}

void maurer(char *numbitsstr, char *primesfilestr, char *rndfilestr){
	/* read the numbits */
	if( numbitsstr == NULL || (*numbitsstr) == '\0' ){
		errorhandle("malformed numbits!");
	}
		
	int k = atoi(numbitsstr);
	
	/* read the primesfile */
	primesfile = primesfilestr;
	if(primesfilestr == NULL || *primesfilestr == '\0'){
		errorhandle("malformed command!");
	}

	/* read the rndfile */
	rndfile = fopen(rndfilestr,"rb");
	if(rndfile == NULL){
		errorhandle("malformed rndfile!");
	}	
	Maurer(k);

	fclose(rndfile);
	
}

unsigned char RndByte(FILE *rndfile){
	return fgetc(rndfile);
}
