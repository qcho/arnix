#include "../include/kasm.h"
#include "../include/defs.h"
#include "kernel/driver/screen.h"
#include "kernel/system/idt.h"
#include "kernel/driver/keyboard.h"
#include "kernel/system/keyboardlisteners.h"

//#include "std/printf.c"

DESCR_INT idt[0x80];			/* IDT de 80 entradas*/
IDTR idtr;				/* IDTR */

int tickpos=0;

/**********************************************
kmain()
Punto de entrada de cóo C.
*************************************************/

kmain()
{		
	init_descriptor_tables();
	init_int80();
	init_in_out();
	init_keyboard();
	init_screen();	
	shell_start();
}
