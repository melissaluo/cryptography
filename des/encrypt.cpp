#include <iostream>
#include <vector>
#include "hw5.h"
#include "table.h"
#include <algorithm> //reverse
#include <string.h>

using namespace std;

vector<vector<int> > subkey_arr;

/****************************************
 * command:
 * hw5 encrypt -k=key -t=tablefile [file]
 ***************************************/
vector<vector<int> > input_array;
vector<int> input;
bool print_flag = false;//just print first 8 byte block
bool last_flag = true;//output E(K3),OR D(K1)

int L[32],R[32],L_backup[32],R_backup[32];

void encrypt(char* key, char* tablefile,istream& f){
	print_flag = false;
	
	Table table;;
	table.tableCheck(tablefile);
	keySchedule(key,table);

	inputProcess(f);
	for(int i=0;i<(signed int)input_array.size();i++){
		input = input_array[i];
		IPinput(table);
		mainProcess(table);
		print_flag = true;
	}
}

void decrypt(char* key, char* tablefile,istream& f){
	print_flag = false;
	
	Table table;;
	table.tableCheck(tablefile);
	
	keySchedule(key,table);
	reverse(subkey_arr.begin(),subkey_arr.end());

	inputProcess(f);
	for(int i=0;i<(signed int)input_array.size();i++){
		input = input_array[i];
		IPinput(table);
		mainProcess(table);
		print_flag = true;
	}
}

void encrypt3(char* key, char* tablefile,istream& f){
	print_flag = true;
	
	Table table;;
	table.tableCheck(tablefile);
	char keys[3][16];
	int key_index=0;
	
	/* key should be 16*3 ASCII */
	while(key[key_index*16]!=0){
		if(strlen(key)<16){
			errorhandle("key has wrong length!");
		}
		strncpy(keys[key_index],&key[key_index*16],16);
		key_index++;
	}
	
	inputProcess(f);
	for(int i=0;i<(signed int)input_array.size();i++){
		
		/* encrypt with k1 */
		last_flag=false;
		input = input_array[i];
		IPinput(table);
		keySchedule(keys[0],table);
		mainProcess(table);
		/* decrypt with k2 */
		IPinput(table);
		keySchedule(keys[1],table);
		reverse(subkey_arr.begin(),subkey_arr.end());
		mainProcess(table);
		/* encrypt with k3 */
		last_flag=true;
		IPinput(table);
		keySchedule(keys[2],table);
		mainProcess(table);
	}
}

void decrypt3(char* key, char* tablefile,istream& f){
	print_flag = true;
	
	Table table;;
	table.tableCheck(tablefile);
	char keys[3][16];
	int key_index=0;
	
	/* key should be 16*3 ASCII */
	while(key[key_index*16]!=0){
		if(strlen(key)<16){
			errorhandle("key has wrong length!");
		}
		strncpy(keys[key_index],&key[key_index*16],16);
		key_index++;
	}
	inputProcess(f);
	for(int i=0;i<(signed int)input_array.size();i++){
		
		/* decrypt with k3 */
		last_flag=false;
		input = input_array[i];
		IPinput(table);
		keySchedule(keys[2],table);
		reverse(subkey_arr.begin(),subkey_arr.end());
		mainProcess(table);
		/* encrypt with k2 */
		IPinput(table);
		keySchedule(keys[1],table);
		mainProcess(table);
		/* decrypt with k1 */
		last_flag=true;
		IPinput(table);
		keySchedule(keys[0],table);
		reverse(subkey_arr.begin(),subkey_arr.end());
		mainProcess(table);
	}
}

/*input should be 64 bits, 8 ASCII characters*/
void inputProcess(istream& f){
	char char_array[8];
	int char_array_index = 0;
	int input_row = 0;
	int mask_array[] = {128,64,32,16,8,4,2,1};
	
	/* input < 64 bits, pad 0
	 * input > 64 bits, push to vector */
	while(!f.eof()){
		f.get(char_array[char_array_index++]);
		if(char_array_index == 8){
			input_row=0;
			char_array_index=0;
			vector<int> inputtemp(64,0);
			/* convert ASCII characters to bits */
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					inputtemp[input_row + j] = (char_array[i]&mask_array[j])>>(7-j);
				}
				input_row+=8;
			}
			input_array.push_back(inputtemp);
			memset(char_array,0,8);
		}
	} 
	char_array_index--;
	if(char_array_index>0 && char_array_index<8){
		/* padding 0*/
		input_row=0;
		vector<int> inputtemp(64,0);
		for(int i=0;i<char_array_index;i++){
			for(int j=0;j<8;j++){
				inputtemp[input_row + j] = (char_array[i]&mask_array[j])>>(7-j);
			}
			input_row+=8;
		}
		input_array.push_back(inputtemp);
	}
}

