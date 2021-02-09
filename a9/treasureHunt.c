#include "disk.h"
#include "queue.h"
#include "uthread.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

queue_t pending_read_queue;
 int *final = NULL;
 int *results;

void interrupt_service_routine() {
  void *val;
  void *arg;
  void (*callback)(void *, void *);
  queue_dequeue(pending_read_queue, &val, &arg, &callback);
    //printf("%d\n", *(int*)val);

  callback(val, arg);
}

void handleOtherReads(void *resultv, void *countv) {
  // TODO
  int *result = (int *)resultv;
  int *count = (int*) countv;
  *count = *count - 1;
  //printf("Performed a read %d, %d\n", *result, *count);
  int val;
  if (*count != 0) {
    disk_schedule_read(&results[*count], *result);
    queue_enqueue(pending_read_queue, &results[*count], count, handleOtherReads);
  } else {
    final = result;
  }
}

void handleFirstRead(void *resultv, void *countv) {
  // TODO
  int *result = (int *)resultv;
  //printf("result is %d\n", *result);
  int ret;
  results = malloc(sizeof(int)* (*result + 1));
  disk_schedule_read(&results[*result], *result);
  queue_enqueue(pending_read_queue, &results[*result], result, handleOtherReads);
}

int main(int argc, char **argv) {
  // Command Line Arguments
  static char *usage = "usage: treasureHunt starting_block_number";
  int starting_block_number;
  char *endptr;
  if (argc == 2)
    starting_block_number = strtol(argv[1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init(1);
  disk_start(interrupt_service_routine);
  pending_read_queue = queue_create();
  int result;
  disk_schedule_read(&result, starting_block_number);
  queue_enqueue(pending_read_queue, &result, NULL, handleFirstRead);

  // Start the Hunt
  // TODO
  while (final == NULL); // infinite loop so that main doesn't return before hunt completes
  printf("%d\n",*final);
}
