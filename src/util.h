#ifndef UTIL_H__
#define UTIL_H__

#include <stdlib.h>

//=============================================================================
// int stack
//=============================================================================

typedef struct _int_stack_t
{
  int  max;
  int  cur;
  int* arr;
}
int_stack_t;

//=============================================================================

void int_stack_destroy(int_stack_t* p);
int_stack_t* int_stack_create(int max)
{
  int_stack_t* p = (int_stack_t*)malloc(sizeof(int_stack_t));
  if (NULL == p)
    goto memory_fault;

  p->max = max;
  p->cur = -1;
  p->arr = NULL;

  p->arr = (int*)malloc(sizeof(int) * max);
  if (NULL == p->arr)
    goto memory_fault;

  return p;

memory_fault:
  int_stack_destroy(p);
  return NULL;
}

//=============================================================================

void int_stack_destroy(int_stack_t* p)
{
  free(p->arr);
  free(p);
}

//=============================================================================

int int_stack_pop(int_stack_t* p, int* val)
{
  if (p->cur >= 0)
    return (*val = p->arr[p->cur--]);
  return 0;
}

//=============================================================================

int int_stack_push(int_stack_t* p, int val)
{
  if (p->cur < p->max - 1)
    return (p->arr[++p->cur] = val);
  return 0;
}

#endif /* UTIL_H__ */
