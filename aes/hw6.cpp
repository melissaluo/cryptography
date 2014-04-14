/******************************************
	 hw6 tablecheck -t=tablefile
    hw6 modprod -p1=poly1 -p2=poly2
    hw6 keyexpand -k=key -t=tablefile
    hw6 encrypt -k=key -t=tablefile [file]
    hw6 decrypt -k=key -t=tablefile [file]
    hw6 inverse -p=poly
********************************************/ 
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "hw6.h"

using namespace std;

void errorhandle(const char* msg){
	cerr << msg << endl;
	exit(1);
}


int main(int argc, char **argv){
	
	argc--,argv++;
	
	if(argc < 2){
		errorhandle("malformed command!");
	}
	
	if(strcmp(*argv,"tablecheck") == 0){	/*hw6 tablecheck -t=tablefile*/
		argc--,argv++;
		char* pos;
		
		if((pos=strstr(*argv,"-t="))!=NULL){
			pos += 3;
			tableCheck(pos);
			return 0;
		}else{
			errorhandle("malformed command!");
		}
	}else if(strcmp(*argv,"modprod") == 0){	/*hw6 modprod -p1=poly1 -p2=poly2*/
		char* pos;
		char *p1, *p2;
		
		if( argc != 3){
			errorhandle("malformed command!");
		}
		
		for(argc--,argv++;argc>0;argc--,argv++){
			if((pos=strstr(*argv,"-p1="))!=NULL){
				pos += 4;
				p1 = new char[strlen(pos)+1];
				strncpy(p1,pos,strlen(pos)+1);
			}else if((pos=strstr(*argv,"-p2="))!=NULL){
				pos += 4;
				p2 = new char[strlen(pos)+1];
				strncpy(p2,pos,strlen(pos)+1);
			}else{
				errorhandle("malformed command!");
			}
		}
		
		modprod(p1,p2);
		delete[] p1;
		delete[] p2;
		return 0;
	}else if(strcmp(*argv,"keyexpand") == 0){	/*hw6 keyexpand -k=key -t=tablefile*/
		char* pos;
		char *key, *tablename;
		
		if( argc != 3){
			errorhandle("malformed command!");
		}
		
		for(argc--,argv++;argc>0;argc--,argv++){
			if((pos=strstr(*argv,"-k="))!=NULL){
				pos += 3;
				key = new char[strlen(pos)+1];
				strncpy(key,pos,strlen(pos)+1);
			}else if((pos=strstr(*argv,"-t="))!=NULL){
				pos += 3;
				tablename = new char[strlen(pos)+1];
				strncpy(tablename,pos,strlen(pos)+1);
			}else{
				errorhandle("malformed command!");
			}
		}
		
		keyexpand(key,tablename,1);
		delete[] key;
		delete[] tablename;
		return 0;
	}else if(strcmp(*argv,"encrypt") == 0){ /*hw6 encrypt -k=key -t=tablefile [file]*/
		int argu = argc;
		if( argc < 3 || argc > 4){
			errorhandle("malformed command!");
		}
		
		char *pos;
		char *key, *tablename;
		ifstream in;
		
		for(argc--,argv++;argc>0;argc--,argv++){
			if((pos=strstr(*argv,"-k="))!=NULL){
				pos += 3;
				key = new char[strlen(pos)+1];
				strncpy(key,pos,strlen(pos)+1);
			}else if((pos=strstr(*argv,"-t="))!=NULL){
				pos += 3;
				tablename = new char[strlen(pos)+1];
				strncpy(tablename,pos,strlen(pos)+1);
			}else{
				if(argu == 4){/* get input from file */
					in.open(*argv, ios::binary);
					if(in.fail()){
						errorhandle("fail to open file!");
					}
					aesencrypt(key,tablename,in);
					delete[] key;
					delete[] tablename;
					in.close();
					return 0;
				}else{
					errorhandle("malformed command!");
				}
			}
		}
		
		aesencrypt(key,tablename,cin);
		delete[] key;
		delete[] tablename;
		return 0;
		
	}else if(strcmp(*argv,"decrypt") == 0){ /*hw6 decrypt -k=key -t=tablefile [file]*/
		int argu = argc;
		if( argc < 3 || argc > 4){
			errorhandle("malformed command!");
		}
		
		char *pos;
		char *key, *tablename;
		ifstream in;
		
		for(argc--,argv++;argc>0;argc--,argv++){
			if((pos=strstr(*argv,"-k="))!=NULL){
				pos += 3;
				key = new char[strlen(pos)+1];
				strncpy(key,pos,strlen(pos)+1);
			}else if((pos=strstr(*argv,"-t="))!=NULL){
				pos += 3;
				tablename = new char[strlen(pos)+1];
				strncpy(tablename,pos,strlen(pos)+1);
			}else{
				if(argu == 4){/* get input from file */
					in.open(*argv, ios::binary);
					if(in.fail()){
						errorhandle("fail to open file!");
					}
					aesdecrypt(key,tablename,in);
					delete[] key;
					delete[] tablename;
					in.close();
					return 0;
				}else{
					errorhandle("malformed command!");
				}
			}
		}
		
		aesdecrypt(key,tablename,cin);
		delete[] key;
		delete[] tablename;
		return 0;
		
	}else if(strcmp(*argv,"inverse") == 0){ /*hw6 inverse -p=poly*/

	}else{
		errorhandle("malformed command!");
	}
	
	return 0;
}
