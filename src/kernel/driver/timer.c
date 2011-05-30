#include "../system/isr.h"
#include "../system/int80.h"
#include "../driver/screen.h"

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

void start_ticks(){
	count_ticks=1;
	ticks=0;
}

void stop_ticks(){
	count_ticks=0;
}

int get_ticks(){
	return ticks;
}

void cpu_speed(registers_t regs){
	screen_write("mamam");
	uint32_t k,j,t;
	start_ticks();
	k=getRDTSC();
	while((t=get_ticks())<30);
	k=getRDTSC()-k;
	stop_ticks();
	regs.eax=(k/t)*18+k/(t*5);
}

void init_timer_tick(){
	sub_func_count=0;
	count_ticks=0;
	register_functionality(15,cpu_speed);
	register_interrupt_handler(IRQ0,IRQ0_handler);
}



