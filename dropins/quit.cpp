// This file was created by the dropin manager
// You still need to add the meat to your function

#include "../core/external.h"

// I need to think of a way to clean up the memory that is used

extern "C" __declspec(dllexport) void quit(const char *arg)
{
    SockDisconnect();
    ExitThread(0);
}


