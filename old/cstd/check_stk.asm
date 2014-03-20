section .text

___chkstk_ms:
	pushq	rcx
	pushq	rax
	cmp     rax,0x1000
	lea     rcx,[rsp+0x18]
	jb     .end
.lp:
	sub    rcx,0x1000
	or     QWORD PTR [rcx],0x0
	sub    rax,0x1000
	cmp    rax,0x1000
	ja     .lp
.end:
	sub    rcx,rax
	or     QWORD PTR [rcx],0x0
	pop    rax
	pop    rcx
	ret
