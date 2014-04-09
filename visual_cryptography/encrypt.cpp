#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hw2.h"
#define STREAM 0

using namespace std;

void encrypt(char* p, char* name, istream& f){
	string id;//format
	string comment;//comment
	char *keystream;
	int width = 0;
	int height = 0;
	
	if(!getline(f,id) || (id != "P1" && id != "P4")){//read the format
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
	
	//bytewidth and byteheight
	int bytewidth = width / 8;
	int byteheight = height;
	
	//number of padding bits
	int remainder = width % 8;
	
	if(remainder != 0){
		bytewidth += 1;
	}
	
	//generate the key, total bits is width*height
	int keybitnum = (width*height%8 == 0)?(width*height/8):(width*height/8 + 1);
	keystream = new char[keybitnum];
	streamcipher(p, keybitnum, STREAM, keystream);
	
	
	//get every bit of keystream
	char * keystream_ptr=keystream;
	char currentkeybyte= *keystream_ptr;
	int keybits = 0;
	int currentkeybit = 0;
		
	//allocate for output images
	int outbytewidth = (width*2%8 == 0)?(width*2/8):(width*2/8 + 1);
	char* outimg11 = new char[outbytewidth];
	char* outimg12 = new char[outbytewidth];
	char* outimg21 = new char[outbytewidth];
	char* outimg22 = new char[outbytewidth];
	char* inputbuffer = new char[bytewidth];
	int index,counter;
	
	//generate outimg1  and outimg2
	ofstream outfile1, outfile2;
	char *temp = new char[strlen(name)+7];
	memcpy(temp,name,strlen(name));
	strcpy(&temp[strlen(name)],".1.pbm");
	outfile1.open(temp, ofstream::binary);
	
	strcpy(&temp[strlen(name)],".2.pbm");	
	outfile2.open(temp, ofstream::binary);
	delete[] temp;
	//strcpy(temp,name);
//	outfile1.open(strcat(name,".1.pbm"), ofstream::binary);
//	outfile2.open(strcat(temp,".2.pbm"), ofstream::binary);
	
	if(outfile1.fail() || outfile2.fail()){
		cerr << "fail to open file" << endl;
		exit(1);
	}

	if((!(outfile1 << "P4\n")) || (!(outfile2 << "P4\n")) ){
		cerr << "wrong format" << endl;
		exit(1);
	}
	if((!(outfile1 << 2*width << " " << 2*height << "\n")) || (!(outfile2 << 2*width << " " << 2*height << "\n"))){
		cerr << "wrong format" << endl;
		exit(1);
	}
	
	/*********************begin processing data**********************/
	for(int j = 0; j < byteheight; j++){
		memset(outimg11,0,outbytewidth);
		memset(outimg12,0,outbytewidth);
		memset(outimg21,0,outbytewidth);
		memset(outimg22,0,outbytewidth);
		index = counter = 0;

		if(!f.read(inputbuffer, bytewidth)){//read a row 
			cerr << "wrong format" << endl;
			exit(1);
		}
		
		for( int i = 0; i < bytewidth; i++){
			int processbits = 8;
			
			if((remainder!=0) && (i == bytewidth-1)){//width is not divisible by 8, and it's the byte with padding 0
				processbits = remainder;
			}
			
			char currentbyte = inputbuffer[i];
			
			//process one byte
			for(int currentprocessbit = 0; currentprocessbit < processbits; currentprocessbit++){
					
				if(counter == 8){
					index ++;
					counter = 0;
				}
				if(keybits == 8){
//					keystream++;
					keystream_ptr++;
					currentkeybyte = (*keystream_ptr);
//					currentkeybyte = *keystream;
					keybits = 0;
				}
				currentkeybit = (currentkeybyte >> (7-keybits))&0x01;//get the first bit of the keystream
				keybits++;//read one bit from key

				int currentinputbit = (currentbyte >> (7-currentprocessbit))&0x01;
				
				if(currentinputbit == 0){
					if(currentkeybit == 0){
						outimg11[index] += (1 << (7-counter));
						outimg11[index] += (0 << (6-counter));
						outimg12[index] += (0 << (7-counter));
						outimg12[index] += (1 << (6-counter));
						outimg21[index] += (1 << (7-counter));
						outimg21[index] += (0 << (6-counter));
						outimg22[index] += (0 << (7-counter));
						outimg22[index] += (1 << (6-counter));
						counter += 2;
					}else if(currentkeybit == 1){
						outimg11[index] += (0 << (7-counter));
						outimg11[index] += (1 << (6-counter));
						outimg12[index] += (1 << (7-counter));
						outimg12[index] += (0 << (6-counter));
						outimg21[index] += (0 << (7-counter));
						outimg21[index] += (1 << (6-counter));
						outimg22[index] += (1 << (7-counter));
						outimg22[index] += (0 << (6-counter));
						counter += 2;
					}
				}else if(currentinputbit == 1){
					if(currentkeybit == 0){
						outimg11[index] += (1 << (7-counter));
						outimg11[index] += (0 << (6-counter));
						outimg12[index] += (0 << (7-counter));
						outimg12[index] += (1 << (6-counter));
						outimg21[index] += (0 << (7-counter));
						outimg21[index] += (1 << (6-counter));
						outimg22[index] += (1 << (7-counter));
						outimg22[index] += (0 << (6-counter));
						counter += 2;
					}else if(currentkeybit == 1){
						outimg11[index] += (0 << (7-counter));
						outimg11[index] += (1 << (6-counter));
						outimg12[index] += (1 << (7-counter));
						outimg12[index] += (0 << (6-counter));
						outimg21[index] += (1 << (7-counter));
						outimg21[index] += (0 << (6-counter));
						outimg22[index] += (0 << (7-counter));
						outimg22[index] += (1 << (6-counter));
						counter += 2;
					}
				}				
			}
			
		}//finish processing a row
		
	
		if((!outfile1.write(outimg11, outbytewidth)) ||
			(!outfile1.write(outimg12, outbytewidth)) ||
			(!outfile2.write(outimg21, outbytewidth)) ||
			(!outfile2.write(outimg22, outbytewidth))){
			cerr << "wrong format" << endl;
			exit(1);
		}			
	}
	delete[] outimg11;
	delete[] outimg12;
	delete[] outimg21;
	delete[] outimg22;
	delete[] inputbuffer;
	delete[] keystream;

	outfile1.close();
	outfile2.close();
}
