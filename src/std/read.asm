global __read

SECTION .text

__read:
    	mov ecx, [esp+8]
	mov eax,3
	mov ebx, [esp+4]
	mov edx, [esp+12]
	int 80h
	ret
