#include "commands.h"
#include "../std/printf.c"

#define COMMAND_MAX_CANT 20

command_t command_list[COMMAND_MAX_CANT];
int commands_added=0;

int my_strcmp(char* str1, char * str2){
	int i;
	for(i=0;str1[i]!='\0' && str1[i]!='\0' ;i++){
		if(str1[i]!=str2[i]){
			return 0;
		}
	}
	if(str1[i]=='\0' && str2[i]=='\0'){
		return 1;
	}
	return 0;
}

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
		if(my_strcmp(command_list[i].name,name)){
			return command_list[i].start;
		}
	}
	return NULL;
}


