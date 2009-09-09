#include "queue.h"

typedef struct {
          char *name;
          int start;
          int duration;
          int pages;
          struct queue *run;
} process;

extern char *strdup(const char * str);

extern process **readFile(char *filename,int *length, int *mode);

extern void output(process *proc);

extern void getReady(QUEUE *pqueue, QUEUE *ready, int time_cycle);

extern void roundRobin(QUEUE *pqueue, int quantum, int *expire, int *mode);

extern void firstCome(QUEUE *pqueue, int *expire, int *mode);

extern QUEUE* sort(process **proc, int length);

extern void print(process *proc, int length);

extern void printQueue(QUEUE *queue);

extern void sortSPN(QUEUE *ready);

extern void shortestNext(QUEUE *pqueue, int *expire, int *mode);

extern void shortestRemaining(QUEUE *pqueue, int *expire, int *mode);

extern void printPage();
