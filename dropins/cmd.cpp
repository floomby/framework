// This file was created by the dropin manager
// You still need to add the meat to your function

#include <windows.h>

extern "C" __declspec(dllexport) void cmd(const char *arg)
{
    system(arg);
}


