global _read

SECTION .text

_read:
	mov eax,3
	int 80h
	ret
