#include <cstdint>
#include <windows.h>

#include "../inject/inject.h"
#include "../reflecter/reflect.h"
#include "meta.h"

extern "C" BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return TRUE;
}

extern "C" __declspec(dllexport) void Dummy()
{
    char *kernel;
    __asm__ __volatile__(
        "lea 2(%%rip), %0;"
        "jmp .+15;"
        ".asciz \"kernel32.dll\";"
        : "=r" (kernel)
        :
        :
    );
    
    void (*exitProcess)(uint32_t) = (void (*)(uint32_t))_GetFuncAddr(kernel, 372);
    exitProcess(123);
}

extern "C" __declspec(dllexport) void Migrate(uint32_t pid)
{
    _InjectPid(DllMeta.where, DllMeta.size, offset_to_rva(DllMeta.where, &Dummy), pid);
}
