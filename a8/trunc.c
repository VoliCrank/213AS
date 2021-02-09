
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print(element_t ev) {
  char **e = ev;
  if (*e != NULL) {
    printf("%s\n", *e);
  }
}
void print_num(element_t ev) {
  int *e = ev;
  printf("%d\n", *e);
}

void to_num(element_t *rv, element_t av) {
  char **str = av;
  int **ret = (int **)rv;
  if (*ret == NULL) {
    *ret = malloc(sizeof(int));
  }
  char *endp;
  int val = strtol(*str, &endp, 10);
  if (*str[0] != '\0' && endp[0] == '\0') {
    **ret = val;
  } else {
    **ret = -1;
  }
}

void to_str(element_t *rv, element_t av, element_t bv) {
  int *num = av;
  char **str = bv;
  char ***ret = (char ***)rv;
  if (*ret == NULL) {
    *ret = malloc(sizeof(char *));
  }
  if (*num != -1) {
    **ret = NULL;
  } else {
    **ret = strndup(*str, strlen(*str));
  }
}

int num_only(element_t av) {
  int *val = av;
  return *val >= 0;
}

int str_only(element_t av) {
  char **val = av;
  if (*val != NULL) {
    return 1;
  } else {
    return 0;
  }
}

void truncate(element_t *rv, element_t a1, element_t a2) {
  int *len = a2;
  char **str = a1;
  char ***ret = (char ***)rv;
  if (*ret == NULL) {
    *ret = malloc(sizeof(char *));
  }
  if (*len > strlen(*str)) {
    **ret = strdup(*str);
    return;
  }
  **ret = strndup(*str, *len);
}

void concat(element_t *rv, element_t av, element_t bv) {
  char ***ret = (char ***)rv;
  char **prev = (char **)av;
  char **curr = (char **)bv;
  if (*ret == NULL) {
    *ret = malloc(sizeof(char *));
  }
  if (strlen(*prev) == 0) {
    if (strlen(*curr) != 0) {
      **ret = strndup(*curr, strlen(*curr));
    } else {
      **ret = "";
    }
    return;
  }
  int length = sizeof((*prev)) + sizeof(*curr);
  char temp[length + 1];
  strcpy(temp, *prev);
  strcat(temp, " ");
  strncat(temp, *curr, strlen(*curr));
  free(**ret);
  **ret = strdup(temp);
}

void get_max(element_t *rv, element_t av, element_t bv) {
  int **ret = (int **)rv;
  int *prev = (int *)av;
  int *curr = (int *)bv;
  if (*prev < *curr) {
    **ret = *curr;
  }
}

void freeStr(element_t ev) {
  char **ret = (char **)ev;
  free(ret[0]);
  free(ret);
}

int main(int argc, char **argv) {
  // initialize list
  struct list *init = list_create();
  element_t *string = malloc(sizeof(*string) * (argc - 1));
  for (int i = 0; i < argc - 1; i++) {
    string[i] = &argv[i + 1];
  }

  list_append_array(init, string, argc - 1);

  // map list to ints and filter non ints
  struct list *ints = list_create();
  list_map1(to_num, ints, init);
  struct list *pureInts = list_create();
  list_filter(num_only, pureInts, ints);

  // map list to str and filter
  struct list *str = list_create();
  list_map2(to_str, str, ints, init);

  struct list *pureStr = list_create();
  list_filter(str_only, pureStr, str);

  struct list *comb = list_create();
  list_map2(truncate, comb, pureStr, pureInts);

  char *ep = "";
  char **combStr = &ep;
  list_foldl(concat, (element_t *)&combStr, comb);

  int max = 0;
  int *maxpt = &max;
  list_foldl(get_max, (element_t *)&maxpt, pureInts);

  list_foreach(print, comb);
  printf("%s\n", ep);
  printf("%d\n", max);

  list_foreach(free, ints);
  list_foreach(freeStr, str);
  list_foreach(freeStr, comb);
  list_destroy(init);
  list_destroy(ints);
  list_destroy(pureInts);
  list_destroy(str);
  list_destroy(pureStr);
  list_destroy(comb);
  if (ep != "")
  free(ep);
  free(string);
}