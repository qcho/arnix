global _read

SECTION .text

_read:
	mov eax,3
	mov ebx,0
	int 80h
	ret
