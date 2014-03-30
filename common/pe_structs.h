#ifndef PE_STRUCTS_H_INCLUDED
#define PE_STRUCTS_H_INCLUDED

#include <cstdint>

#define rva_to_offset(base, rva)    ((void *)((uint8_t *)(base) + (rva)))
#define offset_to_rva(base, off)    ((uint64_t)(void *)off - (uint64_t)base)

#define round_up(size, step)        ((size % step)?((size / step + 1) * step):(size))

struct dos_header {
    uint16_t e_magic;
    uint16_t e_cblp;
    uint16_t e_cp;
    uint16_t e_celc;
    uint16_t e_cdarhdr;
    uint16_t e_minalloc;
    uint16_t e_maxalloc;
    uint16_t e_ss;
    uint16_t e_sp;
    uint16_t e_csum;
    uint16_t e_ip;
    uint16_t e_cs;
    uint16_t e_lfarlc;
    uint16_t e_ovno;
    uint16_t e_res[4];
    uint16_t e_oemid;
    uint16_t e_res2[10];
    uint32_t e_lfanew;
};

struct file_header {
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
};

struct directory {
    uint32_t rva;
    uint32_t size;
};

struct data_directory{
    struct directory Export;
    struct directory Import;
    struct directory Resource;
    struct directory Exception;
    struct directory Security;
    struct directory Relocation;
    struct directory Debug;
    struct directory Architecture;
    struct directory Reserved;
    struct directory TLS;
    struct directory LoadConfig;
    struct directory BoundImport;
    struct directory ImportAddress;
    struct directory DelayImport;
    struct directory NET;
};

struct optional_header {
    uint16_t Magic;
    uint8_t  MajorLikerVersion;
    uint8_t  MinorLinkerVersion;
    uint32_t SizeOfCode;
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;
    uint32_t BaseOfCode;
    uint64_t ImageBase;
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOperatingSystemVersion;
    uint16_t MinorOperatingSystemVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;
    uint32_t SizeOfHeaders;
    uint32_t CheckSum;
    uint16_t Subsystem;
    uint16_t DllCharacteristics;
    uint64_t SizeOfStackReserve;
    uint64_t SizeOfStackCommit;
    uint64_t SizeOfHeapReserve;
    uint64_t SizeOfHeapCommit;
    uint32_t LoaderFlags;
    uint32_t NumberOfRvaAndSize;
    struct data_directory DataDirectory;
};

struct nt_header {
    uint32_t Signature;
    struct file_header FileHeader;
    struct optional_header OptionalHeader;
};

struct section_header {
    char name[8];
    uint32_t size;
    uint32_t addr;
    uint32_t size_of_raw;
    uint32_t pointer_to_raw;
    uint32_t pointer_to_relocations;
    uint32_t pointer_to_line_numbers;
    uint16_t num_of_relocations;
    uint16_t num_of_line_numbers;
    uint32_t charecteristics;
};

struct export_directory {
    uint32_t chariteristics;
    uint32_t timestamp;
    uint16_t major_ver;
    uint16_t minor_ver;
    uint32_t name;
    uint32_t base;
    uint32_t num_funcs;
    uint32_t num_names;
    uint32_t addr_funcs;
    uint32_t addr_names;
    uint32_t addr_ords;
};

struct import_directory {
    uint32_t orig_thunk;
    uint32_t timestamp;
    uint32_t forwarder_chain;
    uint32_t name;
    uint32_t first_thunk;
};

#endif//PE_STUCTS_H_INCLUDED
