#include "clist.h"

void clist_generic_emplace_after(void *plist, void *pnode, void (* pdestructor)())
{
	struct clist_node_internal *list = plist;
	struct clist_node_internal *node = pnode;
	void (* destructor)(void *) = pdestructor;

	node->destructor = destructor;
	node->prev = list;
	node->next = list->next;
	list->next = node;
	node->next->prev = node;
}
