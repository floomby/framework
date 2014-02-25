#include "reflect.h"

#include <cstdio>

//TODO finish reworking this function

#define realloc_type(x) ((x & 0xF000) >> 12)

#define IMAGE_REL_BASED_ABSOLUTE 0
#define IMAGE_REL_BASED_HIGH 1
#define IMAGE_REL_BASED_LOW 2
#define IMAGE_REL_BASED_HIGHLOW 3
#define IMAGE_REL_BASED_HIGHADJ 4
#define IMAGE_REL_BASED_DIR64 10

#define realloc_loc(x) (x & 0x0FFF)

#define SEARCH_PATH "C:\\Windows\\System32\\"

#define ordinal(x)          (x & 0x8000000000000000)
#define ordinal_number(x)   ((x & 0x7FFF80000000000) >> 47)
#define hint_rva(x)         ((uint32_t)((x & 0x000007FFFFFFF)))

void _ReflectiveLoad(void *dll)
{
    char *kernel;
    __asm__ __volatile__(
        "lea test, %0;"
        "jmp done;"
        "test: .asciz \"kernel32.dll\";"
        "done:;"
        : "=r" (kernel)
        :
        :
    );

    // parse kernel export table for address we need
    void *(*getModuleHandleA)(const char *) = (void *(*)(const char *))_GetFuncAddr(kernel, 644);
    void *(*getProcAddress)(void *, const char *) = (void *(*)(void *,const char *))_GetFuncAddr(kernel, 700);

    void *kernel_handle = getModuleHandleA(kernel);
    
    void *(*vAlloc)(void *, size_t, uint32_t, uint32_t) = (void *(*)(void *, size_t, uint32_t, uint32_t))getProcAddress(kernel_handle, (char *)(uint64_t)1468);
    void *(*loadLib)(const char *) = (void *(*)(const char *))getProcAddress(kernel_handle, (char *)(uint64_t)963);

    // parse headers for move
    struct dos_header *DosHeader = (struct dos_header *)dll;
    struct nt_header *NtHeader = (struct nt_header *)rva_to_offset(dll, DosHeader->e_lfanew);
    
    // allocate space
    void *base = vAlloc(NULL, NtHeader->OptionalHeader.SizeOfImage, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    
    // copy headers
    MemCpy(base, dll, NtHeader->OptionalHeader.SizeOfHeaders);
    
    // copy sections
    struct section_header *sections = (struct section_header *)((uint8_t *)NtHeader + sizeof(struct nt_header) + 0x8);
    
    for(int i = 0; i < NtHeader->FileHeader.NumberOfSections; i++){
        MemCpy(rva_to_offset(base, sections[i].addr), rva_to_offset(dll, sections[i].pointer_to_raw), sections[i].size_of_raw);   
    }
    
    // do the relocations
    struct directory *rel = &NtHeader->OptionalHeader.DataDirectory.Relocation;
    
    uint8_t *it = (uint8_t *)rva_to_offset(base, rel->rva);
    
    while(it < (uint8_t *)rva_to_offset(base, rel->rva + rel->size)){
        uint32_t page_rva = *(uint32_t *)it;
        uint32_t block_count = *(uint32_t *)(it + 4);
        
        for(int i = 0; i < (block_count - 8) / 2; i += 2){
            uint16_t tmp = *(uint16_t *)(it + i + 8);
        
            if(realloc_type(tmp) != 0xa){
                // relocation type not supported
                VOID WINAPI (*exit_)(UINT) = (VOID WINAPI (*)(UINT))_GetFuncAddr(kernel, 371);      //ExitProcess
                exit_(0);
            }

            uint64_t rva = page_rva + realloc_loc(tmp);
            *(uint64_t *)rva_to_offset(base, rva) += (uint64_t)base - NtHeader->OptionalHeader.ImageBase;
        }

        it += block_count;
    }
    
    // load dependencies and populate the IAT
    
    // TODO redo the following code
    struct directory *imp_dir = &NtHeader->OptionalHeader.DataDirectory.Import;
    
    int imp_count = 0;
    struct import_directory *imp = (struct import_directory *)rva_to_offset(base, imp_dir->rva);
    while(imp[imp_count].orig_thunk){
        imp_count++;
    }
    
    uint64_t *imp_entry_table = (uint64_t *)rva_to_offset(base, imp_dir->rva + sizeof(struct import_directory) * (imp_count + 1));
    
    void **thunk = (void **)rva_to_offset(base, imp->first_thunk);
    
    while(imp->orig_thunk){
        puts((char *)rva_to_offset(base, imp->name));
        void *mod_base = getModuleHandleA((char *)rva_to_offset(base, imp->name));
        
        // TODO I need to redo the string processing
        
        if(mod_base == NULL){
            char *name = (char *)((uint8_t *)base + imp->name);
            char buf[strlen(SEARCH_PATH) + strlen(name) + 1];
            strcpy(buf, SEARCH_PATH);
            strcat(buf, name);

            mod_base = loadLib(buf);
        }
        
        while(*imp_entry_table){
        
            if(ordinal(*imp_entry_table)){
                //*thunk = getProcAddress(mod_base, (char *)ordinal_number(*imp_entry_table));
            }else{
                //*thunk = getProcAddress(mod_base, (char *)rva_to_offset(base, hint_rva(*imp_entry_table) + 2));
            }
            
            thunk++;
            
            imp_entry_table++;
        }
        
        imp_entry_table++;
        imp++;
    }
    
    puts("here");
    fflush(stdout);
    
    // notify the dll it has been loaded (am I suppose to call thread attach as well?)
    BOOL APIENTRY (*entry_fp)(HINSTANCE, DWORD, LPVOID) = (BOOL APIENTRY (*)(HINSTANCE, DWORD, LPVOID))rva_to_offset(base, NtHeader->OptionalHeader.AddressOfEntryPoint);
    entry_fp(NULL, DLL_PROCESS_ATTACH, NULL);
}
