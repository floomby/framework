#ifndef APIGET_H_INCLUDED
#define APIGET_H_INCLUDED

#include <cstdint>
#include <windows.h>

#include "../../../common/pe_structs.h"
#include "../../../common/peb.h"
#include "str.h"


#define _GetFuncAddr(module, ordinal, addr) \
{ \
    struct _ldr_data_table_entry *the_data; \
    __asm__ __volatile__( \
        "movq %%gs:0x60, %%rax;" \
        "movq 0x18(%%rax), %%rbx;" \
        "movq 0x18(%%rbx), %0;" \
        : "=r" (the_data) \
        : \
        : \
    ); \
    struct _ldr_data_table_entry *stop = the_data; \
    while(stop != (struct _ldr_data_table_entry *)(the_data->links.prev - 1)){ \
        struct nt_header *the_nt_header = (struct nt_header *)rva_to_offset(the_data->base, ((struct dos_header *)the_data->base)->e_lfanew); \
        struct export_directory *the_export_dir = (struct export_directory *)   \
            rva_to_offset(the_data->base, the_nt_header->OptionalHeader.DataDirectory.Export.rva); \
        char *name = (char *)rva_to_offset(the_data->base, the_export_dir->name); \
        if(StrCaseCmp(name, (module))){ \
            uint32_t *funcs = (uint32_t *)rva_to_offset(the_data->base, the_export_dir->addr_funcs); \
            (addr) = (decltype(addr))rva_to_offset(the_data->base, funcs[(ordinal)]); \
            break; \
        } \
        the_data = (struct _ldr_data_table_entry *)(the_data->links.prev - 1); \
    } \
}

#endif//APIGET_H_INCLUDED
