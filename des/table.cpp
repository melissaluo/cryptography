#include "hw5.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXNUM 200
using namespace std;

Table::Table()
{
}

Table::~Table()
{
}

void Table::tableCheck(char* filename)
{
	ifstream tablefile;
	
	tablefile.open(filename,ios::in);
	
	if(tablefile.fail()){
		errorhandle("fail to open tablefile!");
	}
	
	char *tablename = new char[MAXNUM];
	char *pos;
	int table_index = 0;

	while(tablefile.getline(tablename,MAXNUM)){
		if((pos = strstr(tablename,"IP="))!= NULL){	/*IP*/
			pos += 3;
			IPcheck(pos);
			IPinverse(pos);
		}else if((pos = strstr(tablename,"E="))!= NULL){/*E*/
			pos += 2;
			Echeck(pos);
		}else if((pos = strstr(tablename,"P="))!= NULL){/*P*/
			pos += 2;
			Pcheck(pos);			
		}else if((pos = strstr(tablename,"S"))!= NULL){/*S*/
			pos += 1;
			if((*pos)>= '1' && (*pos)<= '8' ){
				if((pos = strstr(pos,"="))!=NULL){
					pos += 1;
					Scheck(pos,table_index++);
				}
			}else{
				errorhandle("malformed tablefile!");
			}
		}else if((pos = strstr(tablename,"V="))!= NULL){/*V*/
			pos += 2;
			Vcheck(pos);
		}else if((pos = strstr(tablename,"PC1="))!= NULL){/*PC1*/
			pos += 4;
			PC1check(pos);
		}else if((pos = strstr(tablename,"PC2="))!= NULL){/*PC2*/
			pos += 4;
			PC2check(pos);
		}else{
			errorhandle("malformed tablefile!");
		}
		memset(tablename,'\0',MAXNUM);
	}	
	tablefile.close();
	delete[] tablename;
}

/* a random permutation of the values from 1 through 64, inclusive*/
void Table::IPcheck(char * str){
	int arr64[64]={0};	
	int count = 0;
	int IP_index = 0;
	char *pos;
	
	while(str!=NULL){
		char temp[3]={0};
		int tempint = -1;
		
		if((pos = strstr(str,","))!=NULL){
			strncpy(temp,str,pos-str);
			str = pos+1;
		}else{
			strncpy(temp,str,strlen(str));
			str = NULL;
		}
		tempint = atoi(temp);
		IP_table[IP_index++] = tempint; 
		
		if(tempint >=1 && tempint <=64){
			if(arr64[tempint-1] == 1){
				errorhandle("repeated number!");
			}else{
				arr64[tempint-1] = 1;
			}
		}else{
			errorhandle("IP check: number is out of range");
		}
		count++;
	}

	if(count < 64){
		errorhandle("IP check: too few values read!");
	}else if(count > 64){
		errorhandle("IP check: too many values read!");
	}
	
}

void Table::IPinverse(char *str){
	for(int i = 0 ;i < 64; i++){
		IP_inverse_table[IP_table[i]-1] = i+1;
	}
}
/* 48 values totally. 1 through 32, inclusive. appear at least once, at most twice*/
void Table::Echeck(char * str){
	int E[32] = {0};/* record 1 to 32 appearing time*/
	char *pos;/* pointer to read string*/	
	int count = 0;/* record char numbers that has been read*/	
	int E_index = 0;
	
	while(str!=NULL){
		char temp[3]={0};/* store every int as char array*/
		int tempint = 0;/* store every number*/
		
		if((pos = strstr(str,","))!=NULL){
			strncpy(temp,str,pos-str);
			str = pos + 1;
		}else{
			strncpy(temp,str,strlen(str));
			str=NULL;
		}
		tempint = atoi(temp);
		E_table[E_index++] = tempint;
		/* tempint should between 1 to 32 */
		if(tempint < 1 || tempint > 32){
			errorhandle("E check: invalid number");
		}
		/* if alread appear twice, invalid*/
		if( E[tempint-1] == 2){
			errorhandle("E check: this number already appeared twice");
		}else{
			E[tempint-1]++;
		}
		count++;		
	}
	
	if(count!=48){
		errorhandle("E check: number of character value is wrong!");
	}
}

/* contains a random permutation of the values from 1 through 32, inclusive.*/
void Table::Pcheck(char * str){
	int P[32] = {0};/* store values from 1 to 32, inclusive*/
	char *pos;
	int count = 0;
	int P_index = 0;
	
	while(str!=NULL){
		char temp[3]={0};
		int tempint = 0;
		
		if((pos = strstr(str,","))!=NULL){
			strncpy(temp,str,pos-str);
			str = pos + 1;
		}else{
			strncpy(temp,str,strlen(str));
			str=NULL;
		}
		
		tempint = atoi(temp);
		P_table[P_index++] = tempint;
		if(tempint < 1 || tempint > 32){
			errorhandle("P check: number is out of range!");
		}
		if(P[tempint-1] == 1){
			errorhandle("P check: repeated number!");
		}else{
			P[tempint-1]++;
		}
		count++;
	}

	if(count != 32){
		errorhandle("P check: wrong number of characters!");
	}
}

