// This file was created by the dropin manager
// You still need to add the meat to your function

#include "../core/external.h"

static HWND wnds[12] = { (HWND)NULL };

BOOL CALLBACK cb_show(HWND hWnd, LPARAM lParam)
{
    DWORD pid;
    
    GetWindowThreadProcessId(hWnd, &pid);
    if(pid == *(DWORD *)lParam)
        ShowWindow(hWnd, SW_RESTORE);

    return TRUE;
}

BOOL CALLBACK cb(HWND hWnd, LPARAM lParam)
{
    DWORD pid;
    
    GetWindowThreadProcessId(hWnd, &pid);
    if(pid == *(DWORD *)lParam)
        ShowWindow(hWnd, SW_HIDE);

    return TRUE;
}

extern "C" __declspec(dllexport) void show(const char *arg)
{
    DWORD pid = GetCurrentProcessId();
    EnumWindows(&cb_show, (LPARAM)&pid);
}

extern "C" __declspec(dllexport) void hide(const char *arg)
{
    DWORD pid = GetCurrentProcessId();
    EnumWindows(&cb, (LPARAM)&pid);
}
