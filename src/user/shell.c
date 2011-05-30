#include "shell.h"

#include "../std/stdio.h"
#include "../std/printf.c"
#include "commands.h"

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
		comand_line[i]=c;
		i++;
	}
	if(i>=COMAND_LINE_MAX-3){
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
	  long int ret1,ret2,ret3;
	  
	  long int li=15000076;
	  int i=9800032;
	  
	  float fi = 5.5;
	  double di = 5.5;
	  int j ;
	  j= (int)di;
	  
	  double a = 5.5;
	  int b ;
	  b = (int)a;
	   printf("%d\n",b);
	  printf("%d %d\n",3 , b);
	  printf("%d\n",b);

 	  printf("%d\n",j);
 	  printf("%d\n",(int)di);
 	  printf("%d\n",(int)fi);
	  asm ("rdtsc" : "=A"(ret1) );
	  
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  asm ("xor %eax,%edx");
	  asm("xor %edx,%eax");
	  
	  //30 xor me aseguran 30 timer ticks
	  asm ("rdtsc" : "=A"(ret2) );
	  printf("%d\n",ret1);
	  printf("%d\n",ret2);
	  ret3 = ret2-ret1;
	  printf("%d\n",ret3);
	  double aux;
	  aux = (double)((ret3*1000000)/(30*0.055));
	  int resp;
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



