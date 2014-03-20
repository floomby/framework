#ifndef SHELL_H_INCLUDED
#define SHELL_H_INCLUDED

#include <cstdint>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>


void _Shell(const char *server, const char *port);

#endif//SHELL_H_INCLUDED