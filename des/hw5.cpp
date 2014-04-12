#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "hw5.h"

using namespace std;


void errorhandle(const char* msg){
	cerr << msg << endl;
	exit(1);
}

int main(int argc, char **argv)
{
	argc--,argv++;
	int flag = 0;//1: encrypt;2: decrypt;3: encrypt3;4: decrypt3
	
	if(argc < 2){
		errorhandle("malformed command!");
	}
   
	if(strcmp(*argv,"tablecheck") == 0){	/*hw5 tablecheck -t=tablefile*/
		argc--,argv++;
		char* pos;
		
		if((pos=strstr(*argv,"-t="))!=NULL){
			pos += 3;
			Table table;
			table.tableCheck(pos);
			return 0;
		}else{
			errorhandle("malformed command!");
		}
	}else if(strcmp(*argv,"encrypt") == 0){	/*hw5 encrypt -k=key -t=tablefile [file]*/
		flag = 1;
	}else if(strcmp(*argv,"decrypt") == 0){	/*hw5 decrypt -k=key -t=tablefile [file]*/
		flag = 2;
	}else if(strcmp(*argv,"encrypt3") == 0){ /*hw5 encrypt3 -k=key3 -t=tablefile [file]*/
		flag = 3;
	}else if(strcmp(*argv,"decrypt3") == 0){ /*hw5 decrypt3 -k=key3 -t=tablefile [file]*/
		flag = 4;
	}else{
		errorhandle("malformed command!");
	}

	char* pos;
	char* key;
	char* tablefile;
	ifstream in;
	int argu = argc;
	
	if(argc < 3 || argc > 4){
		errorhandle("malformed command!");
	}
	
	for(argc--,argv++;argc>0;argc--,argv++){
		if((pos=strstr(*argv,"-k="))!=NULL){
			pos += 3;
			key = new char[strlen(pos)+1];
			strncpy(key,pos,strlen(pos)+1);
		}else if((pos=strstr(*argv,"-t="))!=NULL){
			pos += 3;
			tablefile = new char[strlen(pos)+1];
			strncpy(tablefile,pos,strlen(pos)+1);
		}else{
			if(argu == 4){/* get input from file*/
				in.open(*argv, ios::binary);
				if(in.fail()){
					errorhandle("fail to open file!");
				}
				switch(flag){
					case 1:encrypt(key,tablefile,in);break;
					case 2:decrypt(key,tablefile,in);break;
					case 3:encrypt3(key,tablefile,in);break;
					case 4:decrypt3(key,tablefile,in);break;
					default:break;
				}
				in.close();	
			}else{
				errorhandle("malforme command!");
			}
		}
					
	}

	switch(flag){
		case 1:encrypt(key,tablefile,cin);break;
		case 2:decrypt(key,tablefile,cin);break;
		case 3:encrypt3(key,tablefile,cin);break;
		case 4:decrypt3(key,tablefile,cin);break;
		default:break;
	}

	delete[] key;
	delete[] tablefile;
	
	return 0;
}
