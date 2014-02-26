#include <cstdint>
#include <windows.h>

#include "../inject/inject.h"

__attribute__ ((__optimize__("omit-frame-pointer"))) void shim()
{
    __asm__ __volatile__(
        "pushq %%rax;"
        "movq $0xDEADBEEFDEADBEEF, %%rax;"
        "xchgq %%rax, (%%esp);"
        "pushf;"
        "pushq %%rcx;"
        "pushq %%rdx;"
        "pushq %%r8;"
        "pushq %%r9;"
        "pushq %%r10;"
        "pushq %%r11;"
        "movq $0xDEADBEEFDEADBEEF, %%rcx;"
        "movq $0xDEADBEEFDEADBEEF, %%rax;"
        "call *%%rax;"
        "popq %%r11;"
        "popq %%r10;"
        "popq %%r9;"
        "popq %%r8;"
        "popq %%rdx;"
        "popq %%rcx;"
        "popf;"
        "popq %%rax;"
        :
        :
        :
    );
}

extern "C" void patch(uint64_t ret)
{   
    *(uint64_t *)((uint8_t *)&shim + 3) = ret;
    *(uint64_t *)((uint8_t *)&shim + 29) = 123;
    *(uint64_t *)((uint8_t *)&shim + 39) = 0x7fb5d404454;
}

extern "C" BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    _InjectPid((void *)&shim, 62, 0, 6516, &patch);

    // patch((uint64_t)&&done);
    // shim();
    // done:

    return TRUE;
}
