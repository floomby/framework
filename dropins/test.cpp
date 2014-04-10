#include "../core/external.h"

extern "C" __declspec(dllexport) void test(const char *arg)
{

  ShellExecute(NULL,
               "runas",
               "c:\\windows\\notepad.exe",
               NULL,
               NULL,
               SW_SHOWNORMAL
               );




}


extern "C" __declspec(dllexport) void test2(const char *arg)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;

    // open the file to patch in rw mode (note: this need exclusive privs to the file)
    // TODO: switch to using file locks
    if(INVALID_HANDLE_VALUE == (hFile = CreateFile(arg,
                                                   GENERIC_READ | GENERIC_WRITE,
                                                   0,
                                                   NULL,
                                                   CREATE_NEW,
                                                   FILE_ATTRIBUTE_NORMAL,
                                                   NULL
                                                   ))){
        sc_printf("unable to open %s: %d\n", arg, GetLastError());
        return;
    }
    
    DWORD num;
    
    void *rep = repack(DllMeta.curr.where);
    WriteFile(hFile, rep, disk_size(rep), &num, NULL);
    free_repack(rep);
    
    CloseHandle(hFile);
}
