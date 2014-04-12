#include <iostream>
#include <fstream>
#include <string.h>
#include "hw4.h"
using namespace std;

void crypt(char* keyfilename,istream&f){
	char c;
	
	//open keyfile
	fstream keyfile;
	keyfile.open(keyfilename,iostream::in);
	if(keyfile.fail()){
		errorhandle("fail to open keyfile");
	}
	
	char* keybuffer = new char[27];//one line key
	
	if(!keyfile.getline(keybuffer,27)){
		keyfile.clear();
		keyfile.seekg(0,keyfile.beg);
		keyfile.getline(keybuffer,27);
	}
	
	if(keyfile.gcount()!= 27){
		keyfile.close();
		errorhandle("malformed keyfile!");
	}


	while((c = f.get())!=EOF){
		
		if(c >= 'a' && c <= 'z'){//lower case english character, encryt with the (j mod T) th permutation function
			if(keybuffer[c-'a']>= 'a' && keybuffer[c-'a']<='z'){
				cout << keybuffer[c-'a'];
			}else{
				errorhandle("malformed keyfile!");
			}
		}else{//not encrypted
			cout << c;
		}
		
		if(!keyfile.getline(keybuffer,27)){
			keyfile.clear();
			keyfile.seekg(0,keyfile.beg);
			keyfile.getline(keybuffer,27);
		}
		
		if(keyfile.gcount()!= 27){
			keyfile.close();
			errorhandle("malformed keyfile!");
		}
		
	}
	
	delete[] keybuffer;
	keyfile.close();
}
