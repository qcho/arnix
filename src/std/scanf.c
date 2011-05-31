#include "printf.c"
#include "../../src/std/string.h"
#include "../../include/varargs.h"
#include "../../include/stdarg.h"

int( readFromStr)(char *formatString, char *format, ...) {
	va_list ap;
	va_start ( ap, format );
	float *f;
	int conv = 0, *integer, index, resp = 0,j;
	char *a, *fp, *sp = formatString, buf[256] = { '\0' };

	for (fp = formatString; *fp != '\0'; fp++) {
		for (index = 0; *sp != '\0' && *sp != ' '; index++) {
			buf[index] = *sp++;
		}
		buf[index] = '\0';
		while (*sp == ' ') {
			sp++;
		}
		while (*fp != '%') {
			fp++;
		}
		if (*fp == '%') {
			switch (*++fp) {
			case 'd':
				integer = va_arg ( ap, int * );
				for (j = 0; *fp != '\0' && *fp != ' '; fp++, j++) {
					resp += ((*fp) - '0') * (10 ^ j);
				}
				*integer = resp;
				break;
			case 's':
				a = va_arg ( ap, char * );
				strncpy(buf, a);
				break;
			}
			conv++;
		}
	}
	va_end ( ap );
	return conv;
}
