#include <cstdio>
#include <cassert>
#include <cstdlib>

#include "../core/reflect/reflect.h"
#include "../core/apiget/apiget.h"

#include <windows.h>

#include <iostream>

using std::cerr;
using std::endl;

void *load(const char *name, size_t *read)
{
    FILE *fp = fopen(name, "rb");
    assert(fp);
    
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);
    
    void *ret = malloc(size);
    assert(ret);
    
    ZeroMemory(ret, size);
    *read = size;
    
    fread(ret, 1, size, fp);
    
    fclose(fp);
    
    return ret;
}

int main(int argc, char *argv[])
{

    PROCESS_INFORMATION proc_info;
    STARTUPINFO start_info;

    ZeroMemory(&proc_info, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&start_info, sizeof(STARTUPINFO));
    start_info.cb = sizeof(STARTUPINFO);

    if(argc != 2){

    char cmd[] = "C:\\cygwin64\\bin\\nc.exe -l 1234";

    if(!CreateProcess(NULL,
                      cmd,
                      NULL,
                      NULL,
                      FALSE,
                      0,
                      NULL,
                      NULL,
                      &start_info,
                      &proc_info)
        ){
        cerr << "unable to fork exec netcat: " << GetLastError() << endl;
        exit(EXIT_FAILURE);
    }
    }
    char buf[4096];
    GetCurrentDirectory(sizeof(buf) - strlen("\\test.dll") - sizeof(TCHAR), buf);
    strcat(buf, "\\test.dll");

    size_t size;
    void *dll = load(buf, &size);

    move_dll = 1;
    void *base = ReflectiveLoad(dll);

    free(dll);

    if(argc != 2){
    
    for(;;)
        Sleep(1000);

    CloseHandle(proc_info.hProcess);
    CloseHandle(proc_info.hThread);
    
    }

    return 0;
}
