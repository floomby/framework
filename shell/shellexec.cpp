
#include "sopar.h"

void ShellInit()
{
    // Create a pipes for std{err,in,out}
    HANDLE hStdInReadPipe;
    HANDLE hStdInWritePipe;
    
    HANDLE hStdOutReadPipe;
    HANDLE hStdOutWritePipe;
    
    HANDLE hStdErrReadPipe;
    HANDLE hStdErrWritePipe;
    
    SECURITY_ATTRIBUTES attrs;
    
    attrs.nLength = sizeof(SECURITY_ATTRIBUTES);
    attrs.lpSecurityDescriptor = NULL;
    attrs.bInheritHandle = TRUE;
    
    CreatePipe(&hStdInReadPipe,  &hStdInWritePipe,  &attrs, 0);
    CreatePipe(&hStdOutReadPipe, &hStdOutWritePipe, &attrs, 0);
    CreatePipe(&hStdErrReadPipe, &hStdErrWritePipe, &attrs, 0);
    
    // Duplicate the handles
    HANDLE hProc = (HANDLE)(uint64_t)GetCurrentProcessId();
    
//    DuplicateHandle(hProc, );
//    DuplicateHandle(hProc, );
//    DuplicateHandle(hProc, );

    
    
}



void ShellExecute(const char *what)
{
    system(what);
}

void ShellDone()
{
    // Close the pipes


}
