#include "inject.h"

bool InjectPid(void *what, size_t size, uint32_t where, uint32_t pid)
{
    HANDLE hProc;
    void *mem;
    
    if(!(hProc = OpenProcess(PROC_PERMS, FALSE, pid))) return true;
    if(!(mem = VirtualAllocEx(hProc, NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE))) return true;
    if(!WriteProcessMemory(hProc, mem, what, size, NULL)) return true;
    if(!CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)rva_to_offset(mem, where), mem, 0, NULL)) return true;
    
    CloseHandle(hProc);
    
    return false;
}
