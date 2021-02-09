#include <stdio.h>

#include "mymalloc.h"

/* Feel free to change/remove these variables and/or add your own. */

// Base address of the allocated heap.
static void *heap;
// Size of the complete heap.
static int heapsize;
// Next unallocated location in the heap.
static int top;

struct list {
  struct node *head;
};

struct node {
  int data;
  struct node *next;
};
static struct node *head;

/* Initialize your memory manager to manage the given heap. */
void mymalloc_init(char *_heap, int _heapsize) {
  heap = _heap;
  heapsize = _heapsize;
  struct list * temp = (struct list*) heap;
  struct list *l = &temp[0];
  l->head = NULL;
  head = l->head;
  top = 8;
  /* TODO: Any other initialization you want to do */
  /* NOTE! Each call to mymalloc_init should initialize a brand new heap
     (discarding the old one), as it will be called once per test.
     Therefore, be sure to *initialize all your variables* here! */
}

/* Allocate a block of memory of the given size, or NULL if unable.

Returns: a pointer aligned to 8 bytes, or NULL if allocation failed. */
void *mymalloc(int size) {
  /* This is a dumb implementation of malloc, adapted slightly from the slides.

  You will augment this solution to make a real malloc. */

  /* Round `size` up to a multiple of 8 bytes */
  /* TODO: Make room for any extra metadata you want to store */
  size = (size + 7) / 8 * 8 + 8;

  // struct node *curr = head;
  // struct node *prev = head;
  // while (prev != NULL) {
  //   curr = curr->next;
  //   if (curr != NULL && curr->data >= size) {
  //     prev->next = curr->next;
  //     int *freed  = (void *)curr;
  //     *freed = size;
  //     return ((void *)freed) + 8;
  //   }
  //    prev = prev->next;
  //  }

  struct node *curr = head;
  struct node *prev = head;
  struct node *small = head;
  struct node *smallPrev = head;
  if (curr != NULL) {
    curr = curr->next;
    while (curr != NULL) {
      if (curr != NULL &&
          ((curr->data < small->data && curr->data - size >= 0) ||
           small->data - size < 0)) {
        smallPrev = prev;
        small = curr;
      }
      curr = curr->next;
      prev = prev->next;
    }
    if (small != NULL && small->data - size >= 0) {
      int *freed = (void *)small;
      *freed = size;
      if (small != head) {
        smallPrev->next = small->next;
      } else {
        head = head->next;
      }
      return ((void *)freed) + 8;
    }
  }

  /* TODO: Implement and walk a free list before allocating off `top` as a last
   * resort */
  if (size < 0 || size > heapsize || heapsize - size < top) {
    /* There is not enough room in the heap - fail */
    return NULL;
  }
  int* _heap = (int*) heap;
  /* Allocate the memory from `top` to `top+size` and return it */
  int *res = &_heap[top];
  *res = size;
  top += size;
  return ((void *)res) + 8;
}

/* Free the given block of memory. */
void myfree(void *ptr) {
  int *size = ptr - 8;
  struct node *temp = ptr - 8;
  temp->data = size[0];
  if (head == NULL) {
    head = temp;
    temp->next = NULL;
  } else {
    struct node *tempH = head;
    head = temp;
    temp->next = tempH;
  }
}
