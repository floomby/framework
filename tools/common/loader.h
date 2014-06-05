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
    
    fread(ret, 1, *size, fp);
    
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

    std::cout << "section header offset: " << std::hex << (uint64_t)offset_to_rva(dll, sections) << std::endl;

    // allocate space
    void *base = malloc(nh->OptionalHeader.SizeOfImage);
    assert(base);
    memset(base, 0, nh->OptionalHeader.SizeOfImage);
    
    // copy headers
    memcpy(base, dll, nh->OptionalHeader.SizeOfHeaders);
    
    std::cout << "number of sections rva: " << std::hex << (uint64_t)offset_to_rva(dll, &nh->FileHeader.NumberOfSections) << std::endl;
    
    std::cout << nh->FileHeader.NumberOfSections << std::endl;
    
    // copy sections
    for(int i = 0; i < nh->FileHeader.NumberOfSections; i++){
        memcpy(rva_to_offset(base, sections[i].VirtualAddress), rva_to_offset(dll, sections[i].PointerToRawData), sections[i].SizeOfRawData);
    }
    
    return base;
}

#endif//TOOL_LOADER_H_INCLUDED
