#include "clist.h"

void clist_generic_emplace_before(void *plist, void *pnode, void (* pdestructor)())
{
	struct clist_node_internal *list = plist;
	struct clist_node_internal *node = pnode;
	void (* destructor)(void *) = pdestructor;

	node->destructor = destructor;
	node->prev = list->prev;
	node->next = list;
	node->prev->next = node;
	list->prev = node;
}
