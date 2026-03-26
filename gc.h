#ifndef GC_H
#define GC_H

#include <stddef.h>

typedef struct Header{
  size_t size;
  int marked;
  struct Header* next;
}Header;

#define INITIAL_THRESHOLD 1048576  // 1MB
#define TEST_THRESHOLD 100
void* gc_malloc(size_t s);
void gc_free(void* p);

#endif 
