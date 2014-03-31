#ifndef REFLECT_H_INCLUDED
#define REFLECT_H_INCLUDED

#include "../stuff/str.h"
#include "../apiget/apiget.h"

extern int move_dll;

extern "C" void *ReflectiveLoad(void *dll);
extern "C" void ReflectiveLoad_end();

#define ReflectiveLoad_size    ((uint32_t)((uint64_t)&ReflectiveLoad_end - (uint64_t)&ReflectiveLoad))

#endif//REFLECT_H_INCLUDED
