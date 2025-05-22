#ifndef linkedlist_h
#define linkedlist_h

typedef struct nodesingle
{
	void *item;
	struct nodesingle *next;
} NodeSingle;

#define item(n) (n)->item
#define next(n) (n)->next

#define size(n) (n)->size

NodeSingle* createNodeSingle(const void *data, unsigned int size);

void insertFirst(NodeSingle **head, const void *data, unsigned int size);
void insertLast(NodeSingle **head, const void *data, unsigned int size);

void deleteFirst(NodeSingle **head, void *deleted_data, unsigned int size);
void deleteLast(NodeSingle **head, void *deleted_data, unsigned int size);

void printSingleList(NodeSingle **head, void (*printItem)(const void* item));
void freeSingleList(NodeSingle **head, unsigned int size);
void sortSingleList(NodeSingle **head, int (*cmp) (const void *a, const void *b));

#endif