#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include "hw6.h"

using namespace std;
unsigned char in_arr[16];
extern unsigned char key_arr[16*(Nr+1)];
extern unsigned char P_table[P_NUM];

void inputcheck(istream& in){
	int byte_count = 0;
	while(!in.eof()){
		in_arr[byte_count++] = in.get();
	}
	
	if(byte_count != 17){
		errorhandle("malformed input!");
	}
}

/* do xor on in_arr with key_arr[round_no*4*4, (round_no+1)*4*4-1] */
void AddRoundKey(int round_no){
	for(int i=0; i<16;i++){
		in_arr[i] ^= key_arr[16*round_no + i];
	}
}

/* substition in byte level */
void SubBytes(){
	for(int i=0; i<16; i++){
		in_arr[i] = sbox(in_arr[i]);
	}
}

/* only circularly shift the last 3 rows */
void ShiftRows(){
	unsigned char temp[16];
	memcpy(temp,in_arr,16);

	int shift_time = 0;
	for(int i=0;i<16;i++){
		if((shift_time = i%4) != 0){
			in_arr[i] = temp[(i+4*shift_time)%16];
		}
	}	
}

/* ****************************************
 * example:
 * a(x) = {03}x^3 + {01}x^2 + {01}x + {02} 
 * s'(x) = a(x) xor s(x)
 * 
 * {02} {03} {01} {01}
 * {01} {02} {03} {01}
 * {01} {01} {02} {03}
 * {03} {01} {01} {02}
 * 
 * c3 = {03} 
 * c2 = {01}
 * c1 = {01}
 * c0 = {02}
 * 
 * coefficient matrix:
 * 0321
 * 1032
 * 2103
 * 3210
 * ****************************************/
void MixColumns(){
	unsigned char temp[16];
	memcpy(temp,in_arr,16);
	
	/* P_table[0] store coefficient of x^3 */
	unsigned char c3 = P_table[0];
	unsigned char c2 = P_table[1];
	unsigned char c1 = P_table[2];
	unsigned char c0 = P_table[3];
	
	for(int i=0;i<4;i++){
		in_arr[4*i] = bigdotbyte(c0, temp[4*i]) ^ bigdotbyte(c3, temp[4*i+1]) ^ bigdotbyte(c2, temp[4*i+2]) ^ bigdotbyte(c1, temp[4*i+3]);
		in_arr[4*i+1] = bigdotbyte(c1, temp[4*i]) ^ bigdotbyte(c0, temp[4*i+1]) ^ bigdotbyte(c3, temp[4*i+2]) ^ bigdotbyte(c2, temp[4*i+3]);
		in_arr[4*i+2] = bigdotbyte(c2, temp[4*i]) ^ bigdotbyte(c1, temp[4*i+1]) ^ bigdotbyte(c0, temp[4*i+2]) ^ bigdotbyte(c3, temp[4*i+3]);
		in_arr[4*i+3] = bigdotbyte(c3, temp[4*i]) ^ bigdotbyte(c2, temp[4*i+1]) ^ bigdotbyte(c1, temp[4*i+2]) ^ bigdotbyte(c0, temp[4*i+3]);
	}
}

/* output 16 bytes a time */
void outputword(unsigned char *w, int round_no, const char *step_log){
	cout << "round[" << setfill(' ') << setw(2) << dec << round_no << "]." << step_log;
	if(strcmp(step_log,"output") == 0){
		cout << "   ";
	}else{
		cout << "    ";
	}
	for(int i=0;i<16;i++){
		cout << setfill('0') << setw(2) << hex << (int)w[i];
	}
	cout << endl;
}

void aesencrypt(char *key, char *tablename, istream& in){
	int round_no = 0;
	/* check input bytes */
	inputcheck(in);
	outputword(in_arr, round_no, "input");
	
	/* do key expansion */
	keyexpand(key,tablename,0);
	outputword(&key_arr[16*round_no], round_no, "k_sch");
	
	AddRoundKey(round_no);
	
	for(round_no=1;round_no <= Nr;round_no++){
		outputword(in_arr, round_no, "start");
		
		SubBytes();
		outputword(in_arr, round_no, "s_box");
		
		ShiftRows();
		outputword(in_arr, round_no, "s_row");
		
		if(round_no < Nr){
			MixColumns();
			outputword(in_arr, round_no, "m_col");
		}
		
		outputword(&key_arr[16*round_no], round_no, "k_sch");
		AddRoundKey(round_no);
	}
	outputword(in_arr, round_no-1, "output");
}
