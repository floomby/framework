#include "inject.h"

#define PROC_PERMS  (PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ)

void _InjectPid(void *what, size_t size, uint32_t where, uint32_t pid)
{
    HANDLE hProc = OpenProcess(PROC_PERMS, FALSE, pid);
    
    void *mem = VirtualAllocEx(hProc, NULL, size, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if(!mem) ExitProcess(GetLastError());
    BOOL suc = WriteProcessMemory(hProc, mem, what, size, NULL);
    if(!suc) ExitProcess(GetLastError());
    
    CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)rva_to_offset(mem, where), mem, 0, NULL);
    
    CloseHandle(hProc);
}
