; This creates a thread that returns and
; then continues execution to the next block

[BITS 64]
[ORG 0]

default rel


    cld    
    and rsp, 0xFFFFFFFFFFFFFFF0
    call start
%include "./block_api.asm"
start:
    pop rbp
    
    
    
    
    
    
