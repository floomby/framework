
#include "../internal.h"
#include "shell.h"
#include "../apiget/export.h"

#include "../../common/crt.h"

static char sc_buf[BUF_SIZE];

int sc_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int ret = vsnprintf(sc_buf, sizeof(sc_buf), fmt, args);
    va_end(args);
    SockSend(sc_buf);
    return ret;
}



int SockConnect(const char *server, const char *port)
{
    int ret;
    WSADATA wsaData;
    
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    
    if((ret = WSAStartup(MAKEWORD(2,2), &wsaData))){
        return ret;
    }
    
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    if((ret = getaddrinfo(server, port, &hints, &result))){
        WSACleanup();
        return ret;
    }

    for(ptr = result; ptr; ptr = ptr->ai_next){
        DllMeta.net.sock = socket(ptr->ai_family,
                                  ptr->ai_socktype,
                                  ptr->ai_protocol);

        if(DllMeta.net.sock == INVALID_SOCKET){
            WSACleanup();
            return INVALID_SOCKET;
        }

        if(connect(DllMeta.net.sock, ptr->ai_addr, (int)ptr->ai_addrlen)){
            closesocket(DllMeta.net.sock);
            DllMeta.net.sock = INVALID_SOCKET;
            continue;
        }
    }

    freeaddrinfo(result);
    
    sc_printf("hello\n");
    
    return 0;
}

void SockDisconnect()
{
    closesocket(DllMeta.net.sock);
    DllMeta.net.sock = INVALID_SOCKET;

    WSACleanup();
}

void SockSend(const char *msg)
{
    send(DllMeta.net.sock, msg, strlen(msg), 0);
}

char *SockRecv()
{
    ZeroMemory(sc_buf, sizeof(sc_buf));
    int bytes;
    
    bytes = recv(DllMeta.net.sock, sc_buf, sizeof(sc_buf) - 1, 0);
    
    if(bytes > 0){
        ;
    }else if(bytes == 0){
        SockDisconnect();
    }else{
        // recv error
        SockDisconnect();
    }
    
    return sc_buf;
}

void SockParse()
{
    char *it = sc_buf;
    while(*it && (*it != ' ')){
        it++;
    }
    
    *it = '\0';
    
    if(it == sc_buf)
    {
        return;
    }
    
    void *fp;
    
    if( fp = GetExport(DllMeta.where, sc_buf) ){
        ( (void (*)(const char *))fp )(sc_buf + strlen(sc_buf) + 1);
    }else{
        SockSend("undefined directive: ");
        SockSend(sc_buf);
        SockSend("\n");
    }
}


void SockShell()
{
    for(;;){
        if(DllMeta.net.sock == INVALID_SOCKET)
            break;
        SockRecv();
        sc_buf[strlen(sc_buf) - 1] = '\0';
        SockParse();
    }
}
