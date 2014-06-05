/*
 * This is maybe the most usefull file in the entire
 * library. It makes manipulating PE files easy.
 *
 * This is version 2 of this file, compatibilty with
 * the first version has been broken.
 *
 */


#ifndef PE_STRUCTS_V2_H_INCLUDED
#define PE_STRUCTS_V2_H_INCLUDED

#include <cstdint>

#define rva_to_offset(base, rva)    ((void *)((uint8_t *)(base) + (rva)))
#define offset_to_rva(base, off)    ((uint64_t)(void *)off - (uint64_t)base)

#define round_up(size, step)        ((size % step)?((size / step + 1) * step):(size))
#define round_down(size, step)      ((size / step) * step)

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
    uint32_t RVA;
    uint32_t Size;
};

struct data_directories {
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
    struct data_directories DataDirectories;
    uint64_t Reserved;
};

struct nt_header {
    uint32_t Signature;
    struct file_header FileHeader;
    struct optional_header OptionalHeader;
};

struct section_header {
    char Name[8];
    uint32_t VirtualSize;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
};

struct export_directory {
    uint32_t Characteristics;
    uint32_t TimeDateStamp;
    uint16_t MajorVersion;
    uint16_t MinorVersion;
    uint32_t Name;
    uint32_t Base;
    uint32_t NumberOfFunctions;
    uint32_t NumberOfNames;
    uint32_t AddressOfFunctions;
    uint32_t AddressOfNames;
    uint32_t AddressOfOrdinals;
};

struct import_directory {
    uint32_t OriginalFirstThunk;
    uint32_t TimeDateStamp;
    uint32_t ForwarderChain;
    uint32_t Name;
    uint32_t FirstThunk;
};

#endif//PE_STUCTS_V2_H_INCLUDED
