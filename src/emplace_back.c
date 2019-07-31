#include "clist.h"

void clist_generic_emplace_back(void *plist, void *pnode, void (*destructor_p)())
{
	struct clist_node_internal *list = plist;
	struct clist_node_internal *node = pnode;
	void (*destructor)(void *) = destructor_p;

	node->destructor = destructor;
	node->prev = list->prev;
	list->prev = node;
	node->prev->next = node;
	node->next = list;
}
