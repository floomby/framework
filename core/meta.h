#ifndef META_H_INCLUDED
#define META_H_INCLUDED

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstdint>

#define PROC_PERMS (PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ)

typedef struct {
    void *where;
    size_t size;
    uint32_t pid;
    bool kill;
} DllInfo_t;

struct DllMeta_t {
    DllInfo_t prev;
    DllInfo_t curr;
    DllInfo_t next;
    struct {
        SOCKET sock;
        WSAPROTOCOL_INFO info;
        char *buf;
        size_t buf_sz;
    } net;
};

extern DllMeta_t DllMeta;

#endif//META_H_INCLUDED
