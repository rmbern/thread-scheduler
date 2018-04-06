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

dll * dll_init(int data);
int dll_is_size_one(dll * list);
void dll_add_node(dll * list, int data);
void dll_remove_node(dll * list, dll_node * node);
void dll_print(dll * list, int hops, direction dir);
