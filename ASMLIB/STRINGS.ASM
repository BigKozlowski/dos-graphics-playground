%TITLE "��楤��� ��� ࠡ��� � ���-�ନ��஢���묨 ��ப���"

	ideal
	
	model small
	
	codeseg
	public MoveLeft, MoveRight, StrNull, StrLength
	public StrUpper, StrCompare, StrDelete, StrInsert
	public StrConcat, StrCopy, StrPos, StrRemove
	
;---------------------------------------------------------------------
; MoveLeft	- ��६������� � ��஭� �����⠭�� �����ᮢ
; MoveRight	- ��६������� � ��஭� �뢠��� �����ᮢ
; StrNull	- 㤠���� �� ᨬ���� �� ��ப�
; StrLength	- �������� ���-�� ᨬ����� � ��ப�
; StrUpper	- �८�ࠧ����� �� ᨬ���� � �ய���
; StrCompare	- �ࠢ���� ��� ��ப� � ��䠢�⭮� ���浪�
; StrDelete	- 㤠���� ᨬ���� �� ��ப�
; StrInsert	- ��⠢��� ᨬ���� � ��ப�
; StrConcat	- ��ᮥ������ ���� ��ப� � ��㣮�
; StrCopy	- ᪮��஢��� ���� ��ப� � �����
; StrPos	- ���� � ��ப� ������ �����ப�
; StrRemove	- 㤠���� �� ��ப� �����ப�
;
; � ����ணࠬ���� �⮣� ����� ���ࠧ㬥������, �� ES � DS �������
; ���� � �� �� ᥣ����. ��⠭���� ES=DS �� �맮�� �� �� ��� ����ணࠬ�
; ---------------------------------------------------------------------

ASCNull EQU 0

%NEWPAGE
PROC MoveLeft
	jcxz	@@99
	push	cx
	push	si
	push	di
	
	add	si,bx
	add	di,dx
	cld
	rep	movsb
	
	pop	di
	pop	si
	pop	cx
@@99:
	ret
ENDP MoveLeft

%NEWPAGE
PROC MoveRight
	jcxz	@@99
	push	cx
	push	di
	push	si
	
	add	si,bx
	add	di,dx
	dec	si
	add	di,cx
	dec	di
	std
	rep	movsb
	
	pop 	si
	pop	di
	pop	cx
@@99:
	ret
ENDP	MoveRight

%NEWPAGE
PROC	StrNull
	mov	[byte ptr di], ASCNull
	ret
ENDP StrNull

%NEWPAGE
PROC StrLength
	push	ax
	push	di
	
	xor 	al,al
	mov	cx,0ffffh
	cld
	repnz	scasb
	not	cx
	dec	cx
	
	pop	di
	pop	ax
	ret
ENDP StrLength

%NEWPAGE
PROC StrUpper
	push	ax
	push	cx
	push	di
	push	si
	call 	StrLength
	jcxz	@@99
	cld
	mov	si,di
@@10:
	lodsb
	cmp	al,'a'
	jb	@@20
	cmp	al,'z'
	ja	@@20
	sub	al,'a'-'A'
@@20:
	stosb
	loop	@@10
@@99:
	pop	si
	pop	di
	pop	cx
	pop	ax
	ret
ENDP StrUpper

%NEWPAGE
PROC StrCompare
	push	ax
	push	di
	push	si
	cld
@@10:
	lodsb
	scasb
	jne	@@20
	or	al,al
	jne	@@10
@@20:
	pop	si
	pop	di
	pop	ax
	ret
ENDP StrCompare

%NEWPAGE
PROC StrDelete
	push	bx
	push	cx
	push	di
	push	si
	
	mov	bx,dx
	add	bx,cx
	call	StrLength
	cmp	cx,bx
	ja	@@10
	add	di,dx
	mov	[byte ptr di],ASCNull
	jmp	short @@99
@@10:
	mov	si,di
	sub	cx,bx
	inc	cx
	call	MoveLeft
@@99:
	pop	si
	pop	di
	pop	cx
	pop	bx
	ret
ENDP StrDelete

%NEWPAGE
PROC StrInsert
	push	ax
	push	bx
	push	cx
	
	xchg	si,di
	call	StrLength
	xchg	si,di
	mov	ax,cx
	
	call	StrLength
	sub	cx,dx
	inc	cx
	
	push	dx
	push	si
	mov	si,di
	mov	bx,dx
	add	dx,ax
	call	MoveRight
	pop	si
	pop	dx
	
	xor	bx,bx
	mov	cx,ax
	call	MoveLeft
	
	pop	cx
	pop	bx
	pop	ax
	ret
ENDP StrInsert

%NEWPAGE
PROC StrConcat
	push	bx
	push	cx
	push	dx
	
	call	StrLength
	mov	dx,cx
	xchg	si,di
	call	StrLength
	inc	cx
	xchg	si,di
	xor	bx,bx
	call	MoveLeft
	
	pop	dx
	pop	cx
	pop	bx
	ret
ENDP StrConcat

%NEWPAGE
PROC StrCopy
	push	bx
	push	cx
	push	dx
	
	xchg	si,di
	call	StrLength
	inc	cx
	xchg	si,di
	xor	bx,bx
	xor	dx,dx
	call	MoveLeft
	
	pop	dx
	pop	cx
	pop	bx
	ret
ENDP StrCopy

%NEWPAGE
PROC StrPos
	push	ax
	push	bx
	push	cx
	push	di
	
	call	StrLength
	mov	ax,cx
	xchg	si,di
	call	StrLength
	mov	bx,cx
	xchg	si,di
	sub	ax,bx
	jb	@@20
	mov	dx,0ffffh
@@10:
	inc	dx
	mov	cl,[byte bx + di]
	mov	[byte bx + di],ASCNull
	call	StrCompare
	mov	[byte bx + di],cl
	je	@@20
	inc	di
	cmp	dx,ax
	jne	@@10
	
	xor	cx,cx
	inc	cx
@@20:
	pop	di
	pop	cx
	pop	bx
	pop	ax
	ret
ENDP StrPos

%NEWPAGE
PROC StrRemove
	push	cx
	push	dx
	
	call	StrPos
	jne	@@99
	pushf
	xchg	si,di
	call	StrDelete
	popf
@@99:
	pop	dx
	pop	cx
	ret
ENDP StrRemove
END