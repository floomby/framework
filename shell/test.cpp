#ifdef TESTER

#include <iostream>
#include <cstdlib>

#include "sopar.h"

// the tester for socket parser

int main(int argc, char *argv[])
{
    if(argc != 3){
        std::cerr << "usage: " << argv[0] << " <server> <port>" << std::endl;
        exit(EXIT_FAILURE);
    }

    sopar blah(argv[1], argv[2]);
    blah.go();
    
    return 0;
}

#endif//TESTER