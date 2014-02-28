#include "reflect.h"

#include <cstdio>

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

int move_dll;

extern "C"
#ifdef NEED_META
__declspec(dllexport)
#endif//NEED_META
#ifdef FREESTANDING
__attribute__((optimize("omit-frame-pointer")))
#endif//FREESTANDING
void *_ReflectiveLoad(void *dll)
{
    char *kernel;
    __asm__ __volatile__(
        "lea 2(%%rip), %0;"
        "jmp .+15;"
        ".asciz \"kernel32.dll\";"
        : "=r" (kernel)
        :
        :
    );

    // parse kernel export table for address we need
    void *(*getModuleHandleA)(const char *);
    _GetFuncAddr(kernel, 644, getModuleHandleA);
    void *(*getProcAddress)(void *, const char *);
    _GetFuncAddr(kernel, 700, getProcAddress);

    void *kernel_handle = getModuleHandleA(kernel);
    
    void *(*vAlloc)(void *, size_t, uint32_t, uint32_t) = (void *(*)(void *, size_t, uint32_t, uint32_t))getProcAddress(kernel_handle, (char *)(uint64_t)1468);
    void *(*loadLib)(const char *) = (void *(*)(const char *))getProcAddress(kernel_handle, (char *)(uint64_t)964);

    // parse headers for move
    struct dos_header *DosHeader = (struct dos_header *)dll;
    struct nt_header *NtHeader = (struct nt_header *)rva_to_offset(dll, DosHeader->e_lfanew);
    struct section_header *sections = (struct section_header *)((uint8_t *)NtHeader + sizeof(struct nt_header) + 0x8);
    
    void *base;
    
    if(move_dll){
        // allocate space
        base = vAlloc(NULL, NtHeader->OptionalHeader.SizeOfImage, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    
        // copy headers
        MemCpy(base, dll, NtHeader->OptionalHeader.SizeOfHeaders);
    
        // copy sections
        for(int i = 0; i < NtHeader->FileHeader.NumberOfSections; i++){
            MemCpy(rva_to_offset(base, sections[i].addr), rva_to_offset(dll, sections[i].pointer_to_raw), sections[i].size_of_raw);   
        }
    }else{
        base = dll;
        ((VOID WINAPI (*)(UINT))getProcAddress(kernel_handle, (char *)(uint64_t)371))(123);
    }
    
    // do the relocations
    struct directory *rel = &NtHeader->OptionalHeader.DataDirectory.Relocation;
    
    uint8_t *it = (uint8_t *)rva_to_offset(base, rel->rva);
    
    while(it < (uint8_t *)rva_to_offset(base, rel->rva + rel->size)){
        uint32_t page_rva = *(uint32_t *)it;
        uint32_t block_count = *(uint32_t *)(it + 4);
        
        for(int i = 0; i < (block_count - 8) / 2; i += 2){
            uint16_t tmp = *(uint16_t *)(it + i + 8);
#ifdef FANCY_ERROR
            switch(realloc_type(tmp)){
                case IMAGE_REL_BASED_DIR64:
                    {
#endif//FANCY_ERROR                    
                    uint64_t rva = page_rva + realloc_loc(tmp);
                    *(uint64_t *)rva_to_offset(base, rva) += (uint64_t)base - NtHeader->OptionalHeader.ImageBase;
#ifdef FANCY_ERROR
                    }
                    break;
                    
                default:
                    ((VOID WINAPI (*)(UINT))getProcAddress(kernel_handle, (char *)(uint64_t)371))(realloc_type(tmp));      //ExitProcess
                    break;
            }
#endif//FANCY_ERROR
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
    
#ifdef LOAD_DEPS
    // the gas parser has an annoying limitation see
    // http://effbot.org/pyfaq/why-can-t-raw-strings-r-strings-end-with-a-backslash.htm
    // this is why we use fowards slashes
    char *search_path;
    __asm__ __volatile__(
        "lea 2(%%rip), %0;"
        "jmp .+23;"
        ".asciz \"C:/Windows/System32/\";"
        : "=r" (search_path)
        :
        :
    );
#endif//LOAD_DEPS
    
    while(imp->orig_thunk){
        void **thunk = (void **)rva_to_offset(base, imp->first_thunk);
        void *mod_base = getModuleHandleA((char *)rva_to_offset(base, imp->name));
        
#ifdef LOAD_DEPS
        if(mod_base == NULL){

            char *name = (char *)((uint8_t *)base + imp->name);
            char buf[StrLen(search_path) + StrLen(name) + 1];
            StrCpy(buf, search_path);
            StrCat(buf, name);

            mod_base = loadLib(buf);
        }
#endif//LOAD_DEPS

        if(move_dll){
            while(*import_lookup_table){
                if(ordinal(*import_lookup_table)){
                    *thunk = getProcAddress(mod_base, (char *)ordinal_number(*import_lookup_table));
                }else{
                    // TODO pe spec page 77 - hint/name table entry one (no name only pointer to export name table)
                    *thunk = getProcAddress(mod_base, (char *)rva_to_offset(base, hint_rva(*import_lookup_table) + 2));
                }
            
                thunk++;
                import_lookup_table++;
            }
        }
        
        import_lookup_table++;
        imp++;
    }
    
    // }

    // notify the dll it has been loaded
#ifdef NEED_META
    BOOL APIENTRY (*entry_fp)(void *, size_t) = (BOOL APIENTRY (*)(void *, size_t))rva_to_offset(base, NtHeader->OptionalHeader.AddressOfEntryPoint);
    entry_fp(base, NtHeader->OptionalHeader.SizeOfImage);
#else
    BOOL APIENTRY (*entry_fp)(HINSTANCE, DWORD, LPVOID) = (BOOL APIENTRY (*)(HINSTANCE, DWORD, LPVOID))rva_to_offset(base, NtHeader->OptionalHeader.AddressOfEntryPoint);
    entry_fp(NULL, DLL_PROCESS_ATTACH, NULL);
#endif//NEED_META
    
    //TODO check and make sure that the code below works
#ifndef FREESTANDING
    return base;
#else
    __asm__ __volatile__(
        "movq %0, %%rax;"
        "ret;"
        ".global ___chkstk_ms;"
        "___chkstk_ms:;"
        "pushq %%rcx;"
        "pushq %%rax;"
        "cmpq $0x1000, %%rax;"
        "lea 0x18(%%rsp), %%rcx;"
        "jb end;"
        "lp:;"
        "subq $0x1000, %%rcx;"
        "orq $0x0, (%%rcx);"
        "subq $0x1000, %%rax;"
        "cmpq $0x1000, %%rax;"
        "ja lp;"
        "end:;"
        "subq %%rax, %%rcx;"
        "orq $0x0, (%%rcx);"
        "popq %%rax;"
        "popq %%rcx;"
        "ret;"
        :
        : "r" (base)
        :
    );
#endif//FRESTANDING
}
