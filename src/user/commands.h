#ifndef COMMANDS_H
#define COMMANDS_H

typedef int (*main)(int argc,char * argv[]);


struct command_struct
{
    char * name;
    main start;
};

typedef struct command_struct command_t;

void add_command(char * name,main function);
main get_command(char * name);

#endif //COMMANDS_H
