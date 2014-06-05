#ifndef TOOL_LOADER_H_INCLUDED
#define TOOL_LOADER_H_INCLUDED

#include <cassert>
#include <cstdlib>
#include <windows.h>

inline void *load(const char *path, size_t *size)
{
    char buf[4096];
    GetCurrentDirectory(sizeof(buf) - strlen(path) - sizeof(char), buf);
    strcat(buf, "\\");
    strcat(buf, path);
    
    FILE *fp = fopen(buf, "r");
    assert(fp);
    
    fseek(fp, 0, SEEK_END);
    *size = ftell(fp);
    rewind(fp);
    
    void *ret = malloc(*size);
    assert(ret);
    
    fread(ret, *size, 1, fp);
    
    fclose(fp);
    
    return ret;
}

#include "../../common/pe_structs_v2.h"
#include <iostream>

inline void *expand(void *dll)
{
    struct dos_header *dh = (struct dos_header *)dll;
    struct nt_header *nh = (struct nt_header *)rva_to_offset(dll, dh->e_lfanew);

    // TODO: replace with header sizes
    struct section_header *sections = (struct section_header *)rva_to_offset(nh, sizeof(struct nt_header));

    // allocate space
    void *base = malloc(nh->OptionalHeader.SizeOfImage);
    assert(base);
    memset(base, 0, nh->OptionalHeader.SizeOfImage);
    
    // copy headers
    memcpy(base, dll, nh->OptionalHeader.SizeOfHeaders);
    
    // I am completely bogled here but well make it work for now (think gcc is handling my structs strangly)
    nh->FileHeader.NumberOfSections = *(uint16_t *)rva_to_offset(dll, 0x85);
    
    // copy sections
    for(int i = 0; i < nh->FileHeader.NumberOfSections; i++){
        memcpy(rva_to_offset(base, sections[i].VirtualAddress), rva_to_offset(dll, sections[i].PointerToRawData), sections[i].SizeOfRawData);
    }
    
    return base;
}

#endif//TOOL_LOADER_H_INCLUDED
