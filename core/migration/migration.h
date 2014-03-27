#ifndef MIGRATION_H_INCLUDED
#define MIGRATION_H_INCLUDED

#include <cstdint>

void Next(uint32_t pid);
bool Migrate();

#endif//MIGRATION_H_INCLUDED
