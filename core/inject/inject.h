#ifndef INJECT_H_INCLUDED
#define INJECT_H_INCLUDED

#include "../external.h"

#include "../../common/pe_structs.h"

bool InjectPid(void *what, size_t size, uint32_t where, uint32_t pid);

#endif//INJECT_H_INCLUDED
