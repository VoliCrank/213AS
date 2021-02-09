#include "uthread.h"
#include "uthread_mutex_cond.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3
uthread_t threads[NUM_THREADS];

uthread_mutex_t mx;
uthread_cond_t mc;
int num = 0;

void randomStall() {
  int i, r = random() >> 16;
  while (i++ < r)
    ;
}

void waitForAllOtherThreads() {
  while (num != NUM_THREADS -1){
    num++;
    uthread_cond_wait(mc);
  }
  uthread_cond_broadcast(mc);
}

void *p(void *v) {
  randomStall();
  uthread_mutex_lock(mx);
  printf("a\n");
  waitForAllOtherThreads();
  printf("b\n");
  uthread_mutex_unlock(mx);
  return NULL;
}

int main(int arg, char **arv) {
  mx = uthread_mutex_create();
  mc = uthread_cond_create(mx);
  uthread_init(4);
  for (int i = 0; i < NUM_THREADS; i++)
    threads[i] = uthread_create(p, NULL);
  for (int i = 0; i < NUM_THREADS; i++)
    uthread_join(threads[i], NULL);
  printf("------\n");
  uthread_mutex_destroy(mx);
  uthread_cond_destroy(mc);
}