/*key schedule*/
void keySchedule(char* keystr, const Table& table){
	/*key should be 8-byte long and in hexstring format*/
	int key[64] = {0};
	char tempint;
	int key_index = 0;
	int mask_array[] = {8,4,2,1};
	
	if(strlen(keystr)!=8){
		errorhandle("malformed key!");
	}
	if(subkey_arr.size()>0){
		subkey_arr.clear();
	}
	
	/***************************
	    remove 8 parity bits
	 ***************************/
	for(int i=0;i<16;i++){
		if(keystr[i] >= '0' && keystr[i] <= '9'){
			tempint = keystr[i] - '0';
		}else if(keystr[i] >= 'a' && keystr[i] <= 'f'){
			tempint = keystr[i] - 'a' + 10;
		}else{
			errorhandle("malformed key!");
		}

		for(int j=0;j<4;j++){
			if(i%2!=0 && j==3){
				key[key_index++] = -1;
			}else{
				key[key_index++] = (tempint&mask_array[j])>>(3-j);
			}
		}
	}
	
	/***************************
	    PC1(k)->T C0,D0
	 ***************************/

	 int C[28],D[28];
	 
	 for(int i=0;i<28;i++){
		 C[i] = key[table.PC1_table[0][i]-1];
		 D[i] = key[table.PC1_table[1][i]-1];
	 }
	 
	 if(print_flag == false){
		 cerr << "(C0,D0)=";
		 for(int i=0;i<28;i+=4){
			 int temp = C[i]<<3;
			 temp += C[i+1] << 2;
			 temp += C[i+2] << 1;
			 temp += C[i+3];
			 cerr << hex << temp;
		 }
		 for(int i=0;i<28;i+=4){
			 int temp = D[i]<<3;
			 temp += D[i+1] << 2;
			 temp += D[i+2] << 1;
			 temp += D[i+3];
			 cerr << hex << temp;
		 }
		 cerr << endl;
	 }
	
	 
	/***************************
	    16 iterations, left circular shift
		C[i] = (C[i-1] << V[i])
		D[i] = (D[i-1] << V[i])
		K[i] = PC2(C[i],D[i])
	 ***************************/

	 int startc=0;
	 int startd=0;
	
	 
	 for(int i=0;i<16;i++){
		vector<int> subkey;
		startc = (startc + table.V_table[i])%28;
		startd = (startd + table.V_table[i])%28;

		 for(int j=0;j<48;j++){
			 int index = 0;
			 if(table.PC2_table[j]>=1 && table.PC2_table[j]<=28){/* search from C[i]*/
				index = (startc + table.PC2_table[j]-1)%28;
				subkey.push_back(C[index]);
			 }else if(table.PC2_table[j]>=29 && table.PC2_table[j]<=56){/* search from D[i]*/ 
				index = (startd + table.PC2_table[j]-29)%28;
				subkey.push_back(D[index]);
			 }else{
				 errorhandle("wrong value of V table");
			 }
		 }
		 
		 subkey_arr.push_back(subkey);
		
		 if(print_flag == false){
			 cerr << "(C" << dec << i+1 << ",D" << i+1 << ")=";
			 for(int m=0;m<28;m+=4){
				 int temp = C[(startc+m)%28]<<3;
				 temp += C[(startc+m+1)%28] << 2;
				 temp += C[(startc+m+2)%28] << 1;
				 temp += C[(startc+m+3)%28];
				 cerr << hex << temp;
			 }
			 for(int m=0;m<28;m+=4){
				 int temp = D[(startc+m)%28]<<3;
				 temp += D[(startc+m+1)%28] << 2;
				 temp += D[(startc+m+2)%28] << 1;
				 temp += D[(startc+m+3)%28];
				 cerr << hex << temp;
			 }
			 cerr << endl;
			 cerr << "k" << dec << i+1 << "=";
			 for(int m=0;m<48;m+=4){
				 int temp = subkey[m]<<3;
				 temp += subkey[m+1] << 2;
				 temp += subkey[m+2] << 1;
				 temp += subkey[m+3];
				 cerr << hex << temp;
			 }
			 cerr << endl;
		 }
	 }
}

