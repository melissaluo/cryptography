#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

void merge(char* filename1, char* filename2){
	ifstream file1;
	ifstream file2;
	
	file1.open((const char*)filename1, ios::binary);
	file2.open((const char*)filename2, ios::binary);
	
	if(file1.fail()){
		cerr << "fail to open pmfile1" << endl;
		exit(1);
	}
	if(file2.fail()){
		cerr << "fail to open pmfile2" << endl;
		exit(1);
	}
	
	string id;//format
	string comment;//comment
	int width = 0;
	int height = 0;
	
	if(!getline(file1,id) || (id != "P4")){//read the format
		cerr << "wrong format" << endl;
		exit(1);
	}
	if(file1.peek() == '#' && !getline(file1,comment)){//read next character, if'#', it should be comment
		cerr << "wrong format"<<endl;
		exit(1);
	}
	if(!(file1 >> width >> height)){//read width and height
		cerr << "wrong format"<<endl;
		exit(1);
	}
	
	if(!getline(file2,id) || (id != "P4")){//read the format
		cerr << "wrong format" << endl;
		exit(1);
	}
	if(file2.peek() == '#' && !getline(file1,comment)){//read next character, if'#', it should be comment
		cerr << "wrong format"<<endl;
		exit(1);
	}
	if(!(file2 >> width >> height)){//read width and height
		cerr << "wrong format"<<endl;
		exit(1);
	}

	file1.get();//skip the newline
	file2.get();
	
	int bytewidth = (width/8);
	if(width%8 != 0){
		bytewidth += 1;
	}
	unsigned char* img1 = new unsigned char[bytewidth];
	unsigned char* img2 = new unsigned char[bytewidth];
	unsigned char* mergeimg = new unsigned char[bytewidth];
	
	cout << "P4" << endl;
	cout << width << " " << height << endl;
	
	for(int i=0; i<height; i++){		
		file1.read((char *)img1,bytewidth);
		file2.read((char *)img2,bytewidth);
		
		for(int j=0 ; j<bytewidth; j++){
			mergeimg[j] = img1[j] | img2[j];
		}
		
		cout.write((char *)mergeimg,bytewidth);
		//cout << mergeimg;
	}
	
	delete[] img1;
	delete[] img2;
	delete[] mergeimg;
	file1.close();
	file2.close();
}
