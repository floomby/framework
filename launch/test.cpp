#include <cstdio>
#include <cassert>
#include <cstdlib>

#include "../core/reflect/reflect.h"
#include "../core/apiget/apiget.h"

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
    // if(argc != 2){
        // fprintf(stderr, "usage: %s <pid>\n", argv[0]);
        // exit(1);
    // }

    size_t size;
    void *dll = load("C:\\Users\\Josh\\Desktop\\malware\\framework\\test.dll", &size);

    move_dll = 1;
    void *base = _ReflectiveLoad(dll);

    free(dll);

    return 0;
}
