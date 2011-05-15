#include "../include/kasm.h"
#include "../include/defs.h"
#include "kernel/driver/screen.h"

//para printf
#include "../include/varargs.h"
#include "../include/stdarg.h"


static void printchar(char c);

static int getNumber(const char ** format);

static int pow(int num, int exponent);

static void printchar(char c);

static void putSpaces(int count);

static void putZeroz(int count);

static void printf(char *format, ...);

static void prints(char * string);

char * numberBaseNtoString(unsigned int number, int base, char * out);
//para printf

DESCR_INT idt[0x80];			/* IDT de 80 entradas*/
IDTR idtr;				/* IDTR */

int tickpos=0;

void int_08() {
    screen_setForeColour(tickpos%15);

    if (tickpos++ < 500) {

// 	c s d u o x \n \t
//  	printf("te%cst  ",'j');
//  	printf("te%sst  ","leeol");
//  	printf("te%dst  ",0);
  	printf("te%ust  ",3554);
	printf("%o",9);
	printf("\n");
 	printf("%x",15);
	printf("\n");
//  	printf("te\nst  ");
//  	printf("te\tst  ");
//  	printf("te\\st  ");
	while(1){}
    } else {
    	outb(0x64,0xFE);
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
//         screen_write("Hello, world!");
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

//myprintfcode
#define ALINEATION_LEFT 0
#define ALINEATION_RIGHT 1
#define NUMBER_WITHOUT_SIGNS 0
#define NUMBER_WITH_SIGNS 1
#define FALSE 0
#define TRUE 1

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
