#include "reflect.h"

#define realloc_type(x) ((x & 0xF000) >> 12)

#define IMAGE_REL_BASED_ABSOLUTE 0
#define IMAGE_REL_BASED_HIGH 1
#define IMAGE_REL_BASED_LOW 2
#define IMAGE_REL_BASED_HIGHLOW 3
#define IMAGE_REL_BASED_HIGHADJ 4
#define IMAGE_REL_BASED_DIR64 10

#define realloc_loc(x) (x & 0x0FFF)

#define ordinal(x)          (x & 0x8000000000000000)
#define ordinal_number(x)   ((x & 0x7FFF80000000000) >> 47)
#define hint_rva(x)         ((uint32_t)((x & 0x000007FFFFFFF)))

extern "C"
void *ReflectiveLoad(void *base)
{
    // parse headers for move
    struct dos_header *DosHeader = (struct dos_header *)base;
    struct nt_header *NtHeader = (struct nt_header *)rva_to_offset(base, DosHeader->e_lfanew);
    struct section_header *sections = (struct section_header *)((uint8_t *)NtHeader + sizeof(struct nt_header) + 0x8);
    
    // do the relocations
    struct directory *rel = &NtHeader->OptionalHeader.DataDirectory.Relocation;
    
    uint8_t *it = (uint8_t *)rva_to_offset(base, rel->rva);
    
    while(it < (uint8_t *)rva_to_offset(base, rel->rva + rel->size)){
        uint32_t page_rva = *(uint32_t *)it;
        uint32_t block_count = *(uint32_t *)(it + 4);
        
        for(int i = 0; i < (block_count - 8) / 2; i += 2){
            uint16_t tmp = *(uint16_t *)(it + i + 8);
            
            uint64_t rva = page_rva + realloc_loc(tmp);
            *(uint64_t *)rva_to_offset(base, rva) += (uint64_t)base - NtHeader->OptionalHeader.ImageBase;
        }

        it += block_count;
    }
    
    // load dependencies and populate the IAT
    struct directory *imp_dir = &NtHeader->OptionalHeader.DataDirectory.Import;
    
    int imp_count = 0;
    struct import_directory *imp = (struct import_directory *)rva_to_offset(base, imp_dir->rva);
    while(imp[imp_count].orig_thunk){
        imp_count++;
    }
    
    uint64_t *import_lookup_table = (uint64_t *)(imp + imp_count + 1);
    
    const char *search_path = "C:/Windows/System32/";
    
    while(imp->orig_thunk){
        void **thunk = (void **)rva_to_offset(base, imp->first_thunk);
        void *mod_base = GetModuleHandleA((char *)rva_to_offset(base, imp->name));
        
        if(mod_base == NULL){

            char *name = (char *)((uint8_t *)base + imp->name);
            char buf[strlen(search_path) + strlen(name) + 1];
            strcpy(buf, search_path);
            strcat(buf, name);

            mod_base = LoadLibrary(buf);
        }

        import_lookup_table++;
        imp++;
    }
    
    ((BOOL WINAPI (*)(HINSTANCE, DWORD, LPVOID))rva_to_offset(base, NtHeader->OptionalHeader.AddressOfEntryPoint))((HINSTANCE)base, DLL_PROCESS_ATTACH, NULL);

    return base;
}
