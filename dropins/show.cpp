// This file was created by the dropin manager
// You still need to add the meat to your function

#include "../core/external.h"

BOOL CALLBACK cb2(HWND hWnd, LPARAM lParam)
{
    DWORD pid;
    
    GetWindowThreadProcessId(hWnd, &pid);
    if(pid == *(DWORD *)lParam)
        ShowWindow(hWnd, SW_RESTORE);

    return TRUE;
}



extern "C" __declspec(dllexport) void show(const char *arg)
{
    DWORD pid = GetCurrentProcessId();
    EnumWindows(&cb2, (LPARAM)&pid);
}


