#include "printf.c"

char stream[100];
char * streamp=stream;

char getchar(){
	char c=*streamp;
	if(c=='\0'){
		streamp=stream;
		stream[0]='\0';
		while(stream[0]=='\0'){
			__read(0,stream,100);
		}
		c=*streamp;
	}
	streamp++;
	return c; 
	
}
