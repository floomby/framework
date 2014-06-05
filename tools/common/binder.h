#ifndef BINDER_H_INCLUDED
#define BINDER_H_INCLUDED

#include <cstring>

#define bind(str, func, arg)    \
    if(!strcmp(str, #func)){    \
        func(arg);              \
    }

#endif//BINDER_H_INCLUDED
