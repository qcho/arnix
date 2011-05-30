#include "shell.h"

#include "../std/stdio.h"
#include "../std/printf.c"
#include "../std/stdlib.h"

#include "commands.h"

#define NULL 0
#define COMAND_LINE_MAX 1000


char * name="user";
char * pcname="thispc";


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
        for(i=strlen(str)-1;i>0 && str[i]==' ';i--){
                str[i]='\0';
        }
        //elimina espacios repetidos en el medio
        for(j=0;str[j]!='\0';j++){
                if(str[j]==' ' && str[j+1]==' '){
                        strcpy(str + j, str + j + 1);
                        j--;
                }
        }
        return str;
}

void printuser(){
	printf("\x1B[32m%s@%s:\x1B[0m",name,pcname); 
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
		if(c=='\t'){
			strcpy(comand_line[strlen(comand_line)],autocomplete(comand_line));
			__write(0,comand_line,strlen(comand_line));
		}
		comand_line[i]=c;
		i++;
	}
	if(i>=COMAND_LINE_MAX-3){
		while(getchar()!='\n');
		printf("\n");
	}
	comand_line[i]='\0';
	char* command=strnormalise(comand_line);
	int argcant=0;
	char * argvec[50];
        int in_quotes = 0;
	for(i=0;command[i]!='\0';i++){
		if(command[i]==' ' && !in_quotes){
			command[i]='\0';
			argvec[argcant]=&command[i+1];
			argcant++;
                } else if (command[i]=='"') {
                    in_quotes = !in_quotes;
                }
	}
	return execute(command,argcant,argvec)==-15;
}

int exit_shell(int argc,char* argv[]){
	  return -15;
}

int echo_shell(int argc,char* argv[]){
	int i;
	for(i=0;i<argc;i++){
		printf("%s\n",argv[i]);
	}
	return 0;
}

int getCPUspeed_shell(){
	int ips;
	__cpuspeed(&ips);
	printf("Su procesador esta ejecutando %d instrucciones por segudo.\n",ips);
	printf("La velocidad en MHz es:%d.%d MHz\n",(ips)/(1024*1024),((10*ips)/(1024*1024))%10);
	return 0;
}

int clear_shell(){
	printf("\x1B[2J");
	return 0;
}

void shell_start(){
	int exit=0;
	add_command("echo",echo_shell);	
	add_command("exit",exit_shell);	
	add_command("getCPUspeed",getCPUspeed_shell);
	add_command("clear",clear_shell);
	while(!exit)
	{
		printuser();
		exit=parseline();
	}
}



