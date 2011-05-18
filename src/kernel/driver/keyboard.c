#include "../system/isr.h"
#include "../../std/printf.c"
#include "../system/in.h"

#define KEYBOARD 0x60
#define BUFFER_SIZE 200
char array[BUFFER_SIZE];

buffer stdin;


char * SCAN_CODE_TABLE="--1234567890?¿\tqwertyuiop[]\n-asdfghjklñ{}-<zxcvbnm,.---- ";

void IRQ1_handler(registers_t reg){
	int tmp;
	int i=inb(KEYBOARD);
	if(i<=0x39){
		tmp=(stdin.end+1)%(stdin.buffer_size+1);
		if(tmp!=stdin.start){
			stdin.array[stdin.end]=SCAN_CODE_TABLE[i];
			stdin.end=tmp;
		}else
		{
			printf("buffer lleno");//TODO cambiar por bip
		}
	}
}

init_keyboard(){
	register_interrupt_handler(IRQ1,IRQ1_handler);
	stdin.start=stdin.end=0;
	stdin.array=array;
	stdin.buffer_size=BUFFER_SIZE;
	add_in(0,&stdin);
        load_qcho_scancodes();
}

load_qcho_scancodes() {
    SCAN_CODE_TABLE = "@@1234567890-+\t@qwertyuiop{}\n@asdfghjkl@@@@@zxcvbnm,.";
    SCAN_CODE_TABLE[1] = '\x1B'; // esc
    SCAN_CODE_TABLE[14] = '\x08'; // backspace
    SCAN_CODE_TABLE[15] = '\t'; // tab
    SCAN_CODE_TABLE[57] = ' '; // space
}
