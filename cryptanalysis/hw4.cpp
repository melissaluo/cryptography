#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>/*strstr*/
#include <fstream>

#include "hw4.h"


using namespace std;

void errorhandle(const char* msg){
	cerr << msg << endl;
	exit(1);
}

int main(int argc, char **argv)
{
	
	argc--;argv++;
	if(argc < 1){
		errorhandle("incomplete command!");
	}
	
	if(strcmp(*argv, "keygen") == 0){
		
		char* pos;
		char* temp = NULL;
		
		if(argc < 2){
			errorhandle("incomplete command!");
		}
		
		argv++;
		if((pos = strstr(*argv,"-t="))!=NULL){
			pos += 3;
			int period = atoi(pos);
			if(period <= 0){
				errorhandle("malforme command!");
			}
			vector<string> temp;
			keygen(period,STDOUT,temp);
		}else{
			errorhandle("malforme command!");
		}
		
		delete[] temp;
	}else if(strcmp(*argv, "crypt") == 0){
		
		char* pos;
		char* keyfile = NULL;
		ifstream inputfile;
			
		if(argc < 2 || argc > 3){
			errorhandle("malformed command!");
		}
		
		argv++;
		
		if((pos = strstr(*argv,"-k="))!=NULL){
			pos += 3;
			keyfile = new char[strlen(pos)+1];
			strcpy(keyfile,pos);//keyfile
		}else{
			errorhandle("malforme command!");
		}
		
		if(argc == 3){//from filename
			argv ++;
			
			inputfile.open(*argv, iostream::in);//open the file
			
			if(inputfile.fail()){
				errorhandle("fail to open the file");
			}
			
			crypt(keyfile,inputfile);

			inputfile.close();
		}else{//from stdin
			crypt(keyfile,cin);
		}

		delete[] keyfile;
	}else if(strcmp(*argv, "invkey") == 0){
		if(argc!=2){
			errorhandle("maleformed command!");
		}
		argv++;
		invkey(*argv);
	}else if(strcmp(*argv, "histo") == 0){
		if(argc < 3 || argc > 4){
			errorhandle("malformed command!");
		}
		
		char *pos;
		int period=0;
		int index=0;
		int argu = argc;
		
		for(argc--,argv++;argc>0;argc--,argv++){
			if((pos = strstr(*argv,"-t="))!=NULL){
				pos += 3;
				period = atoi(pos);
				if(period <= 0){
					errorhandle("malforme command!");
				}
			}else if((pos = strstr(*argv,"-i="))!=NULL){
				pos += 3;
				index = atoi(pos);
				if(index <=0 || index > period){
					errorhandle("malforme command!");
				}
			}else{//get input from a file
				/*get input from a file*/
				if(argu == 4){
					fstream file;
					file.open(*argv,iostream::in);
					
					if(file.fail()){
						errorhandle("fail to open file!");
					}
					
					histo(period,index,file);
					file.close();
					return 0;
				}else{
					errorhandle("malformed command!");
				}
			}
		}
		
		histo(period,index,cin);//get input from cin
	}else if(strcmp(*argv, "solve") == 0){
		char *pos;
		int max_t=0;
		if(argc != 3){
			errorhandle("malformed command!");
		}
		argc--;argv++;
		if((pos=strstr(*argv,"-l="))!=NULL){
			pos += 3;
			max_t = atoi(pos);
			if(max_t <= 0){
				errorhandle("malformed command!");
			}
		}else{
			errorhandle("malformed command!");
		}
		argv++;
		
		fstream file;
		file.open(*argv,iostream::in);
		
		if(file.fail()){
			errorhandle("fail to open file!");
		}

		solve(max_t,file);
		file.close();
	}else{
		errorhandle("unknown commad!");
	}

	return 0;
}

