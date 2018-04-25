#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>

typedef enum direction {NEXT, PREV} direction;

typedef struct dll
{	
	struct dll_node * head;
	int is_consistent;
} dll;

typedef struct dll_node
{
	struct dll_node * next;
	struct dll_node * prev;
  ucontext_t * context;
  int is_done_running;
	int data;

} dll_node;

dll * G_DLL_SCHED;

int dll_is_size_one()
{
	return (G_DLL_SCHED->head == G_DLL_SCHED->head->next);
}

void dll_remove_node(dll_node * node)
{
	// removes the dll_node given by the node pointer
	// from the given dll.

	//////////////////////////////////////////////////////
	// First, if we are about to remove the head,
	// make sure designate a new node as the head.
	if (G_DLL_SCHED->head = node)
	{
		// selection of node->next for the
		// new head is essentially arbitrary.
		G_DLL_SCHED->head = node->next;
	}	
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	// take the node ahead of our given node, and make
	// it point to the node behind our given node.
	node->next->prev = node->prev;
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	// take the node behind our given node, and make
	// it point the the node ahead of our given node.
	node->prev->next = node->next;
	//////////////////////////////////////////////////////
  free(node->context);
	free(node);

	return;
}

void thread_yield()
{
  raise(SIGALRM);
}

void thread_exit()
{
  // assume that when this function is called,
  // the head of the dll has called it.
  G_DLL_SCHED->head->is_done_running = 1;

}

void dll_print(int hops, direction dir)
{
  printf("DLL PRINT\n");
	dll_node * traversal = G_DLL_SCHED->head;
	for(int i = 0; i < hops; i++)
	{
		if (traversal == G_DLL_SCHED->head) // about to print the head
		{
			printf("|%d| ", traversal->data);
		}
		else //not about to print the head
		{
			printf("<%d> ", traversal->data);
		}

		if(dir == NEXT)
		{
			traversal = traversal->next;
		}
		else if (dir == PREV)
		{
			traversal = traversal->prev;
		}
		else
		{
			printf("BAD DIRECTION GIVEN TO DLL PRINTER :(\n");
			return;
		}
	}
	printf("\n");
	return;
}

void sig_handler(int signo)
{
  if (signo != SIGALRM)
  {
    printf("Wow! You really fucked up your signal handling!\n");
    exit(1);
  }

  alarm(3);

  if(dll_is_size_one())
  {
    printf("No threads to schedule!\n");
  }
  else
  {
    if (G_DLL_SCHED->head->is_done_running)
    {
      dll_remove_node(G_DLL_SCHED->head); 
      setcontext(G_DLL_SCHED->head->context);
    }
    else // thread is not done running
         // (i.e. there has been no call to thread_exit)
    {
      G_DLL_SCHED->head = G_DLL_SCHED->head->next;
      swapcontext(G_DLL_SCHED->head->prev->context, G_DLL_SCHED->head->context);
    }
  }
}

dll * dll_init()
{
	// Returns a pointer to a single node linked list.

  // BEGIN NODE CREATION
  printf("MAKING ROOM!\n");
	G_DLL_SCHED = malloc(sizeof(dll));
	G_DLL_SCHED->head = malloc(sizeof(dll_node));
	
	// Link the pointers of the dll back to itself
	// to make it a single node doubly linked circular
	// list.
	G_DLL_SCHED->head->next = G_DLL_SCHED->head;
	G_DLL_SCHED->head->prev = G_DLL_SCHED->head;

  G_DLL_SCHED->head->context = malloc(sizeof(ucontext_t));
  // END NODE CREATION
  
  // BEGIN SIGHANDLE CONFIG
  struct sigaction action;
  action.sa_flags = 0;
  action.sa_handler = sig_handler;
  sigemptyset(&action.sa_mask);
  sigaction(SIGALRM, &action, 0);
  // END SIGHANDLE CONFIG
  alarm(2);

  G_DLL_SCHED->head->is_done_running = 0;
  getcontext(G_DLL_SCHED->head->context);
}

// TODO: ERRORS in retval
void thread_create(ucontext_t context, void (*func)(void *), void * args)
{	
  static int first_run = 1;
  if(first_run)
  {
    first_run = 0;
    // Effectively makes main our first thread object.
    dll_init();
  }

	dll_node * new = malloc(sizeof(dll_node));
	
	// the node directly behind the head is always
	// the tail (since the dll is circular).
	dll_node * old_tail = G_DLL_SCHED->head->prev;

	// ^^ called old tail because we will be changing
	//    the tail shortly...
	
	// Attach a new node behind the head node.
	
	//////////////////////////////////////////////////////
	// First, we make the new node point forward
	// to the head node, and backward to the tail.
	// This effectively makes the new node the new tail!!
	new->next = G_DLL_SCHED->head;
	new->prev = old_tail;
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// We then designate the tail to be the new node
	G_DLL_SCHED->head->prev = new;
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// Check if dll has just been initialized...
	if(dll_is_size_one())
	{
		// ...and start the circuluar queue if it has!!!
		G_DLL_SCHED->head->next = new;
	}
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	// Make the old tail point to the new tail,
	// completing the circle!
	old_tail->next = new;
	//////////////////////////////////////////////////////

  // BEGIN THREAD LOADING
  new->context = malloc(sizeof(ucontext_t));
  *(new->context) = context;
  getcontext(new->context);
  new->context->uc_stack.ss_sp = malloc(100000); // currently LEAKS
  new->context->uc_stack.ss_size = 100000;
  new->is_done_running = 0;
  makecontext(new->context,(void(*)(void))func, 1, args);
  // END THREAD LOADING

	return;
}
