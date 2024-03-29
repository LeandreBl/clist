#include "clist.h"

void clist_generic_erase(void *list)
{
	struct clist_node_internal *p = list;

	clist_generic_remove(p);
	if (p->destructor) {
		p->destructor((char *)p + offsetof(clist(int), object));
	}
}
