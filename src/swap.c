#include "clist.h"

void clist_generic_swap(void *c1, void *c2)
{
	struct clist_node_internal *pc1 = c1;
	struct clist_node_internal *pc2 = c2;
	struct clist_node_internal *prev = pc1->prev;
	struct clist_node_internal *next = pc1->next;

	pc1->prev = pc2->prev;
	pc1->next = pc2->next;
	pc1->prev->next = pc1;
	pc1->next->prev = pc1;
	pc2->prev = prev;
	pc2->next = next;
	pc2->prev->next = pc2;
	pc2->next->prev = pc2;
}
