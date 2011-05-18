#ifndef KEYBOARDLISTENER_H
#define KEYBOARDLISTENER_H

#define MAX_SCAN_CODE 300

typedef int (*key_listener)();

int activate(int scan_code);
void add_key_listener(int scan_code, key_listener listener);
void init_key_listeners();

#endif //KEYBOARDLISTENER_H
