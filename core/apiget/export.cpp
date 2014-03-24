#include "export.h"
#include "../../common/crt.h"

void *GetExport(void *base, const char *name)
{
    struct dos_header *dh = (struct dos_header *)base;
    struct nt_header *nh = (struct nt_header *)rva_to_offset(base, dh->e_lfanew);

    struct export_directory *ed = (struct export_directory *)rva_to_offset(base, nh->OptionalHeader.DataDirectory.Export.rva);
    
    char *it = (char *)rva_to_offset(base, *((uint32_t *)rva_to_offset(base, ed->addr_names)));
    uint32_t idx = 0;
    
    while(idx < ed->num_names){
    
        if(!strcmp(name, it))
            return rva_to_offset(base, ((uint32_t *)rva_to_offset(base, ed->addr_funcs))[idx]);
        
        while(*it)
            it++;
        
        it++;
        idx++;
    }
    
    return NULL;
}


