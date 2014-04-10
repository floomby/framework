#ifndef REFLECT_H_INCLUDED
#define REFLECT_H_INCLUDED

#include "../../common/pe_structs.h"
#include "../../common/crt.h"
#include <windows.h>

extern "C" void *ReflectiveLoad(void *dll);

#endif//REFLECT_H_INCLUDED
