#include "../core/external.h"

extern "C" __declspec(dllexport) void currentpid(const char *arg)
{
    sc_printf("current pid is %d\n", (uint32_t)(uint64_t)GetCurrentProcessId());
}

extern "C" __declspec(dllexport) void echo(const char *arg)
{
    SockSend(arg);
    SockSend("\n");
}



