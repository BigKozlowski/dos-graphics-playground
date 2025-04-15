	IDEAl
	model SMALL
	
BufSize	EQU	255
ASCnull	EQU	0
ASCcr	EQU	13
ASClf	EQU	10

STRUC StrBuffer
	maxlen	db BufSize
	strlen	db 0
	chars db BufSize DUP (?)
ENDS StrBuffer

	DATASEG
buffer	StrBuffer	<>

	CODESEG
	
	EXTRN StrLength:proc, StrCopy:proc
	PUBLIC StrRead, StrWrite, StrWrite2, NewLine
	
%NEWPAGE
PROC StrRead
	or	cl,cl
	jz	@@99
	
	push	ax
	push	bx
	push	dx
	push	si
	
	mov	[buffer.maxlen],cl
	mov	ah,0ah
	mov	dx,offset buffer.maxlen
	int	21h
	xor	bh,bh
	mov	bl,[buffer.strlen]
	mov	[bx+buffer.chars],ASCnull
	mov	si,offset buffer.chars
	call	StrCopy
	
	pop	si
	pop	dx
	pop	bx
	pop	ax
@@99:
	ret
ENDP StrRead

%NEWPAGE
PROC StrWrite
	call	StrLength
PROC StrWrite2
	push	ax
	push	bx
	push	dx
	
	mov	bx,1
	mov	dx,di
	mov	ah,40h
	int 21h
	
	pop	dx
	pop	bx
	pop	ax
	ret
ENDP StrWrite2
ENDP StrWrite

%NEWPAGE
PROC NewLine
	mov	ah,2
	mov	dl,ASCcr
	int	21h
	mov	dl,ASClf
	int	21h
	ret
ENDP NewLine

END