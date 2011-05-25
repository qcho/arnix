#include "stdio.h"
#include "printf.c"

#define STREAM_SIZE 500

typedef int (*flusher)(char * streampointer);

char stream[STREAM_SIZE];
char * streamout=stream;


int intro_flush(char * streampointer){
	if(*streampointer=='\n' || 1>=STREAM_SIZE-(streampointer-stream)-1){
		return 1;
	}
	
	return 0;
}

void delete_stream_backspaces(){
	int i, j;
	for(i=0;stream[i]!='\0';i++){
		if(stream[i]=='\b'){
			for(j=i;stream[j]=='\b';j++);
			if(stream[j]=='\0'){
				stream[i+1]='\0';
			}
			stream[i]=stream[j];
			stream[j]='\b';
		}
	}
	i--;
	for(;i>=0 && stream[i]=='\b';i--);
	i++;
	stream[i]='\0';
}


char getchar(){
	char c=*streamout;
	if(c=='\0'){
		streamout=stream;
		char * streamin=stream;
		int i,j;
		for(i=0;i<STREAM_SIZE;i++){
			stream[i]='\0';	
		}
		while(!intro_flush(streamin)){
			if(*streamin!='\0')
				streamin++;
			__read(0,streamin,1);
			if(*streamin!='\b'){
				printf(streamin);
			}
			else{
				for(j=streamin-stream;j>=0 && stream[j]=='\b';j--);
				if(j!=-1){
					printf("\b");
					stream[j]='\b';	
				}
			}
		}
		delete_stream_backspaces();
		c=*streamout;
	}
	streamout++;
	return c; 
	
}


