.global .main
.align 4

.main:
	; x29 is our sfb
	;x15 is our sp
	mov x29, sp
	mov x15, sp
	
	; exit program gracefully
	mov x0, #0
	mov x16, #1
	svc #0x80