#include "reader.h"
#include "handler.h"
/**	Handles the algorithm selection, depending on which scheduling method was specified via the command line
	@param	inputfile The name of the input file containing the processes
	@param	algorithm The desired schedulaing algorithm
	@param	quantum	The desired time quantum (for round robin scheduling only)
*/
void scheduler(char **inputfile, char **algorithm, int *quantum, int *expire,int *mode) {
	int length;

	process **proc = readFile(*inputfile, &length, mode);

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
		firstCome(pqueue,expire,mode);
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
	
	//prepare and print to the memory output file if the mode suggests to do so
	if(*mode == 1) {
		FILE *vfile;
		//clears the file if it exists, else makes a new one
		if((vfile = fopen("vout.file","w")) == NULL) {
				printf("Failed to create/open the file\n");
				return;
		}
		fclose(vfile);
		//append all memeory data to the file
		printPage(expire);
	}
	
	fclose(file);	
	return;
}
