#include "queue.h"

typedef struct {
          char *name;
          int start;
          int duration;
          struct queue *run;
} process;

extern char *strdup(const char * str);

extern process **readFile(int *length);

extern void output(process *proc);

extern void getReady(QUEUE *pqueue, QUEUE *ready, int time_cycle);

extern void roundRobin(QUEUE *pqueue, int quantum);

extern void firstCome(QUEUE *pqueue);

extern QUEUE* sort(process **proc, int length);

extern void print(process *proc, int length);

extern void printQueue(QUEUE *queue);

