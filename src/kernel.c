#include "../include/kasm.h"
#include "../include/defs.h"
#include "kernel/driver/screen.h"
#include "kernel/system/idt.h"
#include "kernel/driver/keyboard.h"
#include "kernel/system/keyboardlisteners.h"

DESCR_INT idt[0x80];			/* IDT de 80 entradas*/
IDTR idtr;				/* IDTR */

/**********************************************
kmain()
Punto de entrada de codigo C.
*************************************************/

kmain()
{
	init_descriptor_tables();
	init_int80();
	init_in_out();
	init_keyboard();
	init_timer_tick();
	init_screen();


	shell_start();
}
