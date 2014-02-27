#include <cstdint>
#include <windows.h>

#include "../inject/inject.h"
#include "../reflecter/reflect.h"
// #include "meta.h"

static struct {
    void *where;
    size_t size;
} DllMeta;

extern "C" BOOL DllMain(void *where, size_t size)
{
    DllMeta.where = where;
    DllMeta.size  = size;
    if(!move_dll) TerminateThread((HANDLE)(uint64_t)GetCurrentThreadId(), 123);
    return TRUE;
}

extern "C" __declspec(dllexport) void Migrate(uint32_t pid)
{
    _InjectPid(DllMeta.where, DllMeta.size, offset_to_rva(DllMeta.where, &_ReflectiveLoad), pid);
}