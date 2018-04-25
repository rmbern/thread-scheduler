#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ucontext.h>
#include "thread.h"

void new_thread(void * args)
{
  int val = *(int*)args;
  free(args);
  while(1)
  {
    printf("I AM A NEW THREAD!\n");
    printf("MY ARG IS %d\n", val);
    sleep(1);
    thread_yield();
  }
}

void newer_thread(void * args)
{
  int val = *(int*)args;
  free(args);
  while(1)
  {
    printf("I AM A NEWER THREAD!\n");
    printf("MY ARG IS %d\n", val);
    sleep(1);
    thread_exit();
  }
}

int main()
{
  ucontext_t thread;
  ucontext_t newerthread;
  int * val1 = malloc(sizeof(val1));
  *val1 = 1;
  int * val2 = malloc(sizeof(val2));
  *val2 = 2;
  thread_create(thread, new_thread, (void*)val1);
  thread_create(newerthread, newer_thread, (void*)val2);
  while(1)
  {
    printf("I AM THE MAIN THREAD!\n");
    sleep(1);
  }
	return 0;
}
