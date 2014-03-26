#ifndef INTERNAL_H_INCLUDED
#define INTERNAL_H_INCLUDED

#ifdef EXTERNAL_H_INCLUDED
#error "don't include both the internal and external headers"
#endif//EXTERNAL_H_INCLUDED

#define BUF_SIZE 4096

#include "meta.h"

extern "C" BOOL DllMain(void *where, size_t size);
extern "C" BOOL DllMain2(void *where, size_t size);

#endif//INTERNEL_H_INCLUDED
