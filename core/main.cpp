#include "internal.h"

#include "../common/pe_structs.h"
#include "apiget/export.h"

struct DllMeta_t DllMeta;

extern "C" BOOL DllMain(void *where, size_t size)
{
    // set the metadata
    DllMeta.where = where;
    DllMeta.size  = size;
    DllMeta.pid = 0;
    
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
    // free memory from the last process
    if(DllMeta.pid){
        HANDLE hProc = OpenProcess(PROC_PERMS, FALSE, DllMeta.pid);
    
        VirtualFreeEx(hProc, DllMeta.where, DllMeta.size, MEM_RELEASE);
    
        CloseHandle(hProc);
    }

    // set the metadata
    DllMeta.where = where;
    DllMeta.size  = size;
    DllMeta.pid = (uint32_t)(uint64_t)GetCurrentProcess();
    
    // call the restart dropin
    ( (void (*)())GetExport(where, "restart") )();
    
    return TRUE;
}
