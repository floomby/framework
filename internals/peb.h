#ifndef PEB_H_INCLUDED
#define PEB_H_INCLUDED

struct _unicode_string {
    unsigned short length;
    unsigned short max_length;
    wchar_t *buf;
};

struct _entry_t {
    struct _entry_t *next;
    struct _entry_t *prev;
};

struct _ldr_data_table_entry {
    void *res1[2];
    struct _entry_t links;
    void *res2[2];
    void *base;
    void *entry_point;
    void *res3;
    struct _unicode_string name;
    char res4[8];
    void *res5[3];
    union {
        unsigned long checksum;
        void *res6;
    };
    unsigned long timestamp;
};

#endif//PEB_H_INCLUDED
