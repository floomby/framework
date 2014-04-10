#include "../core/external.h"

extern "C" __declspec(dllexport) void launch(const char *arg)
{

    ShellExecute(NULL,
                 "runas",
                 arg,
                 NULL,
                 NULL,
                 SW_SHOWNORMAL
                 );

    return;
}

extern "C" __declspec(dllexport) void replace(const char *arg)
{
    ShellExecute(NULL,
                 "runas",
                 arg,
                 NULL,
                 NULL,
                 SW_HIDE
                 );
                 
    SockDisconnect();
    return;
}
