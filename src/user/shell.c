#include "shell.h"

#include "../std/stdio.h"
#include "../std/printf.c"
#include "commands.h"
#include "timer.h"

#define NULL 0
#define COMAND_LINE_MAX 1000


char * name="user";
char * pcname="thispc";

void my_strcpy(char * str_des,char * str_ori){
	int i;
	for(i=0;str_ori[i]!='\0';i++){
		str_des[i]=str_ori[i];
	}
	str_des[i]='\0';
}

int my_strlen(char* str){
	int i;
	for(i=0;str[i]!='\0';i++);
	return i;
}



char * strnormalise(char * str){
        int j, i;
        // cambia enters por espacios
        for(j=0;str[j]!='\0';j++){
                if(str[j]=='\n' || str[j] == '\t'){
                        str[j]=' ';
                }
        }
        // elimina espacios del principio
        while(str[0]==' '){
                str=str+1;
        }
        //elimina espacios del final
        for(i=my_strlen(str)-1;i>0 && str[i]==' ';i--){
                str[i]='\0';
        }
        //elimina espacios repetidos en el medio
        for(j=0;str[j]!='\0';j++){
                if(str[j]==' ' && str[j+1]==' '){
                        my_strcpy(str + j, str + j + 1);
                        j--;
                }
        }
        return str;
}



void printuser(){
	printf("%s@%s:",name,pcname); 
}

int execute(char* comand,int argcant,char * argvec[]){
	if(comand[0]=='\0'){
		return 0;
	}
	main start=get_command(comand);
	if(start==NULL){
		printf("invalid comand: %s\n",comand);
		return -1;
	}
	return start(argcant,argvec);
}

int parseline(){
	char c;
	int i=0;
	char comand_line[COMAND_LINE_MAX];
	while((c=getchar())!='\n' && i<COMAND_LINE_MAX-3){
		comand_line[i]=c;
		i++;
	}
	if(i>=COMAND_LINE_MAX-3){
		printf("\n");
	}
	comand_line[i]='\0';
	char* comand=strnormalise(comand_line);
	int argcant=0;
	char * argvec[50];
	for(i=0;comand[i]!='\0';i++){
		if(comand[i]==' '){
			comand[i]='\0';
			argvec[argcant]=&comand[i+1];
			argcant++;
		}
	}
	return execute(comand,argcant,argvec)==-15;
}

int exit_shell(int argc,char* argv[]){
	  long int ret1,ret2,ret3;
	  int resp;
	  double aux;
	  
	  asm ("rdtsc" : "=A"(ret1) );
	  startTicks();
	  while(getTicks()=<20);
	  stopTicks();
	  asm ("rdtsc" : "=A"(ret2) );
	  ret3 = ret2-ret1;
	  printf("%d\n",ret3);
	  aux = (double)((ret3*1000)/(20*55*1024*1024));
	  printf("%d",(int)aux);
	  resp = (int)aux;
	  printf("%d\n",resp);
}

int echo_shell(int argc,char* argv[]){
	int i;
	for(i=0;i<argc;i++){
		printf("%s\n",argv[i]);
	}
	return 0;
}


void shell_start(){
	int exit=0;
	add_command("echo",echo_shell);	
	add_command("exit",exit_shell);
	while(!exit)
	{
		printuser();
		exit=parseline();
	}
}



