.global ___chkstk_ms

___chkstk_ms:
    pushq %rcx
    pushq %rax
    cmpq $0x1000, %rax
    lea 0x18(%rsp), %rcx
    jb .end
    .lp:
    subq $0x1000, %rcx
    orq $0x0, (%rcx)
    subq $0x1000, %rax
    cmpq $0x1000, %rax
    ja .lp
    .end:
    subq %rax, %rcx
    orq $0x0, (%rcx)
    popq %rax
    popq %rcx
    ret
