global __cpuspeed

__cpuspeed:
	 mov ebx, [esp+4]
	 mov eax,5
	 int 80h
	 ret