#include "disk.h"
#include "queue.h"
#include "uthread.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

queue_t pending_read_queue;
unsigned int sum = 0;
int *results;

void interrupt_service_routine() {
  void *t;
  queue_dequeue(pending_read_queue, &t, NULL, NULL);
  uthread_unblock(t);
}

void *read_block(void *arg) {
  int i = *(int *)arg;
  //printf("sum is %d\n", i);
  disk_schedule_read(&results[i], i);
  uthread_t t = uthread_self();
  queue_enqueue(pending_read_queue, t, NULL, NULL);
  uthread_block();
  sum += results[i];
  return NULL;
}

int main(int argc, char **argv) {
  // Command Line Arguments
  static char *usage = "usage: tRead num_blocks";
  int num_blocks;
  char *endptr;
  if (argc == 2)
    num_blocks = strtol(argv[1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init(1);
  disk_start(interrupt_service_routine);
  pending_read_queue = queue_create();
  // Sum Blocks
  uthread_t last;
  int temp[num_blocks];
  results = malloc(sizeof(int) * num_blocks);
  for (int i = 0; i < num_blocks; i++) {
    temp[i] = i;
    uthread_t t = uthread_create(read_block, &temp[i]);
    last= t;
  }
    uthread_join(last, NULL);
  printf("%d\n", sum);
  return 0;
}
