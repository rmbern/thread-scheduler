#include <stdio.h>
#include <stdlib.h>

typedef struct dll
{	
	struct dll_node * head;
	int is_consistent;
} dll;

typedef struct dll_node
{
	struct dll_node * next;
	struct dll_node * prev;
	int data;

} dll_node;

dll * dll_init(int data)
{
	// Returns a pointer to a single node linked list.
	//
	dll * new_dll = malloc(sizeof(dll));

	new_dll->head = malloc(sizeof(dll_node));
	
	// Link the pointers of the dll back to itself
	// to make it a single node doubly linked circular
	// list.
	new_dll->head->next = new_dll->head;
	new_dll->head->prev = new_dll->head;

	new_dll->head->data = data;

	return new_dll;

}

// TODO: ERRORS in retval
void dll_new_node(dll * list, int data)
{	
	dll_node * new = malloc(sizeof(dll_node));
	new->data = data;
	
	// the node directly behind the head is always
	// the tail (since the dll is circular).
	dll_node * old_tail = list->head->prev;

	// ^^ called old tail because we will be changing
	//    the tail shortly...
	
	// Attach a new node behind the head node.
	
	//////////////////////////////////////////////////////
	// First, we make the new node point forward
	// to the head node, and backward to the tail.
	// This effectively makes the new node the new tail!!
	new->next = list->head;
	new->prev = old_tail;
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// We then designate the tail to be the new node
	list->head->prev = new;
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// Check if dll has just been initialized...
	// (remember we connect the next and last
	//  of a size one dll back to itself!!)
	if(list->head->next == list->head)
	{
		// ...and start the circuluar queue if it has!!!
		list->head->next = new;
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
	dll_node * traversal = test_dll->head;
	while(i --> 0) // i approaches 0
	{
		traversal = traversal->next;
		printf("%d, %d\n", i, traversal->data);	
	}

	i = 40;
	traversal = test_dll->head;
	while(i --> 0) // i approaches 0
	{
		traversal = traversal->prev;
		printf("%d, %d\n", i, traversal->data);	
	}
	
	return 0;

}
