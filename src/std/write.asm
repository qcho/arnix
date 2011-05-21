global __write

SECTION .text

__write:
    	mov ecx, [esp+8]
	mov eax,4
	mov ebx, [esp+4]
	mov edx, [esp+12]
	int 80h
	ret
