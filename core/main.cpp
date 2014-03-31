#include "internal.h"

#include "../common/pe_structs.h"
#include "apiget/export.h"

struct DllMeta_t DllMeta;

extern "C" __declspec(dllexport) BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    struct dos_header *dh = (struct dos_header *)hinstDLL;
    struct nt_header *nh = (struct nt_header *)rva_to_offset(hinstDLL, dh->e_lfanew);

    // set the metadata
    DllMeta.prev.where = DllMeta.curr.where;
    DllMeta.prev.size = DllMeta.curr.size;
    DllMeta.prev.pid = DllMeta.curr.pid;
    DllMeta.prev.kill = DllMeta.curr.kill;

    DllMeta.curr.where = hinstDLL;
    DllMeta.curr.size  = nh->OptionalHeader.SizeOfImage;
    DllMeta.curr.pid = GetCurrentProcessId();
    DllMeta.curr.kill = DllMeta.next.kill;
    
    ZeroMemory(&DllMeta.next, sizeof(DllInfo_t));

    if(DllMeta.prev.pid){
        ((void (*)())GetExport(DllMeta.curr.where, "restart"))();
    }else{
        ((void (*)())GetExport(DllMeta.curr.where, "startup"))();
        return TRUE;
    }

    // free memory from the last process
    if(DllMeta.prev.pid && !DllMeta.prev.kill){
        HANDLE hProc = OpenProcess(PROC_PERMS, FALSE, DllMeta.prev.pid);
    
        VirtualFreeEx(hProc, DllMeta.prev.where, DllMeta.prev.size, MEM_RELEASE);
    
        CloseHandle(hProc);
    }
    
    return TRUE;
}
