#include "inject.h"
#include "../pe/pe_structs.h"

#include <TlHelp32.h>

DWORD get_tid(DWORD pid)
{
    void * hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if(hThreadSnap == INVALID_HANDLE_VALUE){
        ExitProcess(4);
    }
    
    THREADENTRY32 tEntry;
    tEntry.dwSize = sizeof(THREADENTRY32);
    DWORD ret = 0;
    
    for(BOOL success = Thread32First(hThreadSnap, &tEntry);
        !ret && success && GetLastError() != ERROR_NO_MORE_FILES;
        success = Thread32Next(hThreadSnap, &tEntry)){
            if(tEntry.th32OwnerProcessID == pid){
                ret = tEntry.th32ThreadID;
            }
    }
    
    CloseHandle(hThreadSnap);
    
    return ret;
}

#define THREAD_PERMS    (THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME)
#define PROC_PERMS      (PROCESS_VM_WRITE | PROCESS_VM_OPERATION)

void _InjectPid(void *what, size_t size, uint32_t where, uint32_t pid, void (*patch)(uint64_t))
{
    HANDLE hProc = OpenProcess(PROC_PERMS, false, pid);
    
    void *mem = VirtualAllocEx(hProc, NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    
    CONTEXT ctx;
    
    HANDLE hThread = OpenThread(THREAD_PERMS, false, get_tid(pid));
    SuspendThread(hThread);
    
    ctx.ContextFlags = CONTEXT_CONTROL;
    GetThreadContext(hThread, &ctx);
    patch(ctx.Rip);
    WriteProcessMemory(hProc, mem, what, size, NULL);
    ctx.Rip = (uint64_t)rva_to_offset(mem, where);
    ctx.ContextFlags = CONTEXT_CONTROL;
    SetThreadContext(hThread, &ctx);
    
    ResumeThread(hThread);
    
    // This might cause the current application (the one we are injecting from) to go unresponsive
    Sleep(3000);
    
    //VirtualFreeEx(hProc, mem, size, MEM_DECOMMIT);
    
    CloseHandle(hProc);
    CloseHandle(hThread);
}