#include "main.h"
using namespace std;

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

bool flag = false;

void processdecode64(char* buffer){
	char array_3[3];
	char temp_4[4];
	int size=0;
	static const size_t npos = -1;
	
	
	if(buffer[0] == '=' || buffer[1] == '='){
		cerr << "'=' appears in wrong place";
		exit(1); 
	}else if(buffer[2] == '=' && buffer[3] == '='){
		flag = true;//buffer contains '=', set flag true
		size = 1;
	}else if(buffer[3] == '=' && buffer[2] != '='){
		flag = true;//buffer contains '=', set flag true
		size = 2;
	}else if(buffer[2] == '=' && buffer[3] != '='){
		cerr << "'=' appears in wrong place";
		exit(1);
	}else{
		size = 3;
	}
	
	/*
	if(buffer[3] == '='){
		if(buffer[2] == '='){//buffer contain two "="
			size = 1;
		}else{//buffer contain onnely 
			size = 2;
		}
	}else{
		size = 3;
	}
	*/
	
	memset(temp_4,'\0',4);
	for(int i=0;i<size+1;i++){
		if((base64_chars.find(buffer[i])== npos) && (buffer[i]!='=')){
			cerr << "There exists characters that can't be decoded";
		}else{
			temp_4[i] = base64_chars.find(buffer[i]);
		}
	}
	
	array_3[0] = (temp_4[0] << 2) + ((temp_4[1]&0x30) >> 4);
	array_3[1] = ((temp_4[1]&0xf) << 4) + ((temp_4[2]&0x3c) >> 2);
	array_3[2] = ((temp_4[2]&0x3) << 6) + (temp_4[3]&0xff);
	
	for(int i=0;i<size;i++){
		cout << array_3[i];
	}
}

void decode64(istream& in){
	char buffer[4];
	char temp;
	int i=0;
	
	while(!in.eof()){
		i=0;
		memset(buffer,'\0',4);
		
		if(flag == true){
			if(!in.eof()){
				in.get(temp);
				if(temp!='\n'){
					cerr << "wrong file format";
					exit(1);
				}
			}
		}
		while(i<4){
			in.get(temp);
			while(temp =='\n' && !in.eof()){
				in.get(temp);
			}
			if(in.eof()){
				break;
			}
			else{
				buffer[i]=temp;
				i++;
			}
		}
		if(i==4){
			processdecode64(buffer);
		}else if(i!=0){
			cerr << "the number of characters in this file should be divided by 4";
			exit(1);
		}
	}
}
