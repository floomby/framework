#ifndef META_H_INCLUDED
#define META_H_INCLUDED

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstdint>

typedef struct {
    void *where;
    size_t size;
    uint32_t pid;
} DllInfo_t;

struct DllMeta_t {
    DllInfo_t prev;
    DllInfo_t curr;
    DllInfo_t next;
    struct {
        SOCKET sock;
        WSAPROTOCOL_INFO info;
        char *buf;
    } net;
};

extern DllMeta_t DllMeta;

#endif//META_H_INCLUDED
