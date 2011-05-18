#include "int80.h"
#include "in.h"
#include "../../std/printf.c"

buffer * in_vector[10]; 

void READ_INTERRUPT_handler(registers_t regs){
	buffer * buff=in_vector[regs.ebx];
	if(buff->start!=buff->end){
		printf("%c",buff->array[buff->start]);//esto es lo que deberia devolver en realidad, esta asi para prueba
		buff->start=(buff->start+1)%(buff->buffer_size+1);
	}
}

void add_in(int n, buffer * buff){
	in_vector[n]=buff;
}


init_in(){
	register_functionality(3,READ_INTERRUPT_handler);
}
