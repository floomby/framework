#include "reflect.h"

//TODO finish reworking this function

void _ReflectiveLoad(void *dll)
{
    char *kernel;
    __asm__ __volatile__(
        "lea test, %0;"
        "jmp done;"
        "test: .asciz \"kernel32.dll\";"
        "done:;"
        : "=r" (kernel)
        :
        :
    );

//    void *addr = _GetFuncAddr(kernel, 1390);
//    ((void (*)(uint32_t))addr)(3000);



}
