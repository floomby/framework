// This file was created by the dropin manager
// You still need to add the meat to your function

#include <windows.h>

#include "../common/crt.h"
#include "../core/external.h"

extern "C" __declspec(dllexport) void migrate(const char *arg)
{
    DllMeta.next.pid = atoi(arg);
    // create the thing for info proccess
    // DllMeta.next.info = 
    
//    Migrate();
}


