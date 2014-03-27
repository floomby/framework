// This file was created by the dropin manager
// You still need to add the meat to your function

#include "../core/external.h"

extern "C" __declspec(dllexport) void currentpid(const char *arg)
{
    sc_printf("current pid is %d\n", (uint32_t)(uint64_t)GetCurrentProcessId());
}


