/* Using Table Method to calculate multiplicative inverse */
/* b(x) = x^4 + 1 */
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <string.h>
#include "hw6.h"
#define MAX_STEP 6
using namespace std;

unsigned char p_arr[4];/* store the 4 bytes of coefficient */
unsigned char rem[MAX_STEP][5];
unsigned char quo[MAX_STEP][4];
unsigned char aux[MAX_STEP][4];

unsigned char bux[] = {0x01,0x00,0x00,0x00,0x01};/* b(x) = x^4 + 1*/

unsigned char charinverse_table[256] = {0x00,0x01,0x8d,0xf6,0xcb,0x52,0x7b,0xd1,0xe8,0x4f,0x29,0xc0,0xb0,0xe1,0xe5,0xc7,
											 0x74,0xb4,0xaa,0x4b,0x99,0x2b,0x60,0x5f,0x58,0x3f,0xfd,0xcc,0xff,0x40,0xee,0xb2,
											 0x3a,0x6e,0x5a,0xf1,0x55,0x4d,0xa8,0xc9,0xc1,0x0a,0x98,0x15,0x30,0x44,0xa2,0xc2,
											 0x2c,0x45,0x92,0x6c,0xf3,0x39,0x66,0x42,0xf2,0x35,0x20,0x6f,0x77,0xbb,0x59,0x19,
											 0x1d,0xfe,0x37,0x67,0x2d,0x31,0xf5,0x69,0xa7,0x64,0xab,0x13,0x54,0x25,0xe9,0x09,
											 0xed,0x5c,0x05,0xca,0x4c,0x24,0x87,0xbf,0x18,0x3e,0x22,0xf0,0x51,0xec,0x61,0x17,
											 0x16,0x5e,0xaf,0xd3,0x49,0xa6,0x36,0x43,0xf4,0x47,0x91,0xdf,0x33,0x93,0x21,0x3b,
											 0x79,0xb7,0x97,0x85,0x10,0xb5,0xba,0x3c,0xb6,0x70,0xd0,0x06,0xa1,0xfa,0x81,0x82,
											 0x83,0x7e,0x7f,0x80,0x96,0x73,0xbe,0x56,0x9b,0x9e,0x95,0xd9,0xf7,0x02,0xb9,0xa4,
											 0xde,0x6a,0x32,0x6d,0xd8,0x8a,0x84,0x72,0x2a,0x14,0x9f,0x88,0xf9,0xdc,0x89,0x9a,
											 0xfb,0x7c,0x2e,0xc3,0x8f,0xb8,0x65,0x48,0x26,0xc8,0x12,0x4a,0xce,0xe7,0xd2,0x62,
											 0x0c,0xe0,0x1f,0xef,0x11,0x75,0x78,0x71,0xa5,0x8e,0x76,0x3d,0xbd,0xbc,0x86,0x57,
											 0x0b,0x28,0x2f,0xa3,0xda,0xd4,0xe4,0x0f,0xa9,0x27,0x53,0x04,0x1b,0xfc,0xac,0xe6,
											 0x7a,0x07,0xae,0x63,0xc5,0xdb,0xe2,0xea,0x94,0x8b,0xc4,0xd5,0x9d,0xf8,0x90,0x6b,
											 0xb1,0x0d,0xd6,0xeb,0xc6,0x0e,0xcf,0xad,0x08,0x4e,0xd7,0xe3,0x5d,0x50,0x1e,0xb3,
											 0x5b,0x23,0x38,0x34,0x68,0x46,0x03,0x8c,0xdd,0x9c,0x7d,0xa0,0xcd,0x1a,0x41,0x1c};



/* rem[0] / rem[1], store in rem[2], quo[2], x[2] */
void longDiv1(){
	
	memset(rem[2],0,5);
	memset(quo[2],0,4);
	memset(aux[2],0,4);
	
	/* temp table, 4 rows, 5 columns*/
	unsigned char long_div_table[4][5];
	for(int i=0;i<4;i++){
		memset(long_div_table[i],0,5);
	}
	
	/* calculate first row */
	
	/* find inverse of first char of rem[1] as quo[2] */
	quo[2][2] = charinverse_table[rem[1][1]];
	
	/* store quo[2]*rem[1] in first row of long_div_table[0] */
	for(int i=0;i<4;i++){
		long_div_table[0][i] = bigdotbyte(quo[2][2],rem[1][i+1]);
	}
	
	/* do xor between rem[0] and long_div_table[0] */
	for(int i=0;i<5;i++){
		long_div_table[1][i] = long_div_table[0][i] ^ rem[0][i];
	}
	
	/* copy last of rem[0] to long_div_table[1][4] */
	long_div_table[1][4] = rem[0][4];
	
	/* find inverse of first char of rem[1], big dot with long_div_table[1][1], store in quo[3] */
	quo[2][3] = bigdotbyte(quo[2][2], long_div_table[1][1]);
	
	/* do xor between quo[2][3] and rem[1] */
	for(int i=0;i<5;i++){
		long_div_table[2][i] = bigdotbyte(quo[2][3],rem[1][i]);
	}
	
	/* do xor between long_div_table[1] and long_div_table[2] */
	for(int i=0;i<5;i++){
		long_div_table[3][i] = long_div_table[1][i] ^ long_div_table[2][i];
	}
	
	memcpy(rem[2],long_div_table[3],5);
	
	/* assign quo[2] to aux[2] */
	for(int i=0;i<4;i++){
		aux[2][i] = quo[2][i];
	} 
	
}

