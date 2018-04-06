#include <stdio.h>
#include <stdlib.h>

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
	int data;

} dll_node;

int dll_is_size_one(dll * list)
{
	return (list->head == list->head->next);
}

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
void dll_add_node(dll * list, int data)
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
	if(dll_is_size_one(list))
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

void dll_remove_node(dll * list, dll_node * node)
{
	// removes the dll_node given by the node pointer
	// from the given dll.

	//////////////////////////////////////////////////////
	// First, if we are about to remove the head,
	// make sure designate a new node as the head.
	if (list->head = node)
	{
		// selection of node->next for the
		// new head is essentially arbitrary.
		list->head = node->next;
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

	free(node);

	return;
}

void dll_print(dll * list, int hops, direction dir)
{
	dll_node * traversal = list->head;
	for(int i = 0; i < hops; i++)
	{
		if (traversal == list->head) // about to print the head
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
