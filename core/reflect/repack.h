#ifndef REPACK_H_INCLUDED
#define REPACK_H_INCLUDED

#include <windows.h>
#include "../../common/pe_structs.h"

void *repack(void *base);
void free_repack(void *base);
size_t disk_size(void *base);


#endif//REPACK_H_INCLUDED
