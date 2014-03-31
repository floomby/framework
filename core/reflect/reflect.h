#ifndef REFLECT_H_INCLUDED
#define REFLECT_H_INCLUDED

#include "../stuff/str.h"
#include "../apiget/apiget.h"

extern int move_dll;

extern "C" void *_ReflectiveLoad(void *dll);
extern "C" void _ReflectiveLoad_end();

#define _ReflectiveLoad_size    ((uint32_t)((uint64_t)&_ReflectiveLoad_end - (uint64_t)&_ReflectiveLoad))

#endif//REFLECT_H_INCLUDED
