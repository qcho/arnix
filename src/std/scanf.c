#include "../../src/std/string.h"
#include "stdio.h"

static int isNumber(char c) {
    return (c >= '0' && c <= '9');
}

int sscanf(char *stream, char *format, ...) {
	va_list ap;
	va_start ( ap, format );
        int i = 0;
        int j = 0;
        int converted;
        
        int *integer, iTmp;
        char* string;
        char *chr;
        
        while (format[i]) {
            if (format[i] == '%') {
                i++;
                switch(format[i++]) {
                    case 'c':
                        chr = va_arg(ap, char*);
                        *chr = stream[j++];
                        break;
                    case 'd':
                        integer = va_arg(ap, int *);
                        iTmp = 0;
                        while (isNumber(stream[j])) {
                            iTmp = iTmp*10+(stream[j]-'0');
                            j++;
                        }
                        *integer = iTmp;
                        break;
                    case 's':
                        string = va_arg(ap, char *);
                        iTmp = 0;
                        while (stream[j] != ' ') {
                            string[iTmp++] = stream[j++];
                        }
                        string[iTmp] = '\0';
                        break;
                    default:
                        // WRONG %X
                        return converted;
                }
            } else {
                if(format[i] == stream[j]) {
                    i++;
                    j++;
                } else {
                    //WRONG FORMAT STRING
                    return converted;
                }
            }
        }
}
