#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <windows.h>

#define PATH "C:\\Windows\\System32\\"

int main(int argc, char *argv[])
{
	if(argc != 3){
		printf("usage: %s <module name> <export name>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

    char buf[strlen(argv[1]) + strlen(PATH) + 1];
    ZeroMemory(buf, strlen(argv[1]) + strlen(PATH) + 1);
    strcat(buf, PATH);
    strcat(buf, argv[1]);

    if(!LoadLibraryA(buf)){
        fprintf(stderr, "last error: %d\n", GetLastError());
        exit(EXIT_FAILURE);
    }
	printf("%llx\n", GetProcAddress(GetModuleHandle(argv[1]), argv[2]));

	return 0;
}
