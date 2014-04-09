#include "main.h"
using namespace std;
void processhedxump(char *buffer, int offset, int size){
	cout<<hex<<setfill('0')<<setw(6)<<offset<<": ";

	for(int i=0;i<size;i++){
		int z = buffer[i]&0xff;
		cout<<hex<<setfill('0')<<setw(2)<<z<<" ";
		if(i==7 || i==15)
			cout<<" ";
	}
	for(int j=size;j<16;j++){
		cout<<"-- ";
		if(j==7 || j==15)
			cout<<" ";
	}
	for(int i=0;i<size;i++){
		int z = buffer[i]&0xff;
		if(z<=0x1f || z==0x7f){
			cout<<".";
		}else if(z>=0x80){
			cout<<"~";
		}else if(z>=0x20 && z<=0x7e){
			cout<<buffer[i];
		}
	}
	for(int j=size;j<16;j++){
		cout<<" ";
	}
	cout<<endl;
}

void hexdump(istream& in){
	//int wait=0;
	int i=0;
	char buffer[16];
	int offset=0;

	while(!in.eof()){
		i=0;
		memset(buffer,'\0',16);
		while(i<16){
			in.get(buffer[i]);
			if(in.eof()){
				break;
			}else{
				i++;
			}
		}
		processhedxump(buffer,offset,i);
		offset+=16;
	}
}

