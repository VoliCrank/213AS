#include "uthread.h"
#include "uthread_mutex_cond.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ITEMS 10
#define NUM_ITERATIONS 200
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_PROCESSORS 4

uthread_mutex_t mx;
uthread_cond_t haveSpace;
uthread_cond_t haveItems;

// histogram [i] == # of times list stored i items
int histogram[MAX_ITEMS + 1];

// number of items currently produced but not yet consumed
// invariant that you must maintain: 0 >= items >= MAX_ITEMS
int items = 0;

// if necessary wait until items < MAX_ITEMS and then increment items
// assertion checks the invariant that 0 >= items >= MAX_ITEMS
void *producer(void *v) {


  for (int i = 0; i < NUM_ITERATIONS; i++) {
  uthread_mutex_lock(mx);
    while (items >= MAX_ITEMS) {
      uthread_cond_wait(haveSpace);
    }
    items += 1;
    uthread_cond_signal(haveItems);
    assert(items >= 0 && items <= MAX_ITEMS);
    histogram[items]++;
    uthread_mutex_unlock(mx);
  }

  return NULL;
}

// if necessary wait until items > 0 and then decrement items
// assertion checks the invariant that 0 >= items >= MAX_ITEMS
void *consumer(void *v) {


  for (int i = 0; i < NUM_ITERATIONS; i++) {
      uthread_mutex_lock(mx);
    while (items <= 0){
      uthread_cond_wait(haveItems);
    }
    items -= 1;
    assert(items >= 0 && items <= MAX_ITEMS);
    histogram[items]++;
    uthread_cond_signal(haveSpace);
     uthread_mutex_unlock(mx);
  }

  return NULL;
}

int main(int argc, char **argv) {

  // init the thread system
  uthread_init(NUM_PROCESSORS);

  // start the threads
  mx = uthread_mutex_create();
  haveSpace = uthread_cond_create(mx);
  haveItems = uthread_cond_create(mx);
  uthread_t threads[NUM_PRODUCERS + NUM_CONSUMERS];
  for (int i = 0; i < NUM_PRODUCERS; i++)
    threads[i] = uthread_create(producer, NULL);
  for (int i = NUM_PRODUCERS; i < NUM_PRODUCERS + NUM_CONSUMERS; i++)
    threads[i] = uthread_create(consumer, NULL);

  // wait for threads to complete
  for (int i = 0; i < NUM_PRODUCERS + NUM_CONSUMERS; i++)
    uthread_join(threads[i], NULL);

  // sum up
  printf("items value histogram:\n");
  int sum = 0;
  for (int i = 0; i <= MAX_ITEMS; i++) {
    printf("  items=%d, %d times\n", i, histogram[i]);
    sum += histogram[i];
  }
  // checks invariant that ever change to items was recorded in histogram
  // exactly one
  assert(sum == (NUM_PRODUCERS + NUM_CONSUMERS) * NUM_ITERATIONS);
  uthread_mutex_destroy(mx);
  uthread_cond_destroy(haveSpace);
  uthread_cond_destroy(haveItems);
}
