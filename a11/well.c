#include "uthread.h"
#include "uthread_mutex_cond.h"
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf(S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

#define MAX_OCCUPANCY 3
#define NUM_ITERATIONS 100
#define NUM_PEOPLE 20
#define FAIR_WAITING_COUNT 4

/**
 * You might find these declarations useful.
 */
enum Endianness { LITTLE = 0, BIG = 1 };
const static enum Endianness oppositeEnd[] = {BIG, LITTLE};

struct Well {
  uthread_mutex_t mx;
  uthread_cond_t wait[2];
  int hold;
  int currType;
  int drinking[2];
  int waiting[2];
  // TODO
};

struct Well *createWell() {
  struct Well *Well = malloc(sizeof(struct Well));
  Well->mx = uthread_mutex_create();
  Well->wait[0] = uthread_cond_create(Well->mx);
  Well->wait[1] = uthread_cond_create(Well->mx);
  Well->hold = 0;
  Well->currType = 0;
  Well->drinking[0] = 0;
  Well->drinking[1] = 0;
  Well->waiting[0] = 0;
  Well->waiting[1] = 0;
  // TODO
  return Well;
}

struct Well *Well;
int consec = 0;
int min = -1;

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int entryTicker; // incremented with each entry
int waitingHistogram[WAITING_HISTOGRAM_SIZE];
int waitingHistogramOverflow;
uthread_mutex_t waitingHistogrammutex;
int occupancyHistogram[2][MAX_OCCUPANCY + 1];

void recordWaitingTime(int waitingTime) {
  uthread_mutex_lock(waitingHistogrammutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram[waitingTime]++;
  else
    waitingHistogramOverflow++;
  uthread_mutex_unlock(waitingHistogrammutex);
}

void enterWell(enum Endianness g) {
  //printf("came into a thread %d, is waiting %d\n", g, Well->waiting[g]);
  if (entryTicker == 0) {
    Well->currType = g;
  }
  int curr = entryTicker;
  if (min == -1) {
    min = curr;
  }
  entryTicker++;
  if (Well->drinking[!g]== 0){
    Well->currType = g;
  }
  while (Well->drinking[g] >= MAX_OCCUPANCY || Well->currType != g ||
         Well->hold == 1) {
    Well->waiting[g]++;
    uthread_cond_wait(Well->wait[g]);
    Well->waiting[g]--;
  }

  Well->drinking[g]++;
  assert(Well->drinking[!g] == 0);
  assert(Well->drinking[g] <= MAX_OCCUPANCY && Well->drinking[g] >= 1);
  recordWaitingTime(entryTicker - curr);
  uthread_mutex_lock(waitingHistogrammutex);
  occupancyHistogram[g][Well->drinking[g]]++;
  uthread_mutex_unlock(waitingHistogrammutex);
  if (curr - min > FAIR_WAITING_COUNT && min != -1) {
    Well->hold = 1;
    //printf("changing %d, %d\n", curr, min);
    min = -1;
  } 

  // TODO
}

void leaveWell() {

  Well->drinking[Well->currType]--;
  // printf("%d is drinking %d, %d, hold is %d and %d is drinking\n", Well->currType,
  //        Well->waiting[!Well->currType], Well->waiting[Well->currType],Well->hold, Well->drinking[Well->currType]);
  // Well->drinking[Well->currType]); switch if fair waiting has been reached
  if (Well->hold == 1) {
    if (Well->waiting[!Well->currType] == 0) {
      Well->hold = 0;
      uthread_cond_broadcast(Well->wait[Well->currType]);
    } else if (Well->drinking[Well->currType] == 0) {
      Well->hold = 0;
      Well->currType = !Well->currType;
      uthread_cond_broadcast(Well->wait[Well->currType]);
    }
  } else if (Well->drinking[Well->currType] == 0) {
    if (Well->waiting[!Well->currType] != 0) {
      Well->currType = !Well->currType;
    }
    uthread_cond_broadcast(Well->wait[Well->currType]);
  }
}

//
// TODO
// You will probably need to create some additional produres etc.
//
void *wait(void *av) {
  int *type = av;
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    uthread_mutex_lock(Well->mx);
    enterWell((enum Endianness)type);
    uthread_mutex_unlock(Well->mx);
    for (int j = 0; j < NUM_PEOPLE; j++) {
      uthread_yield();
    }
    uthread_mutex_lock(Well->mx);
    leaveWell();
    uthread_mutex_unlock(Well->mx);
    for (int j = 0; j < NUM_PEOPLE *4; j++) {
      uthread_yield();
    }
  }
  return NULL;
}

int main(int argc, char **argv) {
  uthread_init(1);
  Well = createWell();
  entryTicker = 0;
  uthread_t pt[NUM_PEOPLE];
  waitingHistogrammutex = uthread_mutex_create();
  uthread_t threads[NUM_PEOPLE];
  srandom(time(0));
  for (int i = 0; i < NUM_PEOPLE; i++) {
    if (rand() % 2 == 0) {
      threads[i] = uthread_create(wait, (int *)BIG);
    } else {
      threads[i] = uthread_create(wait, (int *)LITTLE);
    }
  }
  for (int i = 0; i < NUM_PEOPLE; i++) {
    uthread_join(threads[i], NULL);
  }
  // TODO

  printf("Times with 1 little endian %d\n", occupancyHistogram[LITTLE][1]);
  printf("Times with 2 little endian %d\n", occupancyHistogram[LITTLE][2]);
  printf("Times with 3 little endian %d\n", occupancyHistogram[LITTLE][3]);
  printf("Times with 1 big endian    %d\n", occupancyHistogram[BIG][1]);
  printf("Times with 2 big endian    %d\n", occupancyHistogram[BIG][2]);
  printf("Times with 3 big endian    %d\n", occupancyHistogram[BIG][3]);
  printf("Waiting Histogram\n");
  for (int i = 0; i < WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram[i])
      printf("  Number of times people waited for %d %s to enter: %d\n", i,
             i == 1 ? "person" : "people", waitingHistogram[i]);
  if (waitingHistogramOverflow)
    printf("  Number of times people waited more than %d entries: %d\n",
           WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}
