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

void thread_create(ucontext_t context, void (*func)(void*), void * args);
void thread_yield();
void thread_exit();
void thread_join();
