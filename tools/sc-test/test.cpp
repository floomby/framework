#include <cstdio>
#include <windows.h>
#include <cassert>

void *load(const char *name, size_t *read)
{
    FILE *fp = fopen(name, "rb");
    assert(fp);
    
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);
    
    void *ret = malloc(size);
    assert(ret);
    
    ZeroMemory(ret, size);
    *read = size;
    
    fread(ret, 1, size, fp);
    
    fclose(fp);
    
    return ret;
}

int main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(stderr, "usage: %s <file>\n", argv[0]);
    return 1;
  }

  char buf[4096];
  GetCurrentDirectory(sizeof(buf) - strlen(argv[1]) - sizeof(TCHAR), buf);
  strcat(buf, "\\");
  strcat(buf, argv[1]);

  size_t size;
  void *loaded = load(buf, &size);

  void *sc = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  if(!sc){
    fprintf(stderr, "error allocating memmory: %d\n", GetLastError());
    return 1;
  }

  memcpy(sc, loaded, size);

  ((void (*)())sc)();
  
  return 0;
}
