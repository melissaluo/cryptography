#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

void decrypt(istream &f){
	string id;//format
	string comment;//comment

	int width = 0;
	int height = 0;
	
	if(!getline(f,id) || (id != "P4")){//read the format
		cerr << "wrong format" << endl;
		exit(1);
	}
	if(f.peek() == '#' && !getline(f,comment)){//read next character, if'#', it should be comment
		cerr << "wrong format"<<endl;
		exit(1);
	}
	if(!(f >> width >> height)){//read width and height
		cerr << "wrong format"<<endl;
		exit(1);
	}
	
	f.get();//skip the newline
	
	int bytewidth = (width%8 == 0)?(width/8):(width/8+1);
	int outbytewidth = (width/2%8 == 0)?(width/16):(width/16+1);
	int outbyteheight = height/2;
	
	char *inimgU = new char[bytewidth];
	char *inimgD = new char[bytewidth]; 
	char *outimg = new char[outbytewidth];

	int index = 0;
	int counter = 0;
	
	cout << "P4" << endl;
	cout << width/2 << " " << height/2 << endl;
	
	for(int i=0;i<outbyteheight;i++){
		f.read(inimgU,bytewidth);//read two rows from merged file,but the first row is enough
		f.read(inimgD,bytewidth);
		index = counter = 0;
		memset(outimg,0,outbytewidth);
		
		for(int j=0;j<bytewidth;j++){//process every byte of merged file
			
			char currentbyte = inimgU[j];

			for(int m=7; m>=0;){
				
				if(counter == 8){
					index ++;
					counter = 0;
				}
				if(((currentbyte>>m) & (currentbyte >> (m-1)))&0x01 ){//if do & on adjacent two bits equals to 0, output 0, otherwise, output 1 
					outimg[index] += 1 << (7-counter); 
				}else{
					outimg[index] += 0 << (7-counter);
				}
				counter++;
				m -= 2;
			}
		}//process a row
		
		//output a row into stdout
		cout.write(outimg,outbytewidth);
	}
	
	delete[] inimgU;
	delete[] inimgD;
	delete[] outimg;
}
