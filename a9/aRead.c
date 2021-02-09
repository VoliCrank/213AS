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
volatile unsigned int sum = 0;
int total = 0;
// You may add your own variables here

void interrupt_service_routine() {
  void *val;
  void (*callback)(void *, void *);
  queue_dequeue(pending_read_queue, &val, NULL, &callback);
  callback(val, NULL);
}

void handle_read(void *resultv, void *not_used) {
  int *val = resultv;
  sum += *val;
  total++;
}

int main(int argc, char **argv) {

  // Command Line Arguments
  static char *usage = "usage: aRead num_blocks";
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
  
  int results[num_blocks];

  for (int i = 0; i < num_blocks; i++) {
    disk_schedule_read(&results[i], i);
    queue_enqueue(pending_read_queue,&results[i],NULL, handle_read);
  }
  while (total != num_blocks);
  printf("%d\n", sum);
  return 0;
}
