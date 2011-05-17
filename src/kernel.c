#include "../include/kasm.h"
#include "../include/defs.h"
#include "../src/std/printf.c"
#include "kernel/driver/screen.h"
#include "kernel/system/idt.h"
#include "kernel/driver/keyboard.h"

DESCR_INT idt[0x80];			/* IDT de 80 entradas*/
IDTR idtr;				/* IDTR */

int tickpos=0;

void int_08() {
    screen_setForeColour(tickpos%15);

    if (tickpos++ < 500) {

// 	c s d u o x \n \t
//  	printf("te%cst  ",'j');
//  	printf("te%sst  ","leeol");
//  	printf("te%dst  ",0);
//  	printf("te%ust  ",3554);
//	printf("%o",9);
//	printf("\n");
//	printf("%x",15);
//	printf("\n");
//  	printf("te\nst  ");
//  	printf("te\tst  ");
  	printf("te\\st  ");
    } else {
    	outb(0x64,0xFE);
    }
}


/**********************************************
kmain()
Punto de entrada de cóo C.
*************************************************/

kmain()
{

        int i,num;

/* Borra la pantalla. */

        screen_clear();
//         screen_write("Hello, world!");
        screen_setBackColour(LIGHT_GREY);

/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

	setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);

/* Carga de IDTR    */

	idtr.base = 0;
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;

	_lidt (&idtr);

	_Cli();
/* Habilito interrupcion de timer tick*/

        _mascaraPIC1(0xFE);
        _mascaraPIC2(0xFF);

	_Sti();
	
	init_descriptor_tables();
	init_keyboard();

        while(1)
        {
        }

}
