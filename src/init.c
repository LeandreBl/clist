#include "clist.h"

void clist_generic_init(void *list, void (* destructor_p)())
{
	void (*destructor)(void *) = destructor_p;
	struct clist_node_internal *p = list;

	p->next = p;
	p->prev = p;
	p->destructor = destructor;
}
