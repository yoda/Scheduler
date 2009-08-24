#include "queue.h"

int isEmpty(QUEUE *queue) {
	return(queue->start->next == queue->end);
} 

QUEUE *init(char *queuename) {
	QUEUE *newQueue;	
	LINK *begin;
	LINK *finish;
	
	// Allocate for first link
	begin = (LINK *)malloc(sizeof(LINK));
	
	if(begin == NULL)
	{	
		return NULL;
	} 
	
	// Allocate for end link
	finish = (LINK *)malloc(sizeof(LINK));
	
	if(finish == NULL)
	{
		return NULL;
	}
	
	newQueue = (QUEUE*)malloc(sizeof(QUEUE));
	
	if(newQueue == NULL) 
	{
		return NULL;
	}

	newQueue->name = queuename;
	newQueue->start = begin;
	newQueue->end = finish;
	newQueue->start->next = newQueue->end;
	newQueue->end->successor = newQueue->start;
	newQueue->end->next = NULL;
	newQueue->start->successor = NULL;	

	return newQueue;
}

int peek(QUEUE *queue, void **data, size_t *size) {
	if(isEmpty(queue)){return 1;}
	*data = queue->start->next->payload;
	*size = queue->start->next->size;
	return 0;
}

int dequeue(QUEUE* queue, void** data, size_t *size) {
	if(isEmpty(queue)) {return 1;}
	*data = queue->start->next->payload;
	*size = queue->start->next->size;
	LINK *temp = queue->start->next;
	queue->start->next = queue->start->next->next;
	
	//special case - when the last item is removed
	if(isEmpty(queue)) {
		queue->end->successor = queue->start;
	}
	
	free(temp);
	return 0;
}

int enqueue(QUEUE* queue, void* data, size_t size) {
	LINK* newLink;
	newLink = (LINK *) malloc(sizeof (LINK));
	if(newLink == NULL)
	{
		return 1;
	}
	newLink->size = size;
	newLink->payload = data;
	
	queue->end->successor->next = newLink;
	newLink->next = queue->end;
	queue->end->successor = newLink;
	return 0;
}

#ifdef debug
int main(int argc, char **argv) {

	const char test1[] = "test1";
	const char test2[] = "test2";
	const char test3[] = "test3";

	QUEUE *queue = init("TestQueue");
	
	
	int a = enqueue(queue, (void*)test1, strlen(test1) * sizeof(char));
	printf("Enqueued: %s - with result %d\n", test1, a);
	int b = enqueue(queue, (void*)test2, strlen(test2) * sizeof(char));
	printf("Enqueued: %s - with result %d\n", test2, b);
	int c = enqueue(queue, (void*)test3, strlen(test3) * sizeof(char));
	printf("Enqueued: %s - with result %d\n", test3, c);

	printf("Attempting to dequeue, expecting %s\n", test1);
	void *temp;
	size_t sizetemp = 0;
	
	int d = dequeue(queue, &temp, &sizetemp);
	printf("Got %s\n", (char*)temp);
	printf("%d\n",isEmpty(queue));
	printf("start: %p first pay: %p\n\n",&queue->start,&queue->start->next->payload);
	
	
	int e = dequeue(queue, &temp, &sizetemp);
	printf("Got %s\n", (char*)temp);
	printf("%d\n",isEmpty(queue));
	printf("start: %p first pay: %p\n\n",&queue->start,&queue->start->next->payload);
	
			
	int f = dequeue(queue, &temp, &sizetemp);
	printf("Got %s\n", (char*)temp);
	printf("%d\n",isEmpty(queue));
	printf("start: %p first pay: %p\n\n",&queue->start,&queue->start->next->payload);
	
		
	int h = enqueue(queue, (void*)test2, strlen(test2) * sizeof(char));
	printf("Enqueued: %s - with result %d\n", test2, c);	
	printf("%d\n",isEmpty(queue));
	printf("start: %p first pay: %p\n\n",queue->start,queue->start->next);
	
	
	int i = dequeue(queue, &temp, &sizetemp);
	printf("Got %s\n", (char*)temp);
	printf("%d\n",isEmpty(queue));

	return 0;
}
#endif
