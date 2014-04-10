#ifndef REFLECT_H_INCLUDED
#define REFLECT_H_INCLUDED

#include "str.h"
#include "apiget.h"

extern "C" void *ReflectiveLoad();
extern "C" void ReflectiveLoad_end();

#define ReflectiveLoad_size    ((uint32_t)((uint64_t)&ReflectiveLoad_end - (uint64_t)&ReflectiveLoad))

#endif//REFLECT_H_INCLUDED
