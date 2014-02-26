#include "inject.h"
#include "../pe/pe_structs.h"

#define PROC_PERMS  (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ)

void _InjectPid(void *what, size_t size, uint32_t where, uint32_t pid)
{
    HANDLE hProc = OpenProcess(PROC_PERMS, false, pid);
    
    void *mem = VirtualAllocEx(hProc, NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    WriteProcessMemory(hProc, mem, what, size, NULL);

    CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)rva_to_offset(mem, where), mem, 0, NULL);
    
    CloseHandle(hProc);
}