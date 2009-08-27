#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


typedef struct link {
	void *payload;
	size_t size;
	struct link *next;
	struct link *successor;
} LINK;

typedef struct queue{
	LINK *start;
	LINK *end;
	char *name;
} QUEUE;

extern int isEmpty(QUEUE *);

extern QUEUE *init(char *);

extern int peek(QUEUE *, void **, size_t *);

extern int dequeue(QUEUE* , void** , size_t *);

extern int enqueue(QUEUE* , void* , size_t );

