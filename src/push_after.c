#include "clist.h"

void clist_generic_push_after(void *plist, void *pnode)
{
	struct clist_node_internal *list = plist;
	struct clist_node_internal *node = pnode;

	node->prev = list;
	node->next = list->next;
	node->next->prev = node;
	list->next = node;
}
