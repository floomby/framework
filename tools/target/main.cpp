#include <cstdio>

#include <windows.h>

int main(int argc, char *argv[])
{
    printf("%d\n", GetCurrentProcessId());
    fflush(stdout);

    for(;;)
	Sleep(100);

    return 0;
}
