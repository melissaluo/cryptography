#include <string.h>
#include <iomanip>
#include "hw6.h"

extern unsigned char S_table[S_NUM];
extern unsigned char INVS_table[S_NUM];
unsigned char key_arr[16*(Nr+1)];

/* calculate rcon, 2^(i-1), similar to xtime calculation, left-ship followed by a conditional xor */
unsigned char rcon(int i){
	unsigned char result_char = 1;
	
	if(i == 0){
		return 0;
	}
	while(i != 1){
		/* get the first bit */
		unsigned char first_bit = result_char & 0x80;
		/* left shift 1 bit */
		result_char <<= 1;
		/* if first bit equals to 1, do xor with m = 0x1b */
		if(first_bit == 0x80){
			result_char ^= 0x1b;
		}
		i--;
	}
	
	return result_char;
}	

/* byte-level substitution */
unsigned char sbox(unsigned char c){
	return S_table[(int)c];
}

/* use inverse sbox byte-level substitution */
unsigned char invsbox(unsigned char c){
	return INVS_table[(int)c];
}

/* [a0,a1,a2,a3] -> [a1,a2,a3,a0] */
void rotate(unsigned char *in){
	char temp = in[0];
	for(int n=0;n<3;n++){
		in[n] = in[n+1];
	}
	in[3] = temp;
}

/* This is the core key expansion, which, given a 4-byte value, does some scrambling */
void schedule_core(unsigned char *in, int i){
	/* Rotate the input 8 bits to the left */
	rotate(in);
	/* Apply Rijndael's s-box on all 4 bytes */
	for(int n = 0; n < 4; n++) 
		in[n] = sbox(in[n]);
	/* On just the first byte, add 2^i to the byte */
	in[0] ^= rcon(i);
}

void keyExpansion(){
	unsigned char temp[4];
	/* c is 16 because the first sub-key is the user-supplied key */
	int byte_count = 16;
	int i = 1;

	/* We need 11 sets of sixteen bytes each for 128-bit mode */
	while(byte_count < 16*(Nr+1)){
		/* Copy the temporary variable over from the last 4-byte block */
		for(int n=0 ; n<4; n++){
			temp[n] = key_arr[byte_count-4+n];
		}
		if(byte_count % 16 == 0 ){
			schedule_core(temp,i);
			i++;
		}
		for(int n=0;n<4;n++){
			key_arr[byte_count] = key_arr[byte_count - 16] ^ temp[n];
			byte_count ++;
		}
	}
}

void output(){
	for(int i=0;i<4*(Nr+1);i++){
		cout << "w[" << setfill(' ') << setw(2) << dec << i << "]: "; 
		cout << setfill('0') << setw(2) << hex << (int)key_arr[4*i] << setfill('0') << setw(2) << (int)key_arr[4*i+1] << setfill('0') << setw(2) << (int)key_arr[4*i+2] << setfill('0') << setw(2) << (int)key_arr[4*i+3] << endl;
	}	
}

void keyexpand(char *key,char *tablename, char flag){
	
	/* check the key */
	if(strlen(key) != 32){
		errorhandle("malformed key");
	}
	
	for(int i=0; i<32; i+=2){
		key_arr[i/2] = hextochar(key[i],key[i+1]);
	}
	
	/* get the table file */
	tableCheck(tablename);
	
	/* keyExpansion */
	
	keyExpansion();
	
	if(flag != 0){
		output();
	}
}
