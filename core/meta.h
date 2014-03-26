#ifndef META_H_INCLUDED
#define META_H_INCLUDED

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstdint>

struct DllMeta_t {
    void *where;
    size_t size;
    struct {
        uint32_t pid;
        WSAPROTOCOL_INFO *info;
    } next;
    struct {
        SOCKET sock;
        void *buf;
    } net;
    struct {
        bool move;
    } inject;
};

extern DllMeta_t DllMeta;

#endif//META_H_INCLUDED
