#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include "hw7.h"

using namespace std;

/* get a[n] */
int getbit(unsigned char *prime_array, int n){
	int index = n/8;
	int remainder = n%8;
	unsigned char mask = (1 << (8-remainder));
	return ((prime_array[index]&mask) > 0)?1:0;
}

/* n is a multiple of prime, set a[n] to 0 */
void setbit(unsigned char *prime_array, int n){
	int index = n/8;
	int remainder = n%8;
	unsigned char mask = 255 - (1 << (8-remainder));
	prime_array[index] &= mask;
}

void Eratosthenes(int N, unsigned char* prime_array, int bytes_num){
	
	/* initialize this byte array, set every bit to 1 (every number is prime at first) */
	memset(prime_array,255,bytes_num);
	
	/* 1 is not a prime, so set first bit to 0 */
	setbit(prime_array,1);
	
	/* start from 2, do iteration to trike out muliples of primes until square root of maxval */
	int p = 2;
	
	while(p*p <= N){
		int j = p*p;
		
		while(j <= N){
			/* find the index of array, and order of this bit */
			setbit(prime_array, j);
			j = j + p;
		}
		
		p++;
		/* find the next prime */
		while(getbit(prime_array,p) == 0){
			p++;
		}
	}
	
}


void primeGenerator(char *maxval){
	/* read the maxval */
	if( maxval == NULL || (*maxval) == '\0'){
		errorhandle("malformed maxval!");
	}
	
	int N = atoi(maxval);
	if(N < 2 || N > pow(2,24)){
		errorhandle("malformed maxval!");
	}
	
	/* calculate dimension of byte array to represent 1...maxval */
	
	
	/*  considering N = 0 or N = 1 */
	
	int input_bytes_num = (N-1)/8 + 1;
	unsigned char* prime_array = new unsigned char[input_bytes_num];
	
	/* implementing Sieve of Eratosthenes algorithm to find all of the small primes */
	Eratosthenes(N, prime_array, input_bytes_num);
	
	/* output all primes less than maxval using required format  */
	for(int j=3;j>=0;j--){
		int temp = N;
		cout << (char)((temp >> 8*j)&0xff);
	}
	
	for(int i=2;i<=N;i++){
		if(getbit(prime_array,i) == 1){
			for(int j=3;j>=0;j--){
				int temp = i;
				cout << (char)((temp >> 8*j)&0xff);
			}
		}
	}
	
	delete[] prime_array;
}

