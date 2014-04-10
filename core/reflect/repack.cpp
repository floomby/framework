#include "repack.h"

// finds the size of the file on disk
size_t disk_size(void *base)
{
    size_t size = 0;

    struct dos_header *dh = (struct dos_header *)base;
    struct nt_header *nh = (struct nt_header *)rva_to_offset(base, dh->e_lfanew);
    
    size += nh->OptionalHeader.SizeOfHeaders;
    
    struct section_header *sections = (struct section_header *)((uint8_t *)nh + sizeof(struct nt_header) + 0x8);
    
    for(int i = 0; i < nh->FileHeader.NumberOfSections; ++i){
        size += sections[i].size_of_raw;
    }

    return size;
}

// "repacks" a pe i.e. memory->disk
void *repack(void *base)
{
    size_t size = 0;

    struct dos_header *dh = (struct dos_header *)base;
    struct nt_header *nh = (struct nt_header *)rva_to_offset(base, dh->e_lfanew);
    
    size += nh->OptionalHeader.SizeOfHeaders;
    
    struct section_header *sections = (struct section_header *)((uint8_t *)nh + sizeof(struct nt_header) + 0x8);
    
    for(int i = 0; i < nh->FileHeader.NumberOfSections; ++i){
        size += sections[i].size_of_raw;
    }
    
    void *ret = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    
    memcpy(ret, base, nh->OptionalHeader.SizeOfHeaders);
    
    for(int i = 0; i < nh->FileHeader.NumberOfSections; ++i){
        memcpy(rva_to_offset(ret, sections[i].pointer_to_raw), rva_to_offset(base, sections[i].addr), sections[i].size_of_raw);
    }
    
    return ret;
}

// frees the memory allocated in repack
void free_repack(void *base)
{
    VirtualFree(base, disk_size(base), MEM_RELEASE);
}
