#include <stdio.h>
#include "gc.h"

int main()
{
  int* p = (int*)gc_malloc(sizeof(int)*5);
  for(int i =0;i<5;i++)
    p[i] = i;
  for(int i=0;i<5;i++)
    printf("%d\n",p[i]);
}
