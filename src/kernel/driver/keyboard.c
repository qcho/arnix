#include "../system/isr.h"
#include "../../std/printf.c"


#define KEYBOARD 0x60

void IRQ1_handler(registers_t reg){
	printf("%d",inb(KEYBOARD));
}


init_keyboard(){
	register_interrupt_handler(IRQ1,IRQ1_handler);
}
