#include <fstream>/*fstream*/
#include <iostream>/*cout,istream,fixed*/
#include <vector>/*vector>*/
#include <string.h>
#include <map>
#include <algorithm> /*sort*/
#include <utility>/*pair*/
#include <iomanip>/*setprecision*/
#include "hw4.h"/*CIPHER*/

using namespace std;

/*sort a map*/
void sortA(vector<mypair> myvec ,int L){
	/*sort the vector*/
	int n=myvec.size();
	for(int i=0;i<n-1;i++){
		for(int j=0;j<n-1-i;j++){
			if(myvec[j].second<myvec[j+1].second){
				mypair temp(myvec[j].first,myvec[j].second);
				myvec[j] = myvec[j+1];
				myvec[j+1] = temp;
			}
		}
	}

	/*print vector*/
	cout << "L=" << L << endl;
	
	/*loop through vector to print frequency*/
	for(vector<mypair>::iterator it = myvec.begin();it!=myvec.end();++it){
		cout << (*it).first << ": " << (*it).second << " (" << std::setprecision(2) << std::fixed << ((float)(*it).second/(float)L)*100 << "%)" << endl;
	}

}


void histo(int T, int which, istream&f){
	
	/*use a pair to record character occurrance, every time it occurs, count++*/
	vector<mypair> Amap;
	for(int i=0;i<26;i++){
		Amap.push_back(make_pair(i+'a',0));
	}

	/*record the total number of lower case character*/
	int L=0;
	
	/*process the ciphertext, read T characters one time*/
	char* cipherbuffer = new char[T];
	/*initialize the buffer, in case next time is last block of ciphertext*/
	memset(cipherbuffer,'\0',T);
	while(f.read(cipherbuffer,T)){
		/*get the character of designed postion in ciphertext*/
		char c = cipherbuffer[which-1];
		/*lower case english character*/
		if(c >= 'a' && c <= 'z'){
			L++;
			/*if the character never mapped before, it'll be inserted with default value 0*/
			Amap[c-'a'].second ++;
		}
		/*clear the buffer, in case next time is last block of plaintext*/		
		memset(cipherbuffer,'\0',T);
	}
	
	/*sort A to output the frequencies*/
	sortA(Amap,L);

	delete[] cipherbuffer;
}

