#ifndef REFLECT_H_INCLUDED
#define REFLECT_H_INCLUDED

#include "../stuff/str.h"
#include "../apiget/apiget.h"

extern int move_dll;

extern "C"
__declspec(dllexport)
void *_ReflectiveLoad(void *dll);

#endif//REFLECT_H_INCLUDED
