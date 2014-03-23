#include "sopar.h"

// the winsock subsytem uses a referance counter
// this means calling wsastartup and wsacleanup
// will not effect any of the wsa code running
// in the rest of the process

sopar::sopar(const char *server, const char *port)
{
    int ret;
    WSADATA wsaData;
    
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    
    if((ret = WSAStartup(MAKEWORD(2,2), &wsaData))){
        TerminateThread((void *)(uint64_t)GetCurrentThreadId(), ret);
    }
    
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    if((ret = getaddrinfo(server, port, &hints, &result))){
        WSACleanup();
        TerminateThread((void *)(uint64_t)GetCurrentThreadId(), ret);
    }

    for(ptr = result; ptr; ptr = ptr->ai_next){
        this->sock = socket(ptr->ai_family,
                      ptr->ai_socktype,
                      ptr->ai_protocol);

        if(this->sock == INVALID_SOCKET){
            WSACleanup();
            TerminateThread((void *)(uint64_t)GetCurrentThreadId(), 92);
        }

        if(connect(this->sock, ptr->ai_addr, (int)ptr->ai_addrlen)){
            closesocket(this->sock);
            this->sock = INVALID_SOCKET;
            continue;
        }
    }

    freeaddrinfo(result);
    
    this->sopar_send("hello\n");
}

void sopar::go()
{
    for(;;){
        this->sopar_recv();
        this->buf[strlen(this->buf) - 1] = '\0';
        this->parse();
        if(this->sock == INVALID_SOCKET)
            break;
    }
}

char *sopar::sopar_recv()
{
    ZeroMemory(this->buf, sizeof(this->buf));
    int bytes;
    
    bytes = recv(this->sock, this->buf, sizeof(this->buf) - 1, 0);
    
    if(bytes > 0){
        ;
    }else if(bytes == 0){
        this->cleanup();
    }else{
        // recv error
        this->cleanup();
    }
    
    return this->buf;
}

void sopar::sopar_send(const char *str)
{
    send(this->sock, str, strlen(str), 0);
}

void sopar::cleanup()
{
    closesocket(this->sock);
    this->sock = INVALID_SOCKET;
    
    WSACleanup();
}
