#include "internal.h"

#include "../common/pe_structs.h"
#include "apiget/export.h"

struct DllMeta_t DllMeta;

extern "C" BOOL DllMain(void *where, size_t size)
{
    // set the metadata
    DllMeta.prev = {0};
    
    DllMeta.curr.where = where;
    DllMeta.curr.size = size;
    DllMeta.curr.pid = (uint32_t)(uint64_t)GetCurrentProcess();

    DllMeta.next = {0};
    
    // change the entry point
    struct dos_header *dh = (struct dos_header *)where;
    struct nt_header *nh = (struct nt_header *)rva_to_offset(where, dh->e_lfanew);
    nh->OptionalHeader.AddressOfEntryPoint = offset_to_rva(where, &DllMain2);
    
    // call the starup dropin
    ( (void (*)())GetExport(where, "startup") )();
    
    return TRUE;
}

extern "C" BOOL DllMain2(void *where, size_t size)
{
    // set the metadata
    DllMeta.prev.where = DllMeta.curr.where;
    DllMeta.prev.size = DllMeta.curr.size;
    DllMeta.prev.pid = DllMeta.curr.pid;

    DllMeta.curr.where = where;
    DllMeta.curr.size  = size;
    DllMeta.curr.pid = DllMeta.next.pid;
    
    DllMeta.next = {0};
    

    // free memory from the last process
    if(DllMeta.prev.pid){
        HANDLE hProc = OpenProcess(PROC_PERMS, FALSE, DllMeta.prev.pid);
    
        VirtualFreeEx(hProc, DllMeta.prev.where, DllMeta.prev.size, MEM_RELEASE);
    
        CloseHandle(hProc);
    }
    
    // call the restart dropin
    ( (void (*)())GetExport(where, "restart") )();
    
    return TRUE;
}
