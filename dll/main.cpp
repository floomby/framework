#include <cstdint>
#include <windows.h>

extern "C" BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    Sleep(3000);


    return TRUE;
}
