#include <string.h>
#include  <iomanip>
#include "hw6.h"
using namespace std;

unsigned char bigdotbyte(unsigned char a, unsigned char b){
	unsigned char c = 0;
	/*** irreducible polynomial m(x) ***/
	unsigned char m = hextochar('1','b');
	
	if((b & 1) > 0){
			c = c ^ a;
	}
	
	for(int i=1;i<8;i++){
		
		/*** 8th bit of c equals to 1 ***/
		
		if((a & (1 << 7)) > 0){
			a = a << 1;
			a = a ^ m;
		}else{
			a = a << 1;
		}
		
		if((b & (1 << i)) > 0){
			c = c ^ a;
		}
	}
	
	return c;
}

void polyCheck(char *p, unsigned char p_arr[], int len){
	/*** each hexstring has a string length of 8 **/
	if(strlen(p)!=8){
		errorhandle("malformed poly");
	}
	
	for(int i=0;i<8;i+=2){
		//p_arr[3-i/2] = hextochar(p[i],p[i+1]);
		p_arr[i/2] = hextochar(p[i],p[i+1]);
	}
}


void circlex(unsigned char* p1_arr, unsigned char* p2_arr, unsigned char* p3_arr){

	/***A*B***
	 * 0321
	 * 1032
	 * 2103
	 * 3210
	 *********/
	p3_arr[3] = bigdotbyte(p1_arr[3],p2_arr[3]);
	p3_arr[3] = p3_arr[3] ^ bigdotbyte(p1_arr[0],p2_arr[2]);
	p3_arr[3] = p3_arr[3] ^ bigdotbyte(p1_arr[1],p2_arr[1]);
	p3_arr[3] = p3_arr[3] ^ bigdotbyte(p1_arr[2],p2_arr[0]);
	
	p3_arr[2] = bigdotbyte(p1_arr[2],p2_arr[3]);
	p3_arr[2] = p3_arr[2] ^ bigdotbyte(p1_arr[3],p2_arr[2]);
	p3_arr[2] = p3_arr[2] ^ bigdotbyte(p1_arr[0],p2_arr[1]);
	p3_arr[2] = p3_arr[2] ^ bigdotbyte(p1_arr[1],p2_arr[0]);
	
	p3_arr[1] = bigdotbyte(p1_arr[1],p2_arr[3]);
	p3_arr[1] = p3_arr[1] ^ bigdotbyte(p1_arr[2],p2_arr[2]);
	p3_arr[1] = p3_arr[1] ^ bigdotbyte(p1_arr[3],p2_arr[1]);
	p3_arr[1] = p3_arr[1] ^ bigdotbyte(p1_arr[0],p2_arr[0]);
	
	p3_arr[0] = bigdotbyte(p1_arr[0],p2_arr[3]);
	p3_arr[0] = p3_arr[0] ^ bigdotbyte(p1_arr[1],p2_arr[2]);
	p3_arr[0] = p3_arr[0] ^ bigdotbyte(p1_arr[2],p2_arr[1]);
	p3_arr[0] = p3_arr[0] ^ bigdotbyte(p1_arr[3],p2_arr[0]);
	
}

void modprod(char *p1, char *p2){
	unsigned char p1_arr[4];
	unsigned char p2_arr[4];
	unsigned char p3_arr[4];/*** output hexstring ***/
	
	/*** check poly1, poly2 ***/
	polyCheck(p1, p1_arr, 4);
	polyCheck(p2, p2_arr, 4);
	
	//bigdotbyte(hextochar('0','1'), hextochar('0','9'));
	
	circlex(p1_arr,p2_arr,p3_arr);
	
	
	/*** ouput result ***/
	
	for(int i=0;i<4;i++){
		cout << "{" << setfill('0') << setw(2) << hex << (int)p1_arr[i] << "}";
	}
	
	cout << " CIRCLEX ";
	
	for(int i=0;i<4;i++){
		cout << "{" << setfill('0') << setw(2) << hex << (int)p2_arr[i] << "}";
	}
	
	cout << " = ";

	for(int i=0;i<4;i++){
		cout << "{" << setfill('0') << setw(2) << hex << (int)p3_arr[i] << "}";
	}
	
	cout << endl;
}
