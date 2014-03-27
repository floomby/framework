#ifndef CRT_H_INCLUDED
#define CRT_H_INCLUDED

#include <cstdint>
#include <cstdarg>

extern "C" int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
extern "C" int system(const char *command);
extern "C" size_t strlen(const char *s);
extern "C" char *strstr(const char *haystack, const char *needle);
extern "C" int strcmp(const char *s, const char *t);
extern "C" char *strcpy(char *dest, const char *src);

#endif//CRT_H_INCLUDED
