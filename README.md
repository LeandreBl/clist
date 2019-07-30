# clist
C linked list &lt;template>

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <clist.h>

/* Free the allocated entry */
static void free_str(char **pstr)
{
	free(*pstr);
}

/* The function takes a pointer to the object (int * for int, char ** for char * ...)
** as first parameter, and any parameter you want after it
*/
static int new_timed_str(char **pdest, const char *input)
{
	time_t t = time(NULL);
	size_t len;

	/* man 3 asprintf (it's an useful function, give it a try !) */
	if (asprintf(pdest, "[%ld] %s", t, input) == -1) {
		/* In case of error, return a negative value.
		** By doing so, the node will not be pushed
		** into the list
		*/
		return -1;
	}

	/* remove the \n at the end of the line */
	len = strlen(*pdest);
	(*pdest)[len - 1] = '\0';
	return 0;
}

/* A simple program that stores each user given line with it's associated time */
int main(void)
{
	/* Declare an empty list */
	clist(char *) *list = NULL;
	char *input;
	size_t n;
	ssize_t rd;

	do {
		/* Set `input` and `n` to zero, so that getline will allocate the buffer itself */
		input = NULL;
		n = 0;

		/* Read a line, stores the allocated pointer in the `input` variable */
		rd = getline(&input, &n, stdin);

		/* In case of a ctrl + D or an error */
		if (rd <= 0) {
			free(input);
			break;
		}

		/* Emplace a new node in the list
		** List as first argument
		** The cleanup function associated with it's payload (here a malloc char *)
		** The function to call on the newly created object
		** Every argument the constructor needs
		*/
		clist_emplace_back(list, free_str, new_timed_str, input);

		/* Since we copied the string with asprintf, we can free the input */
		free(input);
	} while (input != NULL);

	/* Push in front of the list, a read-only string, and set it's
	** cleanup function to NULL, there is no need to free it
	*/
	clist_push_front(list, "Here is the history:", NULL);

	/* Display each entry of the list */
	clist_foreach(line, list) {
		printf("%s\n", *line);
	}

	/* Destroy all the nodes with their associated payloads */
	clist_destroy(list);
  return (0);
}
```
