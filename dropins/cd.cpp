// This file was created by the dropin manager
// You still need to add the meat to your function

#include "../core/external.h"

extern "C" __declspec(dllexport) void cd(const char *arg)
{
    if(!SetCurrentDirectory(arg))
        sc_printf("failed to cd into %s\n", arg);
}


