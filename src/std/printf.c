//para printf
#include "../../include/varargs.h"
#include "../../include/stdarg.h"
#include "../kernel/driver/screen.h"


static void printchar(char c);

static void printf(char *format, ...);

static void prints(char * string);

static char * numberBaseNtoString(unsigned int number, int base, char * out);
//para printf


//myprintfcode

static void printf( char * formatString, ...) {

	int integer;
	unsigned int unsigenedInteger;
	char * string;
	char out[40];

	va_list args;

	va_start(args, formatString);

	while( *formatString !='\0' )
	{
		if(*formatString == '%'){

			formatString++;

			switch(*formatString){
			case 'c' :
				  integer = va_arg(args,char);
				  printchar(integer);
				  break;
			case 's':
				  string = va_arg(args,char *);
				  prints(string);
				  break;
			case 'd' :
				  integer = va_arg(args,int);
				  if(integer < 0){
					  integer = -integer;
					  printchar('-');
				  }
				  prints(numberBaseNtoString(integer,10,out));
				  break;
			case 'u':
				  unsigenedInteger = va_arg(args, unsigned int);
				  prints(numberBaseNtoString(unsigenedInteger,10,out));
				  break;
			case 'o':
				  integer = va_arg(args,unsigned int);
				  prints(numberBaseNtoString(integer,8,out));
				  break;
			case 'x':
				  unsigenedInteger = va_arg( args, unsigned int);
				  prints(numberBaseNtoString(unsigenedInteger,16,out));
				  break;
			case '%':
				  printchar('%');
				  break;
			}
		} else if( *formatString == '\\' ){
			formatString++;
			switch(*formatString){
			case 'n':
				  printchar('\n');
				  break;
			case 't':
				  printchar('\t');
				  break;
			case '\\':
 				  printchar('\\');
				  break;
			}

		} else{
			printchar(*formatString);
		}
		formatString++;
	}
	va_end(argp);
}

static void prints(char * string){
	while(*string != '\0'){
		printchar(*string);
		string++;
	}
}


char * numberBaseNtoString(unsigned int number, int base, char * out){

	int digits[40];
	int position = 0;
	char * numbers = "0123456789ABCDEF";
	int index = 0;

	if( number != 0 ){
		while( number > 0 ){
			if(number < base) {
			      digits[position] = number;
			      number = 0;
			} else {
			      digits[position] = number % base;
			      number /= base;
			}
			position++;
		}

		for( index = 0 ; position > 0 ; position--, index++ ){
		      out[index] = numbers[digits[position-1] % base];
		}
		out[index] = '\0';
	} else {
		out[0] = '0';
	}

	return out;
}

static void printchar(char c){
	screen_put(c);
}

//end myprintfcode
