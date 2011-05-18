#include "../system/isr.h"
#include "../../std/printf.c"
#include "../system/int80.h"

#define KEYBOARD 0x60
#define BUFFER_SIZE 200

char buffer[BUFFER_SIZE];
int buffer_start=0;
int buffer_end=0;


char * SCAN_CODE_TABLE="--1234567890?¿\tqwertyuiop[]\n-asdfghjklñ{}-<zxcvbnm,.---- ";

void IRQ1_handler(registers_t reg){
	int tmp;
	int i=inb(KEYBOARD);
	if(i<=0x39){
		tmp=(buffer_end+1)%(BUFFER_SIZE+1);
		if(tmp!=buffer_start){
			buffer[buffer_end]=SCAN_CODE_TABLE[i];
			buffer_end=tmp;
		}else
		{
			printf("buffer lleno");//TODO cambiar por bip
		}
	}
}

void READ_INTERRUPT_handler(registers_t regs){
	if(buffer_start!=buffer_end){
		printf("%c",buffer[buffer_start]);//esto es lo que deberia devolver en realidad, esta asi para prueba
		buffer_start=(buffer_start+1)%(BUFFER_SIZE+1);
	}
}


init_keyboard(){
	register_interrupt_handler(IRQ1,IRQ1_handler);
	register_functionality(3,READ_INTERRUPT_handler);
        load_qcho_scancodes();
}

load_qcho_scancodes() {
    SCAN_CODE_TABLE = "@@1234567890-+\t@qwertyuiop{}@@asdfghjkl@@@@@zxcvbnm,.";
    SCAN_CODE_TABLE[1] = '\x1B'; // esc
    SCAN_CODE_TABLE[14] = '\x08'; // backspace
    SCAN_CODE_TABLE[15] = '\t'; // tab
    SCAN_CODE_TABLE[57] = ' '; // space
}
