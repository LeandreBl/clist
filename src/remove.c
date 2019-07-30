#include "clist.h"

void clist_generic_remove(void *list)
{
	struct clist_node_internal *p = list;

	p->prev->next = p->next;
	p->next->prev = p->prev;
}
