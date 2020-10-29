#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include <errno.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>		// for open
 
/* Not technically required, but needed on some UNIX distributions */
#include <sys/types.h>
#include <sys/stat.h>
#include <openssl/hmac.h>
#include <openssl/md5.h>
//#include<string.h>
	
#define BUFFER_LENGHT 4097

char buffer[BUFFER_LENGHT];


int main()

{
		unsigned char digest[16];
		system("md5sum file.tar.gz");
		//	printf("-1\n");
		memset(buffer, 0, BUFFER_LENGHT);
		int filedesc = open ("file.tar.gz",O_RDONLY);
		//	printf("0\n");
		//int filedesc = fopen("file.tar.gz", "r+");
		struct stat sb;
		//printf("00\n");
		if(lstat("file.tar.gz",&sb)==-1)
		{
			perror("lstat");
			exit(EXIT_FAILURE);
		}
		//printf("011\n");
		long long fsize = sb.st_size;
		printf("File size: %lld bytes\n",fsize);
		//printf("0111\n");
		if(( 0 == filedesc ))
		{
			printf("Error reached\n\n");		
			return -1;
		}
		
		read(filedesc,buffer,fsize);
		
		MD5_CTX md5;
		
		MD5_Init(&md5);

		//printf("\n size of file : %ld\n\n" , fsize);
		long long length = fsize;
		long long str1=0;
		//string str(buffer);
		char * str = buffer;

		while (length > 0) {
        if (length > 512) {
            MD5_Update(&md5, str, 512);
        } else {
        	//printf("I read here when values of lenght %lld and str %lld ", length, str);
            MD5_Update(&md5, str, length);
        }
        length -= 512;
        str += 512;
    	}
       // printf("Final values of lenght %lld and str %lld ", length, str);

		//MD5_Update(&md5,buffer,fsize);
		MD5_Final(digest,&md5);
		//printf("I got here \n");
		printf("\nmd5sum is\n\n");
		printf("%x",digest);
		printf("\n mah num \n ");
		//char md5string[33];
		int n;
		char *out = (char*)malloc(33);
		for (n = 0; n < 16; ++n) {
        snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    	}
    	printf("%s\n",out);
    	fflush(stdin);


return 0;
}
