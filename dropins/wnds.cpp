#include "../core/external.h"

#define MAX_WND 12

static HWND wnds[MAX_WND] = { (HWND)NULL };

bool add_wnd(HWND wnd)
{
    for(int i = 0; i < MAX_WND; ++i){
        if(!wnds[i]){
            wnds[i] = wnd;
            return false;
        }
    }
    
    return true;
}

bool check_wnd(HWND wnd)
{
    for(int i = 0; i < MAX_WND; ++i){
        if(wnd == wnds[i])
            return true;
    }
    
    return false;
}

bool rm_wnd(HWND wnd)
{
    for(int i = 0; i < MAX_WND; ++i){
        if(wnd == wnds[i]){
            wnds[i] = (HWND)NULL;
            return false;
        }
    }
    
    return true;
}


BOOL CALLBACK cb_show(HWND hWnd, LPARAM lParam)
{
    DWORD pid;
    
    GetWindowThreadProcessId(hWnd, &pid);
    if(pid == *(DWORD *)lParam){
        if(check_wnd(hWnd)){
            ShowWindow(hWnd, SW_RESTORE);
            rm_wnd(hWnd);
        }
    }

    return TRUE;
}

BOOL CALLBACK cb_hide(HWND hWnd, LPARAM lParam)
{
    DWORD pid;
    
    GetWindowThreadProcessId(hWnd, &pid);
    if(pid == *(DWORD *)lParam){
        if(IsWindowVisible(hWnd)){
            if(add_wnd(hWnd)){
                sc_printf("unable to hide window\n");
                return FALSE;
            }
            ShowWindow(hWnd, SW_HIDE);
        }
    }

    return TRUE;
}

extern "C" __declspec(dllexport) void show(const char *arg)
{
    DWORD pid = atoi(arg);
    EnumWindows(&cb_show, (LPARAM)&pid);
}

extern "C" __declspec(dllexport) void hide(const char *arg)
{
    DWORD pid = atoi(arg);
    EnumWindows(&cb_hide, (LPARAM)&pid);
}
