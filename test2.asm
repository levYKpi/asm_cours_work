;=======================================================================
DATA SEGMENT Л
	var1000		dd	10h
	Var.2		dD	(-100+15)*3
	V			dw	0ffffh-10
	V2			Dw	-10b-3*2
	varABC1		DB	100b*10b
	VARABC25	db	-10b/10b
    C			dw  25
    D           db  10101b
	D			dd	2
	;data2 segment
DATA ENDS
Assume

DATA2 SEGMENT
	var1000		dd
	D4			dd	2
	;data2 segment
DATA2 ENDS

DAT SEGMENT
Dat endc
Dat ends
;=======================================================================
;=======================================================================
;Code2; SEGMENT
;Code2 ENDS

CODE SEGMENT
;***********************************
	CODEvar1	DD	10h+15h
	CODEvar2	dd	10-100h
	CVaR1		DW	10+15h
	CVaR2		dW	10h/2
	CV1			Db	10h
	A			dB	-1h
;***********************************
begin:
	sti
	
	jnz	@1
	jnz @3
	
@1:	dec dec	al
	dec	ah
	dec	eax
	dec	bl,
	dec	bh
	dec	ebx
	dec cl
	dec ch
	dec	ecx
	dec	dl
	dec dh
	dec edx
	
	inc var1[eax]
	inc var1[eax*4]
	inc	var2[edi*8]
	inc	var2[ecx]
	inc	es:var1[eax*2]
	inc	gs:A[ebx]
	inc gs:A[ecx*4]
	inc	ss:D[edx]
	inc ss:D[edi*2]
	inc	codeVar2
	
	add	al,al,
	add	ah,ah
	add	al,ah
	add	ah,al
	add	eax,eax	.eax
	add	eax,ebx
	add	ebx,eax
	add	dh,cl
	add	edx,esi
	add	eax,ebp
	add	eax,edi
	add	ebx,esp
	
	or eax, eax
	or	eax,codeVar2[eax]
	or	ebx,var1[[eax*2]]
	or	ecx,var2[edi*4]
	or	al,A[ecx]
	or	bl,es:D[eax*2]
	or	bh,gs:A[ebx]
	or	ecx,gs:var2[ecx*4]
    or	eax,gs:CODEVAR2[ecx]
	or	cl,ss:D[edx]
	or	ch,ss:D[edi*2]
	or	dl,Cv1[edi*8]
	
	and	codeVar2[eax],eax
	and	var1[eax*8],ebx
	and	var2[edi*4],ecx
	and	A[ecx],al
	add	es:var1[eax*2],eax
	add	gs:A[ebx],ah
	add gs:A[ecx*4],al
	add	ss:CODEVAR1[edx],ebx
	add ss:D[edi*2],ch
	and	Cv1[edi*2],cl
	
	
	cmp	eax,10h
	cmp	ebx,5-17
	cmp	ecx,-2
	cmp	edx,10b
	cmp	al,0fh
	cmp	ah,0ah+10b-5*2
	cmp	bl,6
	cmp dh,17/2-1
	cmp	cl,1010b
	
	
	jnz	@1
	jnz @2
	jnz	$
@2:
	
	mov	al,10
	mov ah,10h
	mov bl,10b
	mov bh,-10
	mov dl,-10h
	mov dh,-10b
	mov cl,5
	mov cl,-5
	mov ch,5
	mov	ch,-5
	mov eax,100h
	mov eax,-25
	mov eax,10b
	mov ebx,-100h
	mov ecx,25
	mov edx,-10b
	mov eax,(4+3)/100b-10h
	mov ebx,(4/2)*100-1
	mov ecx,(4*3)-100b*2h
	mov edx,4h-10h+10h/2h*2h

	jnz	@1
	jnz	@3
@3:
CODE ENDS
    END begin
;=======================================================================
