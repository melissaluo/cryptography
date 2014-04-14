#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include "hw6.h"

using namespace std;
unsigned char inv_arr[16];
extern unsigned char key_arr[16*(Nr+1)];
extern unsigned char INVP_table[P_NUM];

void invinputcheck(istream& in){
	int byte_count = 0;
	while(!in.eof()){
		inv_arr[byte_count++] = in.get();
	}
	
	if(byte_count != 17){
		errorhandle("malformed input!");
	}
}

/* do xor on in_arr with key_arr[round_no*4*4, (round_no+1)*4*4-1] */
void invAddRoundKey(int round_no){
	for(int i=0; i<16;i++){
		inv_arr[i] ^= key_arr[16*round_no + i];
	}
}

/* output 16 bytes a time */
void invoutputword(unsigned char *w, int round_no, const char *step_log){
	cout << "round[" << setfill(' ') << setw(2) << dec << round_no << "]." << step_log;
	if(strcmp(step_log,"ioutput") == 0){
		cout << "  ";
	}else{
		cout << "   ";
	}
	for(int i=0;i<16;i++){
		cout << setfill('0') << setw(2) << hex << (int)w[i];
	}
	cout << endl;
}

void invSubBytes(){
	for(int i=0; i<16; i++){
		inv_arr[i] = invsbox(inv_arr[i]);
	}
}

/* only circularly shift the last 3 rows in the opposite direction */
void invShiftRows(){
	unsigned char temp[16];
	memcpy(temp,inv_arr,16);

	int shift_time = 0;
	for(int i=0;i<16;i++){
		if((shift_time = i%4) != 0){
			inv_arr[i] = temp[(16+i-4*shift_time)%16];
		}
	}	
}

/* ****************************************
 * coefficient matrix:
 * 0321
 * 1032
 * 2103
 * 3210
 * ****************************************/
void invMixColumns(){
	unsigned char temp[16];
	memcpy(temp,inv_arr,16);
	
	/* P_table[0] store coefficient of x^3 */
	unsigned char c3 = INVP_table[0];
	unsigned char c2 = INVP_table[1];
	unsigned char c1 = INVP_table[2];
	unsigned char c0 = INVP_table[3];
	
	for(int i=0;i<4;i++){
		inv_arr[4*i] = bigdotbyte(c0, temp[4*i]) ^ bigdotbyte(c3, temp[4*i+1]) ^ bigdotbyte(c2, temp[4*i+2]) ^ bigdotbyte(c1, temp[4*i+3]);
		inv_arr[4*i+1] = bigdotbyte(c1, temp[4*i]) ^ bigdotbyte(c0, temp[4*i+1]) ^ bigdotbyte(c3, temp[4*i+2]) ^ bigdotbyte(c2, temp[4*i+3]);
		inv_arr[4*i+2] = bigdotbyte(c2, temp[4*i]) ^ bigdotbyte(c1, temp[4*i+1]) ^ bigdotbyte(c0, temp[4*i+2]) ^ bigdotbyte(c3, temp[4*i+3]);
		inv_arr[4*i+3] = bigdotbyte(c3, temp[4*i]) ^ bigdotbyte(c2, temp[4*i+1]) ^ bigdotbyte(c1, temp[4*i+2]) ^ bigdotbyte(c0, temp[4*i+3]);
	}
}

void aesdecrypt(char *key, char *tablename, istream& in){
	int round_no = 0;
	/* check input bytes */
	invinputcheck(in);
	invoutputword(inv_arr, round_no, "iinput");
	
	/* do key expansion */
	keyexpand(key,tablename,0);
	invoutputword(&key_arr[(Nr-round_no)*16], round_no, "ik_sch");
	
	invAddRoundKey(Nr-round_no);
	
	for(round_no=1;round_no <= Nr;round_no++){
		invoutputword(inv_arr, round_no, "istart");
		
		invShiftRows();
		invoutputword(inv_arr, round_no, "is_row");
		
		invSubBytes();
		invoutputword(inv_arr, round_no, "is_box");
		
		invoutputword(&key_arr[(Nr-round_no)*16], round_no, "ik_sch");
		invAddRoundKey(Nr-round_no);
		
		if(round_no < Nr){
			invoutputword(inv_arr, round_no, "ik_add");
			invMixColumns();
		}
	}
	invoutputword(inv_arr, round_no-1, "ioutput");
}
