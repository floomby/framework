#include "../core/external.h"

extern "C" __declspec(dllexport) void currentpid(const char *arg)
{
    sc_printf("current pid is %d\n", (uint32_t)(uint64_t)GetCurrentProcessId());
}

extern "C" __declspec(dllexport) void echo(const char *arg)
{
    sc_printf("%s\n", arg);
}

extern "C" __declspec(dllexport) void debuginfo(const char *arg)
{
    sc_printf("location: %x\nsize: %x\n", DllMeta.curr.where, DllMeta.curr.size);
}
