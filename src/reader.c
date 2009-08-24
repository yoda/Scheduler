#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"
#include <string.h>
#include <stdbool.h>

process* readFile(int * length) {

	char filename[] = "test.txt";
	//try to open the file
	FILE * file;
  	file = fopen (filename,"r");
  	if(file==NULL) {
    	printf("File '%s' failed to open, or does not exist!\n",filename);
		exit(EXIT_FAILURE);
  	}
								
	//allocate memory for the initial structure
  	process *proc = malloc(1*sizeof(process)); //n.b. size of process structure is 72
	
	//read the processes into the next 'process' structure in the 'proc' array. reallocates for the next structure
	*length = 0;
	while(true) {
		if(fscanf(file, "%s%d%d", proc[*length].name,&proc[*length].start,&proc[*length].duration) == EOF) {
			break;
		}else {
			//reallocate for the next structure to be filled, and adjust length variable
			(*length)++;
			proc = (process*) realloc(proc,(*length+1)*sizeof(process));
		}
	}
	
	//close the file
  	fclose(file);
	return proc;
}

void printQueue(QUEUE *queue) {
	void *temp;
	size_t sizetemp = 0;
	LINK *link = queue->start->next;
	while(link != queue->end) {
		process *proc = link->payload;
		printf("Q: %s %d %d\n",proc->name,proc->start,proc->duration);
		link = link->next;
	}
}

void getReady(QUEUE *pqueue, QUEUE *ready, int time_cycle) {
	void *temp;
	size_t sizetemp = 0;
	while(!isEmpty(pqueue)) {
		//find the ready processes given the current time cycle
		peek(pqueue, &temp, &sizetemp);
		process *proc = temp;
		if(proc->start==time_cycle) {
			//dequeue from the process queue and enqueue in the ready queue
			dequeue(pqueue, &temp, &sizetemp);
			process *proc = temp;
			enqueue(ready,proc,sizeof(process));
		}else {
			break;
		}
	}
}

void output() {
	//create the output file for writing to	
	FILE *file;
	if ((file = fopen("out.file","w")) == NULL) {
		printf("Failed to create/open the file\n");
		exit(EXIT_FAILURE);
	}

}

void roundRobin(QUEUE *pqueue,int quantum) {

	//initialise variables
	void *temp;
	size_t sizetemp = 0;
	QUEUE *ready = init("ready");
	int time_cycle = 0;
	int counter = 0;	//records the count (relative to the quantum)
	
	//simulate time cycles with a loop and for each cycle, queue the available processes in a new queue (ready queue)
	while(!isEmpty(pqueue) || !isEmpty(ready)) {
		//find the ready processes given the current time cycle
		getReady(pqueue,ready,time_cycle);
		
		//if there are processes ready, grant access to the first
		if(!isEmpty(ready)) {
			//peek and modify the duration until the time quantum expires or until the process terminates
			peek(ready, &temp, &sizetemp);
			process *proc = temp;
			proc->duration--;
			counter++;
			time_cycle++;
			printf("Processing %s\n",proc->name);
			
			//if the process terminates, reset the count and dequeue the process
			if(proc->duration == 0) {
				counter = 0;
			    dequeue(ready, &temp, &sizetemp);
			}
			
			//if the quantum expires, check for new processes and enqueue them before the current process is (re)enqueued, and reset the counter
			if(counter == quantum) {
				counter=0;
				getReady(pqueue,ready,time_cycle);
				dequeue(ready,&temp,&sizetemp);
				enqueue(ready,(process*)temp,sizetemp);
			}
		}else{
			time_cycle++;
		}
	}
}

/* takes in a pointer to the process queue completes the processes one by one, according to the queue order*/
void firstCome(QUEUE *pqueue) {
	//create the output file for writing to	
	FILE *file;
	if ((file = fopen("out.file","w")) == NULL) {
		printf("Failed to create/open the file\n");
		exit(EXIT_FAILURE);
	}
	//handle the pqueue & write the processing times to the out file
	void *temp;
	size_t sizetemp = 0;
	int time = 2; //need to peek here to find the start time of the top process
	while(dequeue(pqueue, &temp, &sizetemp) == 0) {
		//print the current job. start the next job at the start+duration of the previous job
		process *proc = temp;
		fprintf(file,"%s %d %d\n",proc->name,time,proc->duration);
		time = time + proc->duration;
	}
}

//printing method (DEBUG)
void print(process *proc,int length) {
	int cur=0;
  	while(cur < length) {
  		printf("read '%s %d %d'\n",proc[cur].name,proc[cur].start,proc[cur].duration);
  		cur++;
  	}
}	

//takes in the process array and produces the sorted array of processes
QUEUE * sort(process *proc, int length) {
	int swap_count;
	QUEUE *pqueue = init("pqueue");
	
	//sort the processes
	while(true) {
		swap_count = 0;
		for(int i =0;i<length-1;i++) {
			if(proc[i].start > proc[i+1].start) {
				process temp = proc[i];	
				proc[i] = proc[i+1];
				proc[i+1] = temp;
				swap_count++;
			}
		}
		if(swap_count==0) {break;}
	}
	
	//enqueue the processes
	for(int i=0;i<length;i++) {
		enqueue(pqueue,&proc[i],sizeof(proc[i]));
	}
	return pqueue;
}

//reads the input file and sort the processes into an array based on their start time (first at top last at bottom) 
int main(int argc, char **argv) {
	
	int length;
	process* proc;
	proc = readFile(&length);	
	
	//sort the data
	printf("\nOriginal Process List...\n\n");
	print(proc,length);
	printf("\nSorting...\n\n");
  	QUEUE *pqueue = sort(proc,length);
	print(proc,length);
	printf("\n\n\n");
	//firstCome(pqueue);
	roundRobin(pqueue,4);
  	return 0;
	
}
