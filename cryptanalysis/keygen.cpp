#include <iostream>
#include <fstream>
#include <stdlib.h>/*srand,rand*/
#include <sys/time.h>/*gettimeofday*/
#include <string.h>/*strcyp*/
#include <vector>/*vector>*/
#include "hw4.h"
using namespace std;

void swap(char* a, char* b){
	char temp = *a;
	*a = *b;
	*b = temp;
}

/*find a random index from 0 to m, inclusive*/
int uniform(int m){
	/*get a random seed every time run this*/
	timeval time;
	gettimeofday(&time,NULL);
	srand(time.tv_sec*1000 + time.tv_usec/1000);
	
	return (rand() % (m+1));
	
}

void perputationgenerator(char *key){
	int j=0;
	for(int i=0;i<26;i++){
		j = uniform(i);
		swap(&key[i],&key[j]);
	}
}


//void keygen(int T, int mode, char* keyfile){
void keygen(int T, int mode, vector<string> keyfile){
	/*one line key is a perputation of 26 character*/
	char *keyline = new char[26];
	strcpy(keyline,"abcdefghijklmnopqrstuvwxyz");

	for(int i=0; i<T; i++){		
		perputationgenerator(keyline);
		if(mode == STDOUT){
			cout << keyline << endl;
		}else if(mode == CIPHER){
			string buffer(keyline);
			keyfile.push_back(buffer);
		}
	}
	
	delete[] keyline;
}

void invkey(char* keyfilename){
	ifstream keyfile;
	
	keyfile.open(keyfilename,iostream::in);
	
	if(keyfile.fail()){
		errorhandle("fail to open keyfile");
	}
	
//	/*read every line of the keyfile*/
//	char* keybuffer = new char[26];
//	/*inverse key line*/
//	char* outbuffer = new char[26];
	
//	while(keyfile.read(keybuffer,27)){
//		
//		for(int i = 0 ; i< 26 ;i ++){
//			outbuffer[keybuffer[i] - 'a']='a' + i;
//		}
//		cout << outbuffer << endl;
//	}
	
	char* keybuffer = new char[27];
	char* outbuffer = new char[26];
	
	keyfile.getline(keybuffer,27);
	while(!keyfile.eof() && !keyfile.bad()){
		if(keyfile.gcount()!=27){
			errorhandle("malformed keyfile!");
		}
		for(int i = 0 ; i< 26 ;i ++){
			if(keybuffer[i]>= 'a' && keybuffer[i]<='z'){
				outbuffer[keybuffer[i] - 'a']='a' + i;
			}else{
				errorhandle("malformed keyfile!");
			}
		}
		cout << outbuffer << endl;
		keyfile.getline(keybuffer,27);
	}
	
	
	delete[] keybuffer;
	delete[] outbuffer;
	keyfile.close();
}
