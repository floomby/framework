#ifndef SHELL_H_INCLUDED
#define SHELL_H_INCLUDED

int sc_printf(const char *fmt, ...);

int SockConnect(const char *server, const char *port);
void SockDisconnect();
void SockSend(const char *msg);
char *SockRecv();
void SockShell();

#endif//SHELL_H_INCLUDED