/* contains 64 values, in a group of 16. Each group is a random permutation of the values from 0 through 15, inclusive.*/
void Table::Scheck(char * str, int table_index){
	int S[16] = {0};
	char *pos;
	int count = 0;
	int S_row_index = 0;
	int S_col_index = 0;
	
	char errormsg[2];
	errormsg[0] = 'S';
	errormsg[1] = table_index + '1';
	
	while(str!=NULL){
		char temp[3]={0};
		int tempint = 0;
		
		if((pos = strstr(str,","))!=NULL){
			strncpy(temp,str,pos-str);
			str = pos + 1;
		}else{
			strncpy(temp,str,strlen(str));
			str=NULL;
		}
		tempint = atoi(temp);
		S_table[table_index][S_row_index][S_col_index++] = tempint;

		if(tempint < 0 || tempint > 15){
			strcat(errormsg," check: number is out of range!");
			errorhandle(errormsg);
		}
		
		if(S[tempint] == 1){
			strcat(errormsg," check: repeated number!");
			errorhandle(errormsg);
		}else{
			S[tempint]++;
		}
		
		count++;
		
		if(count%16 == 0 && count != 0){
			memset(S,0,16*4);
			S_row_index++;
			S_col_index = 0;
		}
	}

	if(count!=64){
		strcat(errormsg," check: wrong number of characters!");
		errorhandle(errormsg);
	}
	
}

/* contains 16 values. Each value can be either 1 or 2. There should be 4 ones and 12 twos.*/
void Table::Vcheck(char * str){
	int V[2]={0};/* V[0]:count of 1; V[1]:count of 2*/
	char *pos;
	int count = 0;
	int V_index = 0;
	
	while(str!=NULL){
		char temp[2]={0};
		int tempint;
		
		if((pos = strstr(str,","))!=NULL){
			strncpy(temp,str,pos-str);
			str = pos + 1;
		}else{
			strncpy(temp,str,strlen(str));
			str=NULL;
		}
		
		tempint = atoi(temp);
		V_table[V_index++] = tempint;
		
		if(tempint != 1 && tempint != 2){
			errorhandle("V check: wrong number!");
		}
		V[tempint-1]++;
		count++;
	}
	
	if(V[0]!=4 && V[1]!=12){
		errorhandle("V check: wrong number!");
	}
	if(count != 16){
		errorhandle("V check: wrong count!");
	}
}

/* contains 56 values. These values are a random permutation of the numbers from 1 through 64, inclusive, 
 * with the values which are multiples of 8 removed. */
void Table::PC1check(char * str){
	int PC1[64]={0};
	char *pos;	
	int count = 0;
	int PC1_row_index = 0;
	int PC1_col_index = 0;
	
	while(str!=NULL){
		char temp[3]={0};
		int tempint = 0;
		
		if((pos = strstr(str,","))!=NULL){
			strncpy(temp,str,pos-str);
			str = pos + 1;
		}else{
			strncpy(temp,str,strlen(str));
			str=NULL;
		}
		tempint = atoi(temp);
		PC1_table[PC1_row_index][PC1_col_index++] = tempint;
 
		if(tempint < 1 || tempint > 64 || tempint%8 == 0){
			errorhandle("PC1 check: wrong number!");
		}
		
		if(PC1[tempint-1] == 1){
			errorhandle("PC1 check: wrong number!");
		}else{
			PC1[tempint-1]++;
		}
		
		if(count == 27){
			PC1_row_index++;
			PC1_col_index = 0;
		}
		count++;
	}
	if(count!=56){
		errorhandle("PC1 check: wrong number!");
	}
}

/* contains 48 values. These values are a random permutation of the numbers from 1 through 56, inclusive, 
 * with 8 randomly chosen numbers removed. */
void Table::PC2check(char * str){
	int PC2[56]={0};
	char *pos;
	int count = 0;
	int PC2_index = 0;
	
	while(str!=NULL){
		char temp[3]={0};
		int tempint = 0;
		
		if((pos = strstr(str,","))!=NULL){
			strncpy(temp,str,pos-str);
			str = pos + 1;
		}else{
			strncpy(temp,str,strlen(str));
			str=NULL;
		}
		tempint = atoi(temp);
		PC2_table[PC2_index++] = tempint;
		
		if(tempint < 1 || tempint > 56){
			errorhandle("PC2 check: wrong number!");
		}
		
		if(PC2[tempint-1] == 1){
			errorhandle("PC2 check: wrong number!");
		}else{
			PC2[tempint-1]++;
		}
		count++;
	}

	if(count!=48){
		errorhandle("PC2 check: wrong number!");
	}
}


