#include "../include/kasm.h"
#include "../include/defs.h"
#include "driver/screen.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

int tickpos=0;

void int_08() {
    screen_setForeColour(tickpos%15);
    if (tickpos++ < 80*23) {
        screen_put('O');
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
        screen_write("Hello, world!");
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

        while(1)
        {
        }

}

