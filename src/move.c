#include "clist.h"

void clist_generic_move_after(void *pnode, void *pdest)
{
	struct clist_node_internal *dest = pdest;
	struct clist_node_internal *node = pnode;

	clist_generic_remove(node);
	node->next = dest->next;
	node->prev = dest;
	node->next->prev = node;
	node->prev->next = node;
}

void clist_generic_move_before(void *pnode, void *pdest)
{
	struct clist_node_internal *dest = pdest;
	struct clist_node_internal *node = pnode;

	clist_generic_remove(node);
	node->next = dest;
	node->prev = dest->prev;
	node->next->prev = node;
	node->prev->next = node;
}
