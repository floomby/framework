#ifndef APIGET_H_INCLUDED
#define APIGET_H_INCLUDED

#include <cstdint>
#include <windows.h>

#include "../pe/pe_structs.h"
#include "../stuff/str.h"
#include "../internals/peb.h"

void *_GetFuncAddr(const char *, uint32_t);

#endif//APIGET_H_INCLUDED
