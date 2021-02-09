#include <stdio.h>
#include <stdlib.h>

#include "element.h"
#include "str_element.h"
#include "int_element.h"
#include "refcount.h"



/* If the string is numeric, return an int_element. Otherwise return a str_element. */
struct element *parse_string(char *str) {
  char *endp;
  /* strtol returns a pointer to the first non-numeric character in endp.
     If it gets to the end of the string, that character will be the null terminator. */
  int value = strtol(str, &endp, 10);
  if(str[0] != '\0' && endp[0] == '\0') {
    /* String was non-empty and strtol conversion succeeded - integer */
    return (struct element *)int_element_new(value);
  } else {
    return (struct element *)str_element_new(str);
  }
}
struct element** arr;

int compare(const void* e1, const void* e2){
  struct element* a1= *(struct element**) e1;
  struct element* a2= *(struct element**) e2;
  return a1->class->compare(a1,a2);
}


int main(int argc, char **argv) {
 /* TODO: Read elements into a new array using parse_string */
 arr = malloc(sizeof(struct element*) * (argc-1));

  /* Parse input arguments */
  for (int i=0; i<argc-1; i++) {
    struct element* e = parse_string(argv[i+1]);
    arr[i] = e;
  }
  /* TODO: Sort elements with qsort */
//   for (int i = 0;i<argc-1;i++){
 qsort(arr,argc-1,sizeof(arr[0]),compare);
//   }
  // for (int i = argc-2; i > 0; i--){
  //   for (int j = 1; j <= i; j++){
  //     if (compare(arr[j-1],arr[j])>=0){
  //       struct element* temp = arr[j-1];
  //       arr[j-1] = arr[j];
  //       arr[j]= temp;
  //     }
  //   }
  // }
    printf("Sorted: ");
  for (int i = 0; i<argc-1;i++){
     arr[i]->class->print(arr[i]);
     printf(" ");
  }
  

  /* TODO: Print elements, separated by a space */
  printf("\n");
  for (int i = 0; i < argc - 1; i++){
    rc_free_ref(arr[i]);
  }
  free(arr);
}
