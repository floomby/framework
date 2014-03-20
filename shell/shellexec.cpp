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
    
    LPSECURITY_ATTRIBUTES attrs;
    
    attrs.nLength = sizeof(LPSECURITY_ATTRIBUTES);
    attrs.lpSecurityDescriptor = NULL;
    attrs.bInheritHandle = TRUE;
    
    CreatePipe(&hStdInReadPipe,  &hStdInWritePipe,  &attrs, 0);
    CreatePipe(&hStdOutReadPipe, &hStdOutWritePipe, &attrs, 0);
    CreatePipe(&hStdErrReadPipe, &hStdErrWritePipe, &attrs, 0);
    
    // Duplicate the handles
    HANDLE hProc = GetCurrentProcessId();
    
    DuplicateHandle(hProc, );
    DuplicateHandle(hProc, );
    DuplicateHandle(hProc, );

    
    
}

void ShellExecute(const char *what)
{
    // Create an instance of cmd.exe to run the command
    
    
    
    // Buffer the pipes over the socket until termination
    
    
    
    // get the return code (if possible)(optinal)
    
    
    
}



void ShellDone()
{
    // Close the pipes


}