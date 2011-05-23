#include "stdio.h"
#include "printf.c"

#define STREAM_SIZE 500

typedef int (*flusher)(char * streampointer);

char stream[STREAM_SIZE];
char * streamout=stream;


int introflush(char * streampointer){
	for(;*streampointer!='\0';streampointer++){
		if(*streampointer=='\n' || STREAM_SIZE-(streampointer-stream)-1){
			return 1;
		}
	}
	return 0;
}

void delete_stream_backspaces(){
	int i, j;
	for(i=0;stream[i]!='\0';i++){
		if(stream[i]=='\b'){
			for(j=i;stream[j]=='\b';j--);
			stream[j]='\b';
		}
	}
	for(i=0;stream[i]!='\0';i++){
		if(stream[i]=='\b'){
			for(j=i;stream[j]=='\b';j++);
			stream[i]=stream[j];
			stream[j]='\b';
		}
	}
	i--;
	for(;stream[i]=='\b';i--);
	i++;
	stream[i]='\0';

}


char getchar(){
	char c=*streamout;
	if(c=='\0'){
		streamout=stream;
		char * streamin=stream;
		stream[0]='\0';
		while(!introflush(streamin)){
			for(;*streamin!='\0';streamin++);
			__read(0,streamin,STREAM_SIZE-(streamin-stream)-1);
			printf(streamin);
		}
		//delete_stream_backspaces();
		c=*streamout;
	}
	streamout++;
	return c; 
	
}


