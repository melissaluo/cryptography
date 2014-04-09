#include "main.h"
using namespace std;
int numcount=0;
static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

void processencode64(char* buffer, int size){
	int array_4[4];
	
	//complete the three bytes
	for(int i=size; i<3 ; i++){
		buffer[i] = '\0';
	}
	
	//convert three btyes to four 
	array_4[0] = (buffer[0]&0xfc) >> 2;
	array_4[1] = ((buffer[0]&0x03) << 4) + ((buffer[1]&0xf0) >> 4);
	array_4[2] = ((buffer[1]&0x0f) << 2) + ((buffer[2]&0xc0) >> 6);
	array_4[3] = buffer[2]&0x3f;
	
	for(int i=0;i<4;i++){
		if(numcount == 64){
			cout << endl;
			numcount=0;
		}
		if(size == 2 && i == 3){
			cout<<"=";
		}else if(size == 1 && (i == 2 || i == 3)){
			cout<<"=";
		}else{
			cout<<base64_chars[array_4[i]];
		}
		numcount++;
	}
}

void encode64(istream& in){
	char buffer[3];
	int i=0;
	
	while(!in.eof()){
		i = 0;
		memset(buffer,'\0',3);
		while(i<3){
			in.get(buffer[i]);
			if(in.eof()){
				break;
			}else{
				i++;
			}
		}
		if(i!=0){
			processencode64(buffer,i);
		}
	}
	if(numcount!=0){
		cout << endl;
	}
}
