#include "reader.h"
#include "handler.h"

void scheduler(char **inputfile, char **algorithm, int *quantum) {
	int length;

	process **proc = readFile(&length);

	// Sort the processes and enqueue.
	QUEUE *pqueue = sort(proc, length);
	
	// Prepare and output file.
	FILE *file;
	if((file = fopen("out.file","w")) == NULL)
	{
		printf("Failed to create/open the file\n");
		return;
	}

	// Algorithm selection
	if(strcmp(*algorithm, "FCFS") == 0) 
	{
		firstCome(pqueue);
	}
	else if(strcmp(*algorithm, "RR") == 0)
	{
		roundRobin(pqueue, *quantum);
	}
	else if(strcmp(*algorithm, "SRT") == 0)
	{
		shortestRemaining(pqueue);
	}
	else if(strcmp(*algorithm, "SPN") == 0)
	{
		shortestNext(pqueue);
	}


	fclose(file);	
	return;
}
