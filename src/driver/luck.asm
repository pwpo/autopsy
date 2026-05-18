.code



readvm PROC
	mov r10, rcx
	mov eax, 63
	syscall
	ret
readvm ENDP

writevm PROC
	mov r10, rcx
	mov eax, 58
	syscall
	ret
writevm ENDP

END
