#ifndef REFLECT_H_INCLUDED
#define REFLECT_H_INCLUDED

#include "../stuff/str.h"
#include "../apiget/apiget.h"

extern int move_dll;

extern "C"
#ifdef NEED_META
__declspec(dllexport)
#endif//NEED_META
void *_ReflectiveLoad(void *dll);

#endif//REFLECT_H_INCLUDED
