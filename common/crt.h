#ifndef CRT_H_INCLUDED
#define CRT_H_INCLUDED

extern "C" void sprintf(char *buf, const char *fmt, ...);
extern "C" int system(const char *command);
extern "C" size_t strlen(const char *s);
extern "C" int strcmp(const char *s, const char *t);

#endif//CRT_H_INCLUDED
