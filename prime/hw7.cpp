/*********************************************************************
    hw7 primes -n=maxval
    hw7 trialdiv -n=number -p=primesfile
    hw7 millerrabin -n=number -t=maxitr -p=primesfile
    hw7 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile
    hw7 maurer -k=numbits -p=primesfile -r=rndfile
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "hw7.h"

using namespace std;

void errorhandle(const char* msg){
	cerr << msg << endl;
	exit(1);
}

int main(int argc, char **argv)
{
	argc--,argv++;
	
	if(argc < 2){
		errorhandle("malformed command!");
	}
	
	if(strcmp(*argv,"primes") == 0){	/*hw7 primes -n=maxval*/
	/* Generate all prime numbers less than or equal to maxval */
		argc--,argv++;
		char* pos;
		
		if((pos=strstr(*argv,"-n="))!=NULL){
			pos += 3;
			primeGenerator(pos);
			return 0;
		}else{
			errorhandle("malformed command!");
		}
	}else if(strcmp(*argv,"trialdiv") == 0){	/*hw7 trialdiv -n=number -p=primesfile*/
		/* Test the primality of number using trial division by trying sequentially all 
		 * small prime numbers from 2 to floor(sqrt(number)), inclusive */
		 char *pos;
		 char *number,*primesfile;
		 for(argc--,argv++;argc>0;argc--,argv++){
			if((pos=strstr(*argv,"-n="))!=NULL){
				pos += 3;
				number = new char[strlen(pos)+1];
				strncpy(number,pos,strlen(pos)+1);
			}else if((pos=strstr(*argv,"-p="))!=NULL){
				pos += 3;
				primesfile = new char[strlen(pos)+1];
				strncpy(primesfile,pos,strlen(pos)+1);
			}else{
				errorhandle("malformed command!");
			}
		  }
			
		traildiv(number,primesfile);
		delete[] number;
		delete[] primesfile;
		return 0;
	}else if(strcmp(*argv,"millerrabin") == 0){/*  hw7 millerrabin -n=number -t=maxitr -p=primesfile */
		/* Test the primality of number using Miller-Rabin with security parameter maxitr */
		char *pos;
		char *number, *maxitr, *primesfile;
		
		for(argc--,argv++;argc>0;argc--,argv++){
			if((pos=strstr(*argv,"-n="))!=NULL){
				pos += 3;
				number = new char[strlen(pos)+1];
				strncpy(number,pos,strlen(pos)+1);
			}else if((pos=strstr(*argv,"-t="))!=NULL){
				pos += 3;
				maxitr = new char[strlen(pos)+1];
				strncpy(maxitr,pos,strlen(pos)+1);
			}else if((pos=strstr(*argv,"-p="))!=NULL){
				pos += 3;
				primesfile = new char[strlen(pos)+1];
				strncpy(primesfile,pos,strlen(pos)+1);
			}else{
				errorhandle("malformed command!");
			}
		}
		
		millerrabin(number,maxitr,primesfile);
		delete[] number;
		delete[] maxitr;
		delete[] primesfile;
		return 0;
	}else if(strcmp(*argv,"rndsearch") == 0){/* hw7 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile*/
		char *pos;
		char *numbits, *maxitr, *primesfile, *rndfile;
		
		for(argc--,argv++;argc>0;argc--,argv++){
			if((pos=strstr(*argv,"-k="))!=NULL){
				pos += 3;
				numbits = new char[strlen(pos)+1];
				strncpy(numbits,pos,strlen(pos)+1);
			}else if((pos=strstr(*argv,"-t="))!=NULL){
				pos += 3;
				maxitr = new char[strlen(pos)+1];
				strncpy(maxitr,pos,strlen(pos)+1);
			}else if((pos=strstr(*argv,"-p="))!=NULL){
				pos += 3;
				primesfile = new char[strlen(pos)+1];
				strncpy(primesfile,pos,strlen(pos)+1);
			}else if((pos=strstr(*argv,"-r="))!=NULL){
				pos += 3;
				rndfile = new char[strlen(pos)+1];
				strncpy(rndfile,pos,strlen(pos)+1);
			}else{
				errorhandle("malformed command!");
			}
		}
		
		rndsearch(numbits,maxitr,primesfile,rndfile);
		delete[] numbits;
		delete[] maxitr;
		delete[] primesfile;
		delete[] rndfile;
		return 0;
	
	}else if(strcmp(*argv,"maurer") == 0){ /* hw7 maurer -k=numbits -p=primesfile -r=rndfile */
		/*	Generate a numbits-bit provable prime */
		char *pos;
		char *numbits, *primesfile, *rndfile;
		
		for(argc--,argv++;argc>0;argc--,argv++){
			if((pos=strstr(*argv,"-k="))!=NULL){
				pos += 3;
				numbits = new char[strlen(pos)+1];
				strncpy(numbits,pos,strlen(pos)+1);
			}else if((pos=strstr(*argv,"-p="))!=NULL){
				pos += 3;
				primesfile = new char[strlen(pos)+1];
				strncpy(primesfile,pos,strlen(pos)+1);
			}else if((pos=strstr(*argv,"-r="))!=NULL){
				pos += 3;
				rndfile = new char[strlen(pos)+1];
				strncpy(rndfile,pos,strlen(pos)+1);
			}else{
				errorhandle("malformed command!");
			}
		}
		
		maurer(numbits,primesfile,rndfile);
		delete[] numbits;
		delete[] primesfile;
		delete[] rndfile;
		return 0;	
			
			
	}
}


