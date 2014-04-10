; A little macro to help debug
; windows shellcode

[BITS 64]

%macro check 0

    test rax, rax
    jnz %%skip
    int 0x03

  %%skip:

%endmacro
