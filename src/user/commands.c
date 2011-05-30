#include "commands.h"

#include "../std/stdlib.h"

#define NULL 0
#define COMMAND_MAX_CANT 20

command_t command_list[COMMAND_MAX_CANT];
int commands_added=0;



void add_command(char * name,main function){
	if(commands_added<COMMAND_MAX_CANT){
		command_list[commands_added].name=name;
		command_list[commands_added].start=function;
		commands_added++;
	}
}

main get_command(char * name){
	int i;
	for(i=0;i<commands_added;i++){
		if(!strcmp(command_list[i].name,name)){
			return command_list[i].start;
		}
	}
	return NULL;
}

char * autocomplete(char * name){
	int i,j;
	int flag;
	char ret[200];
	for(i=0;i<200;i++){
		ret[i]=' ';
	}
	for(i=0;i<200 && name[i]!='\0';i++){
		ret[i]=name[i];
	}
	for(i=0;i<commands_added;i++){
		flag=1;
		for(j=0;command_list[i].name[j]!='\0' && name[j]!='\0' ;j++){
			if(command_list[i].name[j]!=name[j]){
				flag=0;
			}
		}
		if(name[j]=='\0' && flag){
			for(;command_list[i].name[j]!='\0' && ret[j]!='\0';j++){
				if(ret[j]==' ' || ret[j]==command_list[i].name[j]){
					ret[j]=command_list[i].name[j];
				}
			}
			ret[j]='\0';
		}
	}
	return ret;
}
