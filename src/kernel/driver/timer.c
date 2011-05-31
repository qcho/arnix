#include "../system/isr.h"
#include "../system/int80.h"

#define SUB_FUNC_VEC_SIZE 10

int80_t sub_handler_vec[SUB_FUNC_VEC_SIZE];

int ticks;
int count_ticks;
int sub_func_count;

void register_tick_subhandler(int80_t func) {
	if(sub_func_count<SUB_FUNC_VEC_SIZE-1){
	    	sub_handler_vec[sub_func_count] = func;
		sub_func_count++;
	}
}


void IRQ0_handler(registers_t regs){
	int i;
	if(count_ticks){
		ticks++;
	}
	for(i=0;i<sub_func_count;i++){
		sub_handler_vec[i](regs);
	}
}

void cpu_speed(registers_t regs){
	unsigned long k;
	count_ticks=1;
	ticks=0;
        int TICK_COUNT = 30;
	_Sti();
	k=getRDTSC();
	while(ticks<TICK_COUNT);
	k=getRDTSC()-k;
	_Cli();
	count_ticks=0;
	*((unsigned long*)regs.ebx)=(k/TICK_COUNT)*18+k/(TICK_COUNT*5);
}

void init_timer_tick(){
	sub_func_count=0;
	count_ticks=0;
	register_interrupt_handler(IRQ0,IRQ0_handler);
	register_functionality(5,cpu_speed);
}



