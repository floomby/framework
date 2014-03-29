#include "chomp.h"

char *chomp(char *str)
{
    char *it = str;
    
    while(*it)
        it++;

    *it = '\0';
    
    if(*(it - 1) == '\r')
        *(it - 1) = '\0';
    
    return str;
}
