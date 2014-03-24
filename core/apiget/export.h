#ifndef EXPORT_H_INCLUDED
#define EXPORT_H_INCLUDED

#include "../../common/pe_structs.h"

// does a linear search of a dll's export table
void *GetExport(void *base, const char *name);


#endif//EXPORT_H_INCLUDED
