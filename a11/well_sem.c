#include "uthread.h"
#include "uthread_sem.h"
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
  // TODO
  int currType;
  int drinking[2];
  int waiting[2];
};

struct Well *createWell() {
  struct Well *Well = malloc(sizeof(struct Well));
  Well->currType = -1;
  Well->drinking[0] = 0;
  Well->drinking[1] = 0;
  Well->waiting[0] = 0;
  Well->waiting[1] = 0;
  // TODO
  return Well;
}

struct Well *Well;

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int entryTicker; // incremented with each entry
int waitingHistogram[WAITING_HISTOGRAM_SIZE];
int waitingHistogramOverflow;
uthread_sem_t waitingHistogramMutex;
int occupancyHistogram[2][MAX_OCCUPANCY + 1];
uthread_sem_t wellMutex;
uthread_sem_t queue[2];
uthread_sem_t typeLock[2];
uthread_sem_t tickerMutex;
int todo[2];
int done[2];

void recordWaitingTime(int waitingTime) {
  uthread_sem_wait(waitingHistogramMutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram[waitingTime]++;
  else
    waitingHistogramOverflow++;
  uthread_sem_signal(waitingHistogramMutex);
}

void enterWell(enum Endianness g) {
  uthread_sem_wait(wellMutex);
  if (entryTicker == 0) {
    Well->currType = g;
  }
  entryTicker++;

  int curr = entryTicker;
  // printf("waiting well %d %d\n", g, Well->currType);
  Well->waiting[g]++;
  //printf("waiting %d, %d and %d\n", g, Well->waiting[g], Well->waiting[!g]);

  uthread_sem_signal(wellMutex);
  uthread_sem_wait(queue[g]);

  uthread_sem_wait(wellMutex);
  //printf("entered %d\n", g);
  Well->waiting[g]--;
  Well->drinking[g]++;
  // printf("incremented drinking with respect to %d, current drinking is %d, "
  //        "currtype is %d\n",
  //        g, Well->drinking[g], Well->currType);
  occupancyHistogram[g][Well->drinking[g]]++;
  assert(Well->drinking[!g] == 0);
  assert(Well->drinking[g] <= MAX_OCCUPANCY && Well->drinking[g] >= 0);
  recordWaitingTime(entryTicker - curr);
  uthread_sem_signal(wellMutex);

  // TODO
}

void leaveWell() {

  uthread_sem_wait(wellMutex);
  Well->drinking[Well->currType]--;
  // printf("leaving: there are %d drinking, %d waiting, %d other waiting and "
  //        "type is %d\n",
  //        Well->drinking[Well->currType], Well->waiting[Well->currType],
  //        Well->waiting[!Well->currType], Well->currType);
  if (Well->drinking[Well->currType] == 0) {
    if (Well->waiting[!Well->currType] != 0) {
      Well->currType = !Well->currType;
      int num = Well->waiting[Well->currType];
      //printf("changed type to %d\n", Well->currType);
      if (Well->waiting[Well->currType] > MAX_OCCUPANCY) {
        num = MAX_OCCUPANCY;
      }
      for (int i = 0; i < num; i++) {
        uthread_sem_signal(queue[Well->currType]);
      }
    } else {
      uthread_sem_signal(queue[Well->currType]);
    }
  }
  uthread_sem_signal(wellMutex);
  // TODO
}

void *wait(void *av) {
  int *type = av;
  if (entryTicker == 0) {
    for (int i = 0; i < 1; i++) {
      uthread_sem_signal(queue[(enum Endianness)type]);
    }
  }
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    enterWell((enum Endianness)type);
    for (int j = 0; j < NUM_PEOPLE; j++) {
      uthread_yield();
    }

    leaveWell();
    for (int j = 0; j < NUM_PEOPLE * 4; j++) {
      uthread_yield();
    }
  }
}
//
// TODO
// You will probably need to create some additional produres etc.
//

int main(int argc, char **argv) {
  uthread_init(1);
  Well = createWell();
  entryTicker = 0;
  uthread_t pt[NUM_PEOPLE];
  srand(time(0));
  waitingHistogramMutex = uthread_sem_create(1);
  wellMutex = uthread_sem_create(1);
  queue[0] = uthread_sem_create(0);
  queue[1] = uthread_sem_create(0);
  typeLock[0] = uthread_sem_create(0);
  typeLock[1] = uthread_sem_create(0);
  tickerMutex = uthread_sem_create(1);
  for (int i = 0; i < NUM_PEOPLE; i++) {
    if (rand() % 2 == 0) {
      pt[i] = uthread_create(wait, (int *)BIG);
    } else {
      pt[i] = uthread_create(wait, (int *)LITTLE);
    }
  }
  for (int i = 0; i < NUM_PEOPLE; i++) {
    uthread_join(pt[i], NULL);
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