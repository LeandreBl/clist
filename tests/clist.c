#include <criterion/criterion.h>

#include "clist.h"

Test(clist, foreach)
{
  clist(int) first;
	clist(int) second;

	clist_init(&first);
	first.object = 12;
	first.next = &second;
	first.prev = &second;
	second.next = &first;
	second.prev = &first;
	second.object = 42;

  size_t i = 0;
	clist_foreach(p, &first)
	{
    if (i == 0)
      cr_assert(*p == 12);
    if (i == 1)
      cr_assert(*p == 42);
    ++i;
	}
  cr_assert(i == 2);
}