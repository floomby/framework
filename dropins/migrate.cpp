// This file was created by the dropin manager
// You still need to add the meat to your function

#include "../common/crt.h"
#include "../core/external.h"

extern "C" __declspec(dllexport) void migrate(const char *arg)
{
    Next(atoi(arg));
    Migrate();
    SockDisconnect();
    ExitThread(0);
}


