#include "node_sll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NodeSingle* createNodeSingle(const void *data, unsigned int size)
{
	NodeSingle *new_node = malloc(sizeof(NodeSingle));
	item(new_node) = malloc(size);

	if (item(new_node) == NULL || new_node == NULL)
	{
		printf("Memory error.");
		return NULL;
	}

	next(new_node) = NULL;
	memcpy(item(new_node), data, size);

	return new_node;
}

void insertFirst(NodeSingle **head, const void *data, unsigned int size)
{
	NodeSingle *new_node = createNodeSingle(data, size);

	if (*head == NULL)
	{
		*head = new_node;
		return;
	}

	next(new_node) = *head;
	*head = new_node;
}

void insertLast(NodeSingle **head, const void *data, unsigned int size)
{
	NodeSingle *new_node = createNodeSingle(data, size);

	if (*head == NULL)
	{
		*head = new_node;
		return;
	}

	NodeSingle *temp;
	for (temp = *head; next(temp) != NULL; temp = next(temp));

	next(temp) = new_node;
}

void deleteFirst(NodeSingle **head, void *deleted_data, unsigned int size)
{
	if (*head == NULL) return;
	NodeSingle *temp = *head;
	*head = next(*head);

	memcpy(deleted_data, item(temp), size);

	free(item(temp));
	free(temp);
}

void deleteLast(NodeSingle **head, void *deleted_data, unsigned int size)
{
	if (*head == NULL) return;
	
	if (next(*head) == NULL)
	{
		deleteFirst(head, deleted_data, size);
		return;
	}

	NodeSingle *temp, *temp2;
	for (temp = next(*head); next(next(temp)) != NULL; temp = next(temp));

	temp2 = next(temp);
	next(temp) = NULL;

	memcpy(deleted_data, item(temp2), size);

	free(item(temp2));
	free(temp2);
}

void printSingleList(NodeSingle **head, void (*print_item)(const void *))
{
	NodeSingle *temp;
	for (temp = *head; temp != NULL; temp = next(temp))
	{
		print_item(item(temp));
		printf(" -> ");
	}
	printf("NULL\n");
}

void freeSingleList(NodeSingle **head, unsigned int size)
{
	void *temp_item = malloc(size);

	while (*head != NULL)
		deleteFirst(head, temp_item, size);

	free(temp_item);
}

void sortSingleList(NodeSingle **head, int (*cmp) (const void *a, const void *b))
{
	if (*head == NULL) return;
	if (next(*head) == NULL) return;

	NodeSingle *i, *j, *c;
	i = *head;
	while (next(i) != NULL)
	{
		c = i;
		j = next(i);
		while (j != NULL)
		{
			if (cmp(item(c), item(j)))
				c = j;
			
			j = next(j);
		}

		if (c != i)
		{
			void *temp = item(c);
			item(c) = item(i);
			item(i) = temp;
		}

		i = next(i);
	}
}