#include "keyboardlisteners.h"


key_listener KEY_LISTENERS[MAX_SCAN_CODE];


int print(){
	return 1;
}

int no_print(){
	return 0;
}

int activate(int scan_code){
	return KEY_LISTENERS[scan_code]();
}

void add_key_listener(int scan_code, key_listener listener){
	KEY_LISTENERS[scan_code]=listener;
}

void init_key_listeners(){
        int i;
        for(i=0;i<58;i++){
        	add_key_listener(i, print);
        }
        for(i=58;i<MAX_SCAN_CODE;i++){
        	add_key_listener(i, no_print);
        }
}
