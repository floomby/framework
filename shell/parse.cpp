#include "sopar.h"

// lack of propper support for std::regex in
// gcc 4.8.2 puts a kink in my plan so for
// right now we will roll our own parser

void sopar::parse()
{
    char *it = this->buf;
    while(*it && (*it != ' ')){
        it++;
    }
    
    *it = '\0';
    
    if(it == this->buf)
    {
        return;
    }
    else if(StrCaseCmp(this->buf, "cmd"))
    {
        this->sopar_send("executing cammand: ");
        this->sopar_send(it + 1);
        this->sopar_send("\n");
        return;
    }
    else if(StrCaseCmp(this->buf, "migrate"))
    {
        // uint32_t pid = atoi(it + 1);
        this->sopar_send("migrating to process ");
        this->sopar_send(it + 1);
        this->sopar_send("\n");
        return;
    }
    else if(StrCaseCmp(this->buf, "quit"))
    {
        this->cleanup();
    }
    else
    {
        this->sopar_send("undefined directive: ");
        this->sopar_send(this->buf);
        this->sopar_send("\n");
        return;
    }
}