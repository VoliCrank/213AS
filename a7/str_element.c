#include "str_element.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "refcount.h"

/* TODO: Implement all public str_element functions, including element interface
functions.

You may add your own private functions here too. */
/* Forward reference to a str_element. You get to define the structure. */
struct str_element {
  struct element_class *class;
  char *val;
};



void str_print(struct element *e) { 
  struct str_element * this = (struct str_element*) e;
  printf(this->val);
   }

int str_compare(struct element *e1, struct element *e2) {
  if (is_str_element(e1) == 1 && is_str_element(e2) == 1) {
    struct str_element *this1 = (struct str_element *)e1;
    struct str_element *this2 = (struct str_element *)e2;
    return strcmp(this1->val, this2->val);
  } else if (is_str_element(e1)) {
    return 1;
  } else if (is_str_element(e2)){
    return -1;
  }
  return -1;
}

void str_finalizer(void *e) { 
  struct str_element * this = e;
  free(this->val); 
  }

struct element_class str_class = {str_print, str_compare};
/* Static constructor that creates new string elements. */
struct str_element *str_element_new(char *value) {
  struct str_element *ret = rc_malloc(sizeof(*ret), str_finalizer);
  ret->val = strdup(value);
  ret->class = &str_class;
  return ret;
}
/* Static function that obtains the string from a str_element. The caller should
 * keep_ref it. */
char *str_element_get_value(struct str_element *e) { return e->val; }
/* Static function that determines whether this is a str_element. */
int is_str_element(struct element *e) {
  if ((e->class - &str_class) % sizeof(struct str_element*) == 0 )
  return 1;
  return 0;
}
