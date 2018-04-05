#include <stdio.h>
#include <stdlib.h>

typedef struct dll
{	
	struct dll * next;
	struct dll * prev;
	struct dll * active;
	int is_consistent;
	int data;

} dll;

dll * dll_init(int data)
{
	dll * new_dll = malloc(sizeof(dll));
	
	new_dll->next = new_dll;
	new_dll->prev = new_dll;
	// TODO: Check if this returns correctly!!
	new_dll->active = new_dll;
	new_dll->data = data;

	return new_dll;

}

// TODO: ERRORS in retval
void dll_new_node(dll * list, int data)
{	
	dll * new = malloc(sizeof(dll));
	new->data = data;
	
	// The active node can be thought of as a head,
	// at which point the node directly behind it
	// becomes a tail (since the dll is circular).
	dll * old_tail = list->active->prev;

	// ^^ called old tail because we will be changing
	//    the tail shortly...
	
	// Attach a new node behind the active node.
	
	//////////////////////////////////////////////////////
	// First, we make the new node point forward
	// to the active node, and backward to the tail.
	// This effectively makes the new node the new tail!!
	new->next = list->active;
	new->prev = old_tail;
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// We then designate the tail to be the new node
	list->active->prev = new;
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// Check if dll has just been initialized...
	if(list->active->next == list->active)
	{
		// ...and start the circuluar queue if it has!!!
		list->active->next = new;
	}
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	// Make the old tail point to the new tail,
	// completing the circle!
	old_tail->next = new;
	//////////////////////////////////////////////////////

	return;
}

int main()
{
	dll * test_dll = dll_init(0);
	int i = 20;

	while(i --> 0) // i approaches 0
	{	
		dll_new_node(test_dll, i + 70);
	}
	
	i = 40;
	dll * traversal = test_dll;
	while(i --> 0) // i approaches 0
	{
		traversal = traversal->next;
		printf("%d, %d\n", i, traversal->data);	
	}

	i = 40;
	traversal = test_dll;
	while(i --> 0) // i approaches 0
	{
		traversal = traversal->prev;
		printf("%d, %d\n", i, traversal->data);	
	}
	
	return 0;

}
