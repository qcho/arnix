#include "commands.h"

#include "../std/string.h"

#define NULL 0
#define COMMAND_MAX_CANT 20

command_t command_list[COMMAND_MAX_CANT];
int commands_added=0;

char** get_command_list() {
    char* commands[COMMAND_MAX_CANT];
    int i;
    for(i=0;i<commands_added;i++) {
        commands[i] = command_list[i].name;
    }
    commands[i] = NULL;
    return commands;
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
		if(!strcmp(command_list[i].name,name)){
			return command_list[i].start;
		}
	}
	return NULL;
}