/* rem[1] / rem[2], store in rem[3], quo[3], x[3] */
void longDiv2(){
	memset(rem[3],0,5);
	memset(quo[3],0,4);
	memset(aux[3],0,4);
	unsigned char long_div_table[4][5];
	/* find inverse of divisor rem[2][2] as quo[3][2] */
	unsigned char inverse_first= charinverse_table[rem[2][2]];
	
	/* big dot between rem[1][1] and inverse_first */
	quo[3][2] = bigdotbyte(rem[1][1], inverse_first);
	
	/* do bigdot between rem[2] and quo[3][2], store in  long_div_table[0]*/ 
	for(int i=0;i<3;i++){
		long_div_table[0][i+1] = bigdotbyte(rem[2][i+2],quo[3][2]);
	}
	
	/* do xor between long_div_table[0] and rem[1], store in  long_div_table[1]*/ 
	for(int i=0;i<5;i++){
		long_div_table[1][i] = long_div_table[0][i] ^ rem[1][i];
	}
	
	/* copy last of rem[1] to long_div_table[1][4] */
	long_div_table[1][4] = rem[1][4];
	
	/* find quo[3][3] */
	quo[3][3] = bigdotbyte(inverse_first, long_div_table[1][2]);
	
	/* big dot between quo[3][3] and rem[1] */
	for(int i=0;i<5;i++){
		long_div_table[2][i] = bigdotbyte(rem[2][i], quo[3][3]);
	}
	
	/* xor between long_div_table[1] and long_div_table[2] */
	for(int i=0;i<5;i++){
		long_div_table[3][i] = long_div_table[1][i] ^ long_div_table[2][i];
	}
	
	memcpy(rem[3],long_div_table[3],5);
	
	/* update x */
	circlex(quo[3], aux[2], aux[3]);
	for(int i=0;i<4;i++){
		aux[3][i] ^= aux[1][i];
	}
}

/* rem[2] / rem[3], store in rem[4], quo[4], aux[4] */
void longDiv3(){
	memset(rem[4],0,5);
	memset(quo[4],0,4);
	memset(aux[4],0,4);
	unsigned char long_div_table[4][5];
	/* find inverse of divisor rem[3][3] */	
	unsigned char inverse_first= charinverse_table[rem[3][3]];
	
	/* big dot between rem[2][2] and inverse_first, store in quo[4][2]  */
	quo[4][2] = bigdotbyte(rem[2][2], inverse_first);
	
	/* do bigdot between rem[3] and quo[4][2], store in  long_div_table[0]*/ 
	for(int i=0;i<2;i++){
		long_div_table[0][i+2] = bigdotbyte(rem[3][i+3],quo[4][2]);
	}
	
	/* do xor between long_div_table[0] and rem[2], store in  long_div_table[1]*/ 
	for(int i=0;i<5;i++){
		long_div_table[1][i] = long_div_table[0][i] ^ rem[2][i];
	}
	
	/* copy last of rem[2] to long_div_table[1][4] */
	long_div_table[1][4] = rem[2][4];
	
	/* find quo[4][3] */
	quo[4][3] = bigdotbyte(inverse_first, long_div_table[1][3]);
	
	/* big dot between quo[4][3] and rem[3] */
	for(int i=0;i<5;i++){
		long_div_table[2][i] = bigdotbyte(rem[3][i], quo[4][3]);
	}
	
	/* xor between long_div_table[1] and long_div_table[2] */
	for(int i=2;i<5;i++){
		long_div_table[3][i] = long_div_table[1][i] ^ long_div_table[2][i];
	}
	
	memcpy(rem[4],long_div_table[3],5);
	
	/* update x */
	circlex(quo[4],aux[3],aux[4]);
	for(int i=0;i<4;i++){
		aux[4][i] ^= aux[2][i];
	}
}

/* rem[3] / rem[4], make rem[5] = 00 00 00 01 */
void longDiv4(){
	memset(rem[5],0,5);
	memset(quo[5],0,4);
	memset(aux[5],0,4);
	
	unsigned char inverse_first = charinverse_table[rem[4][4]];
	
	quo[5][2] = bigdotbyte(inverse_first,rem[3][3]);
	
	unsigned char last_char = rem[3][4] ^ 1;
	
	quo[5][3] = bigdotbyte(inverse_first,last_char);
	
	circlex(quo[5],aux[4],aux[5]);
	for(int i=0;i<4;i++){
		aux[5][i] ^= aux[3][i];
	}
	
	rem[5][4] = 1;
}

