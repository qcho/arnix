#include "../system/isr.h"
#include "../../std/printf.c"
#include "../system/in_out.h"
#include "../system/keyboardlisteners.h"

#define KEYBOARD 0x60
#define BUFFER_SIZE 100

#define LSHIFT_KEY_PRESED_SCAN_CODE 42
#define LSHIFT_KEY_RELESED_SCAN_CODE 170
#define RSHIFT_KEY_PRESED_SCAN_CODE 54
#define RSHIFT_KEY_RELESED_SCAN_CODE 182

#define CTRL_KEY_PRESED_SCAN_CODE 29
#define CTRL_KEY_RELESED_SCAN_CODE 157

#define ALT_KEY_PRESED_SCAN_CODE 56
#define ALT_KEY_RELESED_SCAN_CODE 184

#define BLOQ_MAYUS_SCAN_CODE 58

char array[BUFFER_SIZE];

buffer_t stdin;

char * actual_scan_code_table;
char * SCAN_CODE_TABLES[4];

int bloq_mayusc;

int bloq_mayusc_unpresed();
int bloq_mayusc_presed();

int bloq_mayusc_presed(){
	bloq_mayusc=0;
        add_key_listener(BLOQ_MAYUS_SCAN_CODE, bloq_mayusc_unpresed);
	return 0;
}

int bloq_mayusc_unpresed(){
	bloq_mayusc=1;
        add_key_listener(BLOQ_MAYUS_SCAN_CODE, bloq_mayusc_presed);
	return 0;
}

int shift_presed(){
	actual_scan_code_table=SCAN_CODE_TABLES[1];
	return 0;
}

int shift_relesed(){
	actual_scan_code_table=SCAN_CODE_TABLES[0];
	return 0;
}

void IRQ1_handler(registers_t reg){
	int tmp;
	int i=inb(KEYBOARD);
	if(activate(i)){
		tmp=(stdin.end+1)%stdin.size;
		if(tmp!=stdin.start){
			char c=actual_scan_code_table[i];
			if(bloq_mayusc){
				if(c>='a' && c<='z'){
					c=c+'A'-'a';
				}else if(c>='A' && c<='Z'){
					c=c+'a'-'A';
				}
			}
			stdin.array[stdin.end]=c;
			stdin.end=tmp;	
		}else
		{
			printf("buffer lleno");//TODO cambiar por bip
		}
	}
	//printf("%d",i);
}

init_keyboard(){
	register_interrupt_handler(IRQ1,IRQ1_handler);
	stdin.start=stdin.end=0;
	stdin.array=array;
	stdin.size=BUFFER_SIZE;
	add_in_out(0,&stdin);
        load_qcho_scancodes();
        bloq_mayusc=0;
        init_key_listeners();
        add_key_listener(LSHIFT_KEY_PRESED_SCAN_CODE, shift_presed);
        add_key_listener(RSHIFT_KEY_PRESED_SCAN_CODE, shift_presed);
        add_key_listener(LSHIFT_KEY_RELESED_SCAN_CODE, shift_relesed);
        add_key_listener(RSHIFT_KEY_RELESED_SCAN_CODE, shift_relesed);
        add_key_listener(BLOQ_MAYUS_SCAN_CODE, bloq_mayusc_unpresed);
}

load_qcho_scancodes() {
    SCAN_CODE_TABLES[0] = "@@1234567890-+\t@qwertyuiop{}\n@asdfghjkl@@@@@zxcvbnm,.";
    SCAN_CODE_TABLES[0][1] = '\x1B'; // esc
    SCAN_CODE_TABLES[0][14] = '\x08'; // backspace
    SCAN_CODE_TABLES[0][15] = '\t'; // tab
    SCAN_CODE_TABLES[0][57] = ' '; // space
    SCAN_CODE_TABLES[1]="@@!\"#$%&/()=??\t@QWERTYUIOP[]\n@ASDFGHJKL@@@@@ZXCVBNM,.";
    SCAN_CODE_TABLES[1][1] = '\x1B'; // esc
    SCAN_CODE_TABLES[1][14] = '\x08'; // backspace
    SCAN_CODE_TABLES[1][15] = '\t'; // tab
    SCAN_CODE_TABLES[1][57] = ' '; // space
    actual_scan_code_table=SCAN_CODE_TABLES[0];
}
