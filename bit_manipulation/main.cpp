#include "main.h"
using namespace std;

int main(int argc, char* args[])
{
	ifstream in;
	
	if(argc <= 1){
		cerr<<"malformed command";
		exit(1);
	}
	
	if(strcmp(args[1],"hexdump")!= 0 && strcmp(args[1],"enc-base64")!=0 && strcmp(args[1],"dec-base64")!=0){
		cerr<<"unknown command";
		exit(1);
	}
	
	if(argc == 3){//with filename
		in.open(args[2],ios::binary);
		if (in.fail()) {
                cerr << "Cannot open " << args[1] << " for reading." << endl;
                return 0;
		}
		if(strcmp(args[1], "hexdump") == 0){
			hexdump(in);
		}else if(strcmp(args[1], "enc-base64") == 0){
			encode64(in);
		}else if(strcmp(args[1], "dec-base64") == 0){
			decode64(in);
		}
		in.close();
	}else{//with stdin
		if(strcmp(args[1], "hexdump") == 0){
			hexdump(cin);
		}else if(strcmp(args[1], "enc-base64") == 0){
			encode64(cin);
		}else if(strcmp(args[1], "dec-base64") == 0){
			decode64(cin);
		}
	}
	return 0;
}



