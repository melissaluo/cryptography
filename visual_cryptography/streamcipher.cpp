#include <sys/types.h>
#include <openssl/md5.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#define MD5_DIGEST_LENGTH 16

using namespace std;

void streamcipher(char* p, int outlen, int mode, char* keystream){

	char *md5_buf  = new char[MD5_DIGEST_LENGTH];
	int len=strlen(p)+2+MD5_DIGEST_LENGTH;
	char *s= new char[len+1];
	int i=0;
	int j=0,remainder=0;
	int index = 0;
	
	MD5((unsigned char *)p, strlen(p), (unsigned char *)md5_buf);
	/*
     * Begin code I did not write.
     * The following code was obtained from <URL>...
     * [ copyright so and so, if required ... ]
     */
	for (j=1; 8*j <= outlen; j++) {
		sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, p);
		memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
		MD5((unsigned char *)s, len, (unsigned char *)md5_buf);
		if(mode == 1){//output to stdout
			cout.write(md5_buf,8);
		}else if(mode == 0){//output to keystream
			for(int m = 0;m<8;m++){
				keystream[index++] = md5_buf[m];
			}		
		}
				
		if (++i == 100) i = 0;
	}
    /*
     * End code I did not write.
     */
	 
	if(j == 1){
		remainder = outlen;
	}else{//len can't be divided by 8
		remainder = outlen%(8*(j-1));
	}
	if(remainder!=0){
		sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, p);
		memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
		MD5((unsigned char *)s, len, (unsigned char *)md5_buf);
		if(mode == 1){//output to stdout
			cout.write(md5_buf,remainder);
		}else if(mode == 0){//output to keystream
			for(int m = 0;m<remainder;m++){
				keystream[index++] = md5_buf[m];
			}	
		}
	}
	
	delete[] s;
	delete[] md5_buf;
}
