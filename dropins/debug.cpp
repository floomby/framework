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

extern "C" __declspec(dllexport) void sockinfo(const char *arg)
{
    int iOptVal = 0;
    int iOptLen = sizeof (int);

    int iResult = getsockopt(DllMeta.net.sock, SOL_SOCKET, SO_KEEPALIVE, (char *) &iOptVal, &iOptLen);
    if(iResult == SOCKET_ERROR){
        sc_printf("getsockopt for SO_KEEPALIVE failed with error: %u\n", WSAGetLastError());
    }else{
        sc_printf("SO_KEEPALIVE Value: %ld\n", iOptVal);
    }
    
}