/*IP get *L[0] and R[0]*/
void IPinput(const Table& table){
	for(int i=0;i<32;i++){
		L[i] = input[table.IP_table[i]-1];
		R[i] = input[table.IP_table[i+32]-1];
		L_backup[i]=L[i];
	}
	if(print_flag == false){
		cerr << "(L0,R0)=";
		for(int m=0;m<32;m+=4){
			int temp=L[m]<<3;
			temp+=L[m+1]<<2;
			temp+=L[m+2]<<1;
			temp+=L[m+3];
			cerr << hex << temp;
		}
		for(int m=0;m<32;m+=4){
			int temp=R[m]<<3;
			temp+=R[m+1]<<2;
			temp+=R[m+2]<<1;
			temp+=R[m+3];
			cerr << hex << temp;
		}
		cerr << endl;
	}
}

/* 16 loop, f(R(i-1),ki) = P(S(E(R(i-1) exlusive or Ki)))*/
void mainProcess(const Table& table){
	int expand_R[48];
	/* iterate 16 times*/
	for(int i=1;i<=16;i++){
		/* update L[i]*/
		for(int j=0;j<32;j++){
			L[j] = R[j];
		}
		/* update R[i]
		 * P(S(E(R[i-1]) exclusive K[i])))
		 * */
		
		/* expand R[i-1], and do exclusive or with K  */
		for(int j=0;j<48;j++){
			expand_R[j] = R[table.E_table[j]-1] ^ subkey_arr[i-1][j];
		}
		/* do 8 substition */
		int row=0;
		int col=0;
		int temp=0;
		int mask_array[] ={8,4,2,1};
		for(int m=0;m<48;m+=6){
			row=expand_R[m]*2;
			row+=expand_R[m+5]*1;
			col=expand_R[m+1]*8;
			col+=expand_R[m+2]*4;
			col+=expand_R[m+3]*2;
			col+=expand_R[m+4]*1;
			temp = table.S_table[m/6][row][col];
			R_backup[m*4/6] = (temp&mask_array[0])>>3;
			R_backup[m*4/6+1] = (temp&mask_array[1])>>2;
			R_backup[m*4/6+2] = (temp&mask_array[2])>>1;
			R_backup[m*4/6+3] = temp&mask_array[3];
		}
		/* do permutation and exclusive or with L_backup*/
		for(int j=0;j<32;j++){
			R[j] = (R_backup[table.P_table[j]-1]) ^ L_backup[j];
			/* update L_backup */
			L_backup[j] = L[j];
		}
		
		if(print_flag == false){
			cerr << dec << "(L" << i << ",R" << i << ")=";
			for(int m=0;m<32;m+=4){
				int temp=L[m]<<3;
				temp+=L[m+1]<<2;
				temp+=L[m+2]<<1;
				temp+=L[m+3];
				cerr << hex << temp;
			}
			for(int m=0;m<32;m+=4){
				int temp=R[m]<<3;
				temp+=R[m+1]<<2;
				temp+=R[m+2]<<1;
				temp+=R[m+3];
				cerr << hex << temp;
			}
			cerr << endl;
		}
	}
	
	/* exchange L[16] and R[16] */

	for(int i=0;i<32;i++){
		L[i] = R[i];
		R[i] = L_backup[i];
	}
	
	/* IP inverse */
	for(int i=0;i<64;i++){
		if(table.IP_inverse_table[i]>=1 && table.IP_inverse_table[i]<=32){
			input[i]=L[table.IP_inverse_table[i]-1];
		}else if(table.IP_inverse_table[i]>=33 && table.IP_inverse_table[i]<=64){
			input[i]=R[table.IP_inverse_table[i]-1];
		}else{
			errorhandle("wrong value of IP inverse table!");
		}
	}
	if(last_flag == true){
		for(int m=0;m<64;m+=8){
			char temp=input[m]<<7;
			temp+=input[m+1]<<6;
			temp+=input[m+2]<<5;
			temp+=input[m+3]<<4;
			temp+=input[m+4]<<3;
			temp+=input[m+5]<<2;
			temp+=input[m+6]<<1;
			temp+=input[m+7];
			cout << temp;
		}
	}
}
