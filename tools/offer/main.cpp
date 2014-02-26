#include <cstdio>
#include <cstdlib>

#include <windows.h>

int main(int argc, char *argv[])
{
	if(argc != 3){
		printf("usage: %s <module name> <export name>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	printf("%llx\n", GetProcAddress(GetModuleHandle(argv[1]), argv[2]));

	return 0;
}
