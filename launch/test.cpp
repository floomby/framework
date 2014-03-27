#include <cstdio>
#include <cassert>
#include <cstdlib>

#include "../core/reflect/reflect.h"
#include "../core/apiget/apiget.h"

#include <windows.h>
#include <unistd.h>

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
    STARTUPINFO start_info = {sizeof(STARTUPINFO)};

    char cmd[] = "C:\\cygwin64\\bin\\nc.exe";
    char arg[] = "C:\\cygwin64\\bin\\nc.exe -l 1234";

    if(!CreateProcess(cmd,
                      arg,
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

    size_t size;
    void *dll = load("C:\\Users\\Josh\\Desktop\\malware\\framework\\test.dll", &size);

    move_dll = 1;
    void *base = _ReflectiveLoad(dll);

    free(dll);

    WaitForSingleObject(proc_info.hProcess, INFINITE);
    
    CloseHandle(proc_info.hProcess);
    CloseHandle(proc_info.hThread);
    
    return 0;
}
