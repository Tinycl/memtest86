.code


;
; unsigned long long AsmTestAccessTime(PBYTE Address)
;       Returns the number of CPU clocks it took to access Address.
;       Parameters:
;           Address - memory region to be accessed
;       Returns:
;           The number of CPU clocks the access took.
;
AsmTestAccessTime proc
    rdtsc
    shl     rdx, 32
    or      rdx, rax
    mov     r8, rdx

    lfence
    mov     al, [rcx]
    lfence

    rdtsc
    shl     rdx, 32
    or      rax, rdx
    sub     rax, r8

    ret
AsmTestAccessTime endp

mfence;

AsmMovsDW proc
    cld

    mov edi, ecx
    mov esi, edx
    mov rcx, r8
    rep movsd

    ret

AsmMovsDW endp

mfence;

AsmBlockMove2Move proc

	;
	; At the end of all this 
	; - the second half equals the inital value of the first half
	; - the first half is right shifted 32-bytes (with wrapping)
	;
	; ecx pmid, edx pbuf, r8 len

	cld

	; Move first half to second half
	mov edi, ecx ; Destionation, pp (mid point)
	mov esi, edx ; Source, p (start point)
	mov rcx, r8 ; Length, len (size of a half in DWORDS)
	rep movsd


	; Move the second half, less the last 32-bytes. To the first half, offset plus 32-bytes
	mov edi, edx
	add edi, 32  ;Destination, p(start-point) plus 32 bytes
	mov esi, ecx ;Source, pp(mid-point)
	mov rcx, r8
	sub rcx, 8 ;Length, len(size of a half in DWORDS) minus 8 DWORDS (32 bytes)
	rep movsd

	; Move last 8 DWORDS (32-bytes) of the second half to the start of the first half
	mov edi, edx ;Destination, p(start-point)
	mov rcx, 8	;Source, 8 DWORDS from the end of the second half, left over by the last rep/movsl, 8 DWORDS (32-bytes)
	rep movsd

AsmBlockMove2Move endp




;(PDWORD pmid, PDWORD pbuf, unsigned int size_dw); //rcx, rdx, r8


end