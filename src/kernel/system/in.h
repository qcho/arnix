#ifndef IN_H
#define IN_H


struct buffer_struct
{
	int buffer_size;
	char * array;
	int start;
	int end;
};

typedef struct buffer_struct buffer;

#endif // IN_H
