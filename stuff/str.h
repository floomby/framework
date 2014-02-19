#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED

#define is_upper(x) ((0x40 < x) && (x < 0x5B))
#define is_lower(x) ((0x60 < x) && (x < 0x7B))
#define to_upper(x) (x - 0x20)
#define to_lower(x) (x + 0x20)

inline __attribute__((always_inline)) bool StrCaseCmp(const char *str1, const char *str2)
{
    char *ita = (char *)str1;
    char *itb = (char *)str2;

    while(*ita && *itb){    
        char a = *ita;
        char b = *itb;
        
        if(is_upper(a)) a = to_lower(a);
        if(is_upper(b)) b = to_lower(b);
        
        if(!(a == b))   return false;
        
        ita++;
        itb++;
    }
    
    return true;
}

#endif//STR_H_INCLUDED
