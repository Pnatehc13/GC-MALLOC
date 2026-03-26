#include "gc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
  #include <windows.h>
#else 
  extern char **environ;
#endif

void gc_mark(void);
void gc_collect(void);
void check_and_mark(void *p);
void insert(Header* h);

void* get_stack_bottom()
{
  void * bottom =NULL;
  #if defined(_WIN64)
    __asm__("mov %%gs:0x08, %0" : "=r"(bottom));
  #elif defined(_WIN32)
    __asm__("mov %%fs:0x04, %0" : "=r"(bottom));
  #else 
   bottom =(void*)environ;
  #endif
  return bottom;
}


Header* tail = NULL;
Header* head = NULL;
size_t curr_size = 0;


void insert(Header* h)
{
  if(head == NULL) head = tail =h;
  else 
  {
    tail->next = h;
    tail = h;
  }
}

void delete(Header* t)
{
  if(head == t) head = head->next;
  else 
  {
    Header* curr = head;
    while(curr->next != NULL)
    {
      if(curr->next == t){curr->next = t->next;break;}
      curr = curr->next;
    }
  }
}


void* gc_malloc(size_t s)
{
  if(curr_size >= INITIAL_THRESHOLD)
  {
    gc_mark();
    gc_collect();
  }
  Header* h = (Header*)malloc(sizeof(Header) + s);
  h->size = s;
  h->marked = 0;
  h->next = NULL;
  curr_size+=s;
  insert(h);
  return (void*)(h + 1);
}

void gc_free(void* p)
{
  Header* temp = (Header*)p-1;
  curr_size-=temp->size;
  delete(temp);
  free(temp);
}

void gc_collect()
{
  Header* curr = head;
  int c = 0;
  while(curr != NULL)
  {
    
    Header* next= curr->next;
    if(curr->marked == 0)
    {
      curr_size-=curr->size;
      delete(curr);
      free(curr);
      c++;
    }
    else 
      curr->marked = 0;
    curr = next;
  }
}

void gc_mark()
{
  void* stack_bottom = get_stack_bottom();
  void* rsp = __builtin_frame_address(0);

  uintptr_t* start = (uintptr_t*)rsp;
  uintptr_t* end = (uintptr_t*)stack_bottom;

  while(start<end)
  {
    check_and_mark((void*)*start);
    start++;
  }
}

void check_and_mark(void* p)
{
  Header* temp = head;
  while(temp!=NULL)
  {
    void* block_start = (void*)(temp+1);
    void* block_end = block_start+temp->size;
    if(p>=block_start && p<= block_end)
    {
      temp->marked = 1;
      return;
    }
    else 
      temp = temp->next;
  }
}
