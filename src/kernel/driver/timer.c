#include "timer.h"
#include "../system/isr.h"
#include "../system/in_out.h"
#include "ascii.h"



int ticks;

int getTicks(){
  return ticks;
}
void startTicks(){
  ticks = 0;
}
void stopTicks(){
  ticks = 0;
}


void IRQ0_handler(registers_t reg){
	ticks++;
}

void init_timer(){
	register_interrupt_handler(IRQ0,IRQ0_handler);
}