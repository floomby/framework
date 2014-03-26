// This file was created by the dropin manager
// You still need to add the meat to your function

#include "../core/external.h"

extern "C" __declspec(dllexport) void restart()
{
    MessageBeep(MB_ICONEXCLAMATION);

//    ReconnectShell();
    ExitThread(0);
}


