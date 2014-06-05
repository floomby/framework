#include <iostream>
#include "../../common/pe_structs_v2.h"

#include "../common/loader.h"
#include "../common/binder.h"

using std::cout;
using std::endl;
using std::cerr;

void exports(void *dll);

int main(int argc, char *argv[])
{
    if(argc != 3){
        cerr << "usage: " << argv[0] << " <filename> <directive>" << endl;
        return 1;
    }
    
    size_t size;
    void *dll = load(argv[1], &size);
    
    void *base = expand(dll);
    
    free(dll);
    
    bind(argv[2], exports, base);
    
    free(base);
    
    return 0;
}

void exports(void *base)
{
    struct dos_header *dh = (struct dos_header *)base;
    struct nt_header *nh = (struct nt_header *)rva_to_offset(base, dh->e_lfanew);
    struct export_directory *ed = (struct export_directory *)rva_to_offset(base, nh->OptionalHeader.DataDirectories.Export.RVA);

    cout << "export dir rva: " << std::hex << nh->OptionalHeader.DataDirectories.Export.RVA << endl;
    cout << ed->NumberOfNames << endl;
    
    uint32_t *names = (uint32_t *)rva_to_offset(base, ed->AddressOfNames);
    uint32_t idx = 0;
    
    while(idx < ed->NumberOfNames){
    
        cout << (char *)rva_to_offset(base, names[idx]) << endl;
        
        idx++;
    }

}
