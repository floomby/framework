// This file was created by the dropin manager
// You still need to add the meat to your function

#include "../core/external.h"

extern "C" __declspec(dllexport) void echo(const char *arg)
{
    SockSend(arg);
    SockSend("\n");
}

