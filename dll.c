#include <stdio.h>
#include <stdlib.h>

typedef struct dll
{	
	struct dll * next;
	struct dll * prev;
	struct dll * active;
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

int main()
{
	dll * test_dll = dll_init(50);

	printf("%d\n", test_dll->data);	
	printf("%d\n", test_dll->active->data);	
	printf("%d\n", test_dll->next->data);	
	printf("%d\n", test_dll->prev->data);	
	
	return 0;

}