/* check the char of rem[step-1] at position 'step' */
void zero_check(int step){
	
	if(rem[step-1][step-1] == 0){
		cout << "{" << setfill('0') << setw(2) << hex << (int)rem[1][1] << "}{" << setfill('0') << setw(2) << hex << (int)rem[1][2] << "}{" << setfill('0') << setw(2) << hex << (int)rem[1][3]<< "}{" << setfill('0') << setw(2) << hex << (int)rem[1][4];
		cout << " does not have a multiplicative inverse." << endl;;
		exit(1);
	}
}

/*  example:
 *  i=1, rem[i]={00}{00}{00}{01}, quo[i]={00}{00}{00}{00}, aux[i]={00}{00}{00}{00} */
void outputstep(int i){
	//cout << "i=" << i << ", rem[i]=";
	printf("i=%d, rem[i]=",i);
	printf("{%02x}{%02x}{%02x}{%02x}",rem[i-1][1],rem[i-1][2],rem[i-1][3],rem[i-1][4]);
	printf(", quo[i]=");
	printf("{%02x}{%02x}{%02x}{%02x}",quo[i-1][0],quo[i-1][1],quo[i-1][2],quo[i-1][3]);
	printf(", aux[i]=");
	printf("{%02x}{%02x}{%02x}{%02x}\n",aux[i-1][0],aux[i-1][1],aux[i-1][2],aux[i-1][3]);
	//cout << "{" << setfill('0') << setw(2) << hex << (int)rem[i-1][1] << "}" << "{" << setfill('0') << setw(2) << hex << (int)rem[i-1][2] << "}" << "{" << setfill('0') << setw(2) << hex << (int)rem[i-1][3] << "}" << "{" << setfill('0') << setw(2) << hex << (int)rem[i-1][4] << "}";
	//cout << ", quo[i]=";
	//cout << "{" << setfill('0') << setw(2) << hex << (int)quo[i-1][0] << "}" << "{" << setfill('0') << setw(2) << hex << (int)quo[i-1][1] << "}" << "{" << setfill('0') << setw(2) << hex << (int)quo[i-1][2] << "}" << "{" << setfill('0') << setw(2) << hex << (int)quo[i-1][3] << "}";
	//cout << ", aux[i]=";
	//cout << "{" << setfill('0') << setw(2) << hex << (int)aux[i-1][0] << "}" << "{" << setfill('0') << setw(2) << hex << (int)aux[i-1][1] << "}" << "{" << setfill('0') << setw(2) << hex << (int)aux[i-1][2] << "}" << "{" << setfill('0') << setw(2) << hex << (int)aux[i-1][3] << "}";	
	//cout << endl;
}

void inverse(char *p){
	int step = 1;
	
	if(strlen(p)!=8){
		errorhandle("malformed poly");
	}
	
	for(int i=0;i<8;i+=2){
		p_arr[i/2] = hextochar(p[i],p[i+1]);
	}
	
	unsigned char identity_arr[] = {0x00,0x00,0x00,0x01};
	
	/* initialize rem[0], rem[1], quo[0], quo[1], aux[0], aux[1] */
	rem[0][0] = 1;
	rem[1][0] = 0;
	
	memcpy(&rem[0][1],identity_arr,4);
	memcpy(&rem[1][1], p_arr,4);
	
	memset(quo[0],0,4);
	memset(quo[1],0,4);
	
	memset(aux[0],0,4);
	memcpy(&aux[1], identity_arr,4);
	
	outputstep(step++);
	outputstep(step++);
	
	/* first round, calculate rem[2] */
	longDiv1();
	outputstep(step);
	zero_check(step++);
	
	/* second round, calculate rem[3] */
	longDiv2();
	outputstep(step);
	zero_check(step++);
	
	/* third round, calculate rem[4] */
	longDiv3();
	outputstep(step);
	zero_check(step++);
	
	/* last round, calculate rem[5] */
	longDiv4();
	outputstep(step);
	
	cout << "Multiplicative inverse of ";
	cout << "{" << setfill('0') << setw(2) << hex << (int)rem[1][1] << "}{" << setfill('0') << setw(2) << hex << (int)rem[1][2] << "}{" << setfill('0') << setw(2) << hex << (int)rem[1][3]<< "}{" << setfill('0') << setw(2) << hex << (int)rem[1][4] << "}";
	cout << " is ";
	cout << "{" << setfill('0') << setw(2) << hex << (int)aux[5][0] << "}{" << setfill('0') << setw(2) << hex << (int)aux[5][1] << "}{" << setfill('0') << setw(2) << hex << (int)aux[5][2]<< "}{" << setfill('0') << setw(2) << hex << (int)aux[5][3] << "}";
	cout << endl;
}
