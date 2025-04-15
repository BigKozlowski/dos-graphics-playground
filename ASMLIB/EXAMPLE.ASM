%title "Com file shell"

	ideal
	model tiny
	stack 256

MaxLen	EQU	128
cr	EQU	13
lf	EQU	10

	dataseg
greet db	"This is test string", cr, lf, lf, 0
testString	DB	MaxLen	DUP	(0), 0
exCode	DB	0

	codeseg
	EXTRN	StrRead:proc, StrWrite:proc, NewLine:proc

Start:
	mov	ax,@data
	mov	ds,ax
	mov	es,ax
	mov	di,offset greet
	call 	StrWrite
	
	mov	di,offset testString
	mov	cx, MaxLen
	call	StrRead
	call	NewLine
	call	StrWrite
Exit:
	mov	ah,	04Ch
	mov     al,	[exCode]
	int	21h

end	Start
