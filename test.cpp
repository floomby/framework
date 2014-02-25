#include <cstdio>
#include <windows.h>
#include <cassert>
#include <cstdlib>

#include "reflecter/reflect.h"
#include "apiget/apiget.h"

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
    size_t size;
    void *dll = load(argv[1], &size);

    _ReflectiveLoad(dll);
    
    free(dll);

    return 0;
}
