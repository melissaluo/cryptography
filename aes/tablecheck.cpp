#include <iostream>
#include <fstream>
#include <string.h>
#include "hw6.h"

using namespace std;
unsigned char S_table[S_NUM];
unsigned char P_table[P_NUM];
unsigned char INVP_table[P_NUM];
unsigned char INVS_table[S_NUM];

/******* convert two hex numbers to a char ********/
unsigned char hextochar(char c1, char c2){
	int i1,i2;
	
	if(c1 >= '0' && c1 <= '9'){
		i1 = c1 - '0';
	}else if(c1 >= 'a' && c1 <= 'f'){
		i1 = c1 - 'a' + 10;
	}else{
		errorhandle("malformed tablefile!");
	}
	
	if(c2 >= '0' && c2 <= '9'){
		i2 = c2 - '0';
	}else if(c2 >= 'a' && c2 <= 'f'){
		i2 = c2 - 'a' + 10;
	}else{
		errorhandle("malformed tablefile!");
	}
	
	return (i1 << 4) + i2;
}


/* check P bigdot INVP == {00}{00}{00}{01} ? */
void inversecheck(){
	unsigned char identity_arr[4];
	identity_arr[0] = hextochar('0','0');
	identity_arr[1] = hextochar('0','0');
	identity_arr[2] = hextochar('0','0');
	identity_arr[3] = hextochar('0','1');
	for(int i=0; i<4; i++){
		if(identity_arr[i] != (bigdotbyte(P_table[i],INVP_table[i])));
	}
}

void inversesbox(){
	for(int i=0;i<S_NUM;i++){
		INVS_table[(int)S_table[i]] = i; 
	}
}

void tableCheck(char *filename){
	ifstream tablefile;
	
	tablefile.open(filename,ios::in);
	
	if(tablefile.fail()){
		errorhandle("fail to open tablefile!");
	}
	
	/*******get the first line S= ***********/
	int s_len = S_NUM*2 + 2;/*** S= + 256*2 ***/
	char *s_str = new char[s_len+2];
	tablefile.getline(s_str, s_len+2);
	
	char *pos;
	int checker[S_NUM] = {0}; /** to check whether this char is duplicated **/
	int S_table_index = 0;
	
	if((pos = strstr(s_str, "S="))!=NULL){
		pos += 2;
		while( *pos != 0){
				char c1 = *pos;
			char c2;
			pos ++;
			if( *pos != 0){
				c2= *pos;
				pos++;
			}else{
				errorhandle("malformed tablefile of S!");
			}
			
			/** convert hexstring to a char **/
			unsigned char c;
			
			/** should obey the hexstring form **/
			c = hextochar(c1, c2);
			
			/** check whether this char is duplicated **/
			int val = c;
			if(checker[val] == 1){
				errorhandle("malformed tablefile of S!");
			}else{
				checker[val] = 1;
			}
			
			/** store this char into S_table **/
			S_table[S_table_index++] = c;
		}
	}else{
		errorhandle("malformed tablefile of S!");
	}
	
	if(S_table_index!=S_NUM){
		errorhandle("malformed tablefile of S!");
	}
	
	delete[] s_str;
	
	/*******get the second line P= ***********/
	int p_len = P_NUM*2 + 2;
	char *p_str = new char[p_len+2];
	tablefile.getline(p_str,p_len+2);
	
	int P_table_index = 0;
	
	if((pos = strstr(p_str, "P="))!=NULL){
		pos += 2;
		
		while( *pos != 0){
			char c1 = *pos;
			char c2;
			pos ++;
			if( *pos != 0){
				c2= *pos;
				pos++;
			}else{
				errorhandle("malformed tablefile of P!");
			}
			
			/** convert hexstring to a char **/
			unsigned char c;
			
			/** should obey the hexstring form **/
			c = hextochar(c1, c2);
			
			/** store this char into P_table **/
			P_table[P_table_index++] = c;
		}
	}
	
	if(P_table_index!=P_NUM){
		errorhandle("malformed tablefile of P!");
	}
	
	delete[] p_str;
	
	/*******get the third line INVP= ***********/
	int invp_len = P_NUM*2 + 5;
	char *invp_str = new char[invp_len+2];
	tablefile.getline(invp_str,invp_len+2);
	
	int INVP_table_index = 0;
	
	if((pos = strstr(invp_str, "INVP="))!=NULL){
		pos += 5;
		
		while( *pos != 0){
			char c1 = *pos;
			char c2;
			pos ++;
			if( *pos != 0){
				c2= *pos;
				pos++;
			}else{
				errorhandle("malformed tablefile of INVP!");
			}
			
			/** convert hexstring to a char **/
			unsigned char c;
			
			/** should obey the hexstring form **/
			c = hextochar(c1, c2);
			
			/** store this char into P_table **/
			INVP_table[INVP_table_index++] = c;
		}
	}
	
	if(INVP_table_index!=P_NUM){
		errorhandle("malformed tablefile of INVP!");
	}
	
	inversecheck();
	inversesbox();
	delete[] invp_str;
	
	tablefile.close();
	
}
