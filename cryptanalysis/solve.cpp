#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>/*strncpy*/
#include <map>
#include <ctype.h>/*islower*/
#include <utility>/*pair,make_pair*/
#include <iomanip>/*setprecision*/
#include "hw4.h"

using namespace std;

map<int,vector<matched> > mymap;


void solve(int max_t,fstream&f){
	/*read the ciphertext file into a char array*/
	streampos begin,end;
	begin = f.tellg();
	f.seekg(0,ios::end);
	end = f.tellg();
	
	int textlen = end-begin;
	char* ciphertext = new char[textlen];
	f.clear();
	f.seekg(0,ios::beg);
	f.read(ciphertext,textlen);
	
	/*begin solve*/
	KasiskiMethod(textlen,ciphertext);
	
	avgindexcoin(max_t,textlen, ciphertext);
	
	AutocorrelationMethod(max_t,textlen, ciphertext);
	
	delete[] ciphertext;
}


/*Kasiski Method*/
void KasiskiMethod(int textlen, char *text){
	/*begin with a string length of 4, loop through 4 to max_t, if no matches found at any length, break */
	int index = start_index;
	while(searchcipher(index,text,textlen)){
		index++;
	}
	
	/*print result*/
	KasiskiPrinter();
}

matched make_matched(int i, int j, char* str, int len){
	matched mystring;
	
	mystring.i = i;
	mystring.j = j;
	mystring.text = new char[len];
	strncpy(mystring.text,str,len);
	mystring.text[len]='\0';
	
	return mystring;
}

bool searchcipher(int len,char* text,int textlen){
	char* buffer = new char[len];
	char *begin,*current;	
	begin = text;
	
	bool search_found = false;
	
	/*delcare a vector for given lenght*/
	vector<matched> myvec;
	
	
	/*if current length equals start_index, Kasiski is empty, need to loop through the whole text*/
	if(len == start_index){
		for(int i=0;i<textlen-len;i++){
			/*copy the len character into buffer*/
			strncpy(buffer,&text[i],len);
			/*if buffer contains non-alphabet continue*/
			bool flag = true;
			for(int m=0; m<len; m++){
				if(!islower(buffer[m])){
					flag = false;
					break;
				}
			}
			if(flag == false){
				continue;
			}
			/*search string from the remaining text
			 * if has return value, store them in Kasiski vector*/
			int index = i + len;

			while((index < textlen) && (current=strstr(&text[index],buffer))!=NULL){
				myvec.push_back(make_matched(i,current-begin,buffer,len));
				search_found = true;
				index = current-begin + len;
			}

		}
	}else{
		/*if current length larger than start_index, search Kasiski for (length-1) substrings*/
		vector<matched> smallervec = mymap[len-1];
		
		for(vector<matched>::iterator iv=smallervec.begin();iv!=smallervec.end();++iv){
			
			/*if the two character match, and they are between 'a' and 'z'*/
			if(((*iv).j + len) < textlen){
				if((text[(*iv).i+len-1] == text[(*iv).j+len-1]) && islower(text[(*iv).i+len-1])){
					/*put them into vector*/
					myvec.push_back(make_matched((*iv).i,(*iv).j,&text[(*iv).i],len));
					search_found = true;
				}
			}
			
		}
		
	}
	
	mymap[len] = myvec;
		
	delete[] buffer;
	return search_found;
}

void KasiskiPrinter(){	
	cout << "Kasiski Method" << endl;
	cout << "==============" << endl;
	
	for(map<int,vector<matched> >::iterator im = mymap.begin();im!=mymap.end();++im){
		/*get the number of substrings with current length*/
		vector<matched> lenvec = (*im).second;

		for(vector<matched>::iterator iv=lenvec.begin();iv!=lenvec.end();++iv){
			cout << "len=" << (*im).first << ", i=" << (*iv).i << ", j=" << (*iv).j << ", j-i=" << (*iv).j - (*iv).i << ", " << (*iv).text << endl;
		}
	}
	
	/*output the last line*/
	cout << "len=" << mymap.size() - 1 + start_index << ", no more matches" << endl;

	
}

/*average index of coincidence*/
void avgindexcoin(int max_t,int textlen, char *text){
	/*count the total number of lower case and its frequency*/
	vector<mypair> Amap;
	int L=0;
	
	/*initialize Amap*/
	for(int i=0;i<26;i++){
		Amap.push_back(make_pair(i+'a',0));
	}
	
	/*loop through the whole text*/
	for(int i=0;i<textlen;i++){
		if(islower(text[i])){
			Amap[text[i]-'a'].second++;
			L++;
		}
	}
	
	/*compute IC 
	 *float can support up to 7 decimal digits, double supports 15~16 decimal digits, here we set precision to 9*/
	double IC=0;
	
	for(vector<mypair>::iterator it=Amap.begin();it!=Amap.end();++it){
		IC += ((*it).second)*((*it).second-1);
	}
	
	IC /= L*(L-1);
	
	/*compute expected ICs through t=1 to max_t*/
	double *EICs = new double[max_t];
 
	for(int t=1;t<=max_t;t++){
		EICs[t-1] = (double)((L-t)*kp)/(t*(L-1)) + (double)((t-1)*L)/(t*(L-1)*N);
	}
	
	avgindexcoinPrinter(L, max_t, Amap, IC,EICs);
	
	delete[] EICs;
}

void avgindexcoinPrinter(int L, int max_t, vector<mypair> Amap, double IC,double* EICs){
	
	cout << endl;
	cout << "Average Index of Coincidence" << endl;
	cout << "============================" << endl;
	
	cout << "L=" << L << endl;
	
	for(vector<mypair>::iterator it=Amap.begin();it!=Amap.end();++it){
		cout << "f('" << (*it).first << "')=" <<  (*it).second << endl;
	}
	
	cout << "IC=" << setprecision(8) << IC << endl;
	
	for(int i=0; i < max_t; i++){
		cout << "t=" << i+1 << ", E(IC)=" << setprecision(8) << EICs[i] << endl;
	}
}

/* Auto correlation Method*/
void AutocorrelationMethod(int max_t,int textlen, char *text){
	int *occurcount = new int[max_t];
	
	/*initialize two pointers for searching*/
	char* searchp1 = NULL;
	char* searchp2 = NULL;
	int index = 0;
	
	/*loop from t=1 to max_t, every time right shift the text t characters*/
	for(int t=1;t<=max_t;t++){
		
		searchp1 = text;
		searchp2 = text + t;
		index = textlen-t;
		occurcount[t-1] = 0;
		
		/*search common characters(lower case) from two strings, count the occurrance*/	
		while(index > 0){
			if(*searchp1 == *searchp2 && islower(*searchp1)){
				occurcount[t-1] ++;
			}
			searchp1 ++;
			searchp2 ++;
			index--;
		}
	}
	
	AutocorrelationPrinter(max_t,occurcount);
	delete[] occurcount;
}

void AutocorrelationPrinter(int max_t,int occurcount[]){
	
	cout << endl;
	cout << "Auto-correlation Method" << endl;
	cout << "=======================" << endl;
	
	for(int i=0;i<max_t;i++){
		cout << "t=" << i+1 << ", count=" << occurcount[i] << endl;
	}
	cout << endl;
}

