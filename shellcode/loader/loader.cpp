#include <cstdio>
#include <cstdint>

#include "reflect/reflect.h"

int main(int argc, char *argv[])
{
    uint8_t *it = (uint8_t *)&ReflectiveLoad;

    printf("print \"");
    for(size_t i = 0; i < ReflectiveLoad_size; ++i){
        printf("\\x%02x", it[i]);
    }

    puts("\"");
    
    return 0;

}
