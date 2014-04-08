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
