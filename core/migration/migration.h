#ifndef MIGRATION_H_INCLUDED
#define MIGRATION_H_INCLUDED

#include "../external.h"

void Next(uint32_t pid, bool kill);
bool Migrate();

#endif//MIGRATION_H_INCLUDED
