#include "shell.h"

#include "../std/stdio.h"
#include "../std/printf.c"
#include "commands.h"

#define NULL 0



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



printuser(){
	printf("%s@%s:",name,pcname); 
}

int execute(char* comand){
	if(comand[0]=='\0'){
		return 0;
	}
	main start=get_command(comand);
	if(start==0){
		printf("invalid comand: %s\n",comand);
		return -1;
	}
	return start(0,NULL);
}

int parseline(){
	char c;
	int i=0;
	char comand_line[300];
	while((c=getchar())!='\n'){
		comand_line[i]=c;
		i++;
	}
	comand_line[i]='\0';
	char* comand=strnormalise(comand_line);
	return execute(comand)==-15;
}

int exit_shell(int argc,char* argv){
	return -15;
}


void shell_start(){
	int exit=0;
	add_command("exit",(main)exit_shell);
	while(!exit)
	{
		printuser();
		exit=parseline();
	}
}



