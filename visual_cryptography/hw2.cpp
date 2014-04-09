#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include "hw2.h"
#define STDOUT 1
using namespace std;


int main(int argc, char *argv[])
{	
	argc--,argv++;
	if(argc <= 0){
		cerr << "error" << endl;
		exit(1);
	}
	if(strcmp(*argv,"stream") == 0){//stream
		char* pphrase = NULL ;
		int len=0;
		
		if(argc < 3 || argc > 3){
			cerr << "malformed command" << endl;
			exit(1);
		}
		char *pos;
		char *temp = NULL;
		for(argc--, argv++; argc > 0; argc--, argv++){
			if((pos=strstr(*argv,"-p="))!=NULL){//passphrase for key generation
				pos += 3;
				pphrase = new char[strlen(pos)+1];
				strcpy(pphrase,pos);
			}else if((pos=strstr(*argv,"-l="))!=NULL){//number of bytes to output
				pos += 3;
				temp = new char[strlen(pos)+1];
				strcpy(temp,pos);
				len = atoi(temp);
			}else{
				cerr << "wrong command" << endl;
				exit(1);
			}
		}
		streamcipher(pphrase, len, STDOUT, NULL);//output to stdout
		delete[] pphrase;
		delete[] temp;
	}else if(strcmp(*argv,"encrypt") == 0){//encrypt
		char* pphrase = NULL ;
		char* out = NULL;
		ifstream pbmfile;
		bool flag = true;
		
		if(argc < 3 || argc > 4){
			cerr << "malformed command" << endl;
			exit(1);
		}
		char *pos;
		for(argc--, argv++; argc > 0; argc--, argv++){
			if((pos=strstr(*argv,"-p="))!=NULL){//passphrase for key generation
				pos += 3;
				pphrase = new char[strlen(pos)+1];
				strcpy(pphrase,pos);
			}else if((pos=strstr(*argv,"-out="))!=NULL){
				pos += 5;
				out = new char[strlen(pos)+1];
				strcpy(out,pos);
			}else{
				pbmfile.open(*argv, ios::binary);
				if(pbmfile.fail()){//specify a filename
					cerr << "can't open the file" << endl;
					exit(1);
				}
				encrypt(pphrase, out, pbmfile);
				pbmfile.close();
				flag = false;
			}
		}
		if(flag == true){
			encrypt(pphrase, out, cin);
		}
		delete[] pphrase;
		delete[] out;
	}else if(strcmp(*argv,"merge") == 0){
		char *pmfile1, *pmfile2;
		if(argc < 3 || argc > 3){
			cerr << "malformed command" << endl;
			exit(1);
		}
		argv++;
		pmfile1 = new char[strlen(*argv)+1];
		strcpy(pmfile1,*argv);
		argv++;
		pmfile2 = new char[strlen(*argv)+1];
		strcpy(pmfile2,*argv);
		merge(pmfile1,pmfile2);
		delete[] pmfile1;
		delete[] pmfile2;
	}else if(strcmp(*argv,"decrypt") == 0){
		char* filename;
		ifstream pmfile;
		
		if(argc < 1 || argc > 2){
			cerr << "malformed command" << endl;
			exit(1);
		}
		if(argc == 2){//with pmfile
			argv++;
			filename = new char[strlen(*argv)+1];
			strcpy(filename,*argv);
			pmfile.open(filename,ios::binary);
			if(pmfile.fail()){
				cerr << "fail to open '" << filename << "'" <<endl;
				exit(1);
			}
			decrypt(pmfile);
			pmfile.close();
		}else{
			decrypt(cin);
		}
		delete[] filename;
	}else{
		cerr << "no commands found!"<<endl;
		exit(1);
	}

	return 0;
}
