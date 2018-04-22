#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <ucontext.h>
#include "dll.h"

dll * G_DLL_SCHED;

void sig_handler(int signo)
{
  if (signo != SIGALRM)
  {
    printf("Wow! You really fucked up your signal handling!\n");
    exit(1);
  }

  alarm(2);

  if(dll_is_size_one(G_DLL_SCHED))
  {
    printf("No threads to schedule!\n");
  }
  else
  {
    G_DLL_SCHED->head = G_DLL_SCHED->head->next;
    swapcontext(G_DLL_SCHED->head->prev->context, G_DLL_SCHED->head->context);
  }
}

void new_thread()
{
  while(1)
  {
    printf("I AM A NEW THREAD MOTHERFUCKER!\n");
    sleep(1);
  }
}

void newer_thread()
{
  while(1)
  {
    printf("I AM A NEWER THREAD MOTHERFUCKER!\n");
    sleep(1);
  }
}

int main()
{
  G_DLL_SCHED = dll_init(0);
  
  struct sigaction action;
  action.sa_flags = 0;
  action.sa_handler = sig_handler;
  sigemptyset(&action.sa_mask);
  sigaction(SIGALRM, &action, 0);
  alarm(2);

  ucontext_t thread;
  ucontext_t newerthread;
  dll_add_thread(G_DLL_SCHED, thread, new_thread, 1);
  dll_add_thread(G_DLL_SCHED, newerthread, newer_thread, 2);
  while(1)
  {
    printf("I AM THE MAIN THREAD MOTHERFUCKER!\n");
    sleep(1);
  }
	return 0;
}
