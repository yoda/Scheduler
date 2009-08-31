#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "reader.h"


void output(process *proc) {
	//create the output file for writing to	
	FILE *file;
	if ((file = fopen("out.file","a")) == NULL) {
		printf("Failed to create/open the file\n");
		exit(EXIT_FAILURE);
	}
	fprintf(file,"%s ",proc->name);
	void *vp;
	size_t sizetemp;
	int *ip;
	while(!isEmpty(proc->run)) {
		dequeue(proc->run,&vp,&sizetemp);
		ip = vp;
		fprintf(file,"(%d,%d) ",*ip,*(ip+1));
		free(vp);
	}
	fprintf(file,"\n");
	fflush(file);
}

process ** readFile(int *length) {
	char filename[] = "in.file";
	//try to open the file
	FILE * file;
  	file = fopen (filename,"r");
  	if(file==NULL) {
    	printf("File '%s' failed to open, or does not exist!\n",filename);
		exit(EXIT_FAILURE);
  	}	
	
	//allocate memory for the initial pointer array
  	process **proc = malloc(1*sizeof(process*)); //n.b. size of process structure is 72
	
	//read the processes into the next 'process' structure in the 'proc' array. reallocates for the next structure
	*length = 0;
	char tempc[1024]; //buffer to read the string into
	char tempstr[1024];
	int kk = 0;
	
	while(fgets(tempc,sizeof(tempc),file) != NULL) {
		//malloc for current structure
		proc[*length] = malloc(sizeof(process));
		
		//scan data to structure
		if(kk = sscanf(tempc,"%s%d%d",tempstr,&proc[*length]->start,&proc[*length]->duration) != 3) {
			break;
		}else {
			//add the name to the structure. strdup automatically mallocs for the size of the string
			proc[*length]->name = strdup(tempstr);
			
			//reallocate for the next structure to be filled, and adjust length variable
			(*length)++;
			proc = (process**) realloc(proc,(*length+1)*sizeof(process*));
		}
	}
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
/** Sorts the ready queue by process duration. Shortest process next.*/
void sortSPN(QUEUE * ready) {
	int swap_count;	//counter that indicates whether a change was made to the queue in the last pass. Used for termination of sorting loop.
	LINK *link = ready->start->next;
	int duration1;
	int duration2;
	process *proc;
	//sort the processes
	while(true) {
		swap_count = 0;
		link = ready->start->next;
		while(link->next != ready->end) {
			
			//find duration of the current and next processes (within the current and next links)
			duration1 = ((process*)link->payload)->duration;
			duration2 = ((process*)link->next->payload)->duration;
			if(duration2 < duration1) {
				//swap the processes within the links (ie do not change queue structure)
				proc = (process*)link->payload;
				link->payload = link->next->payload;
				link->next->payload = proc;
				swap_count++;
			}
			link = link->next;
		}
		if(swap_count==0) {break;}
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
			   	//add the processing data to the 'run' queue in the current process structure
				int *data = malloc(2*sizeof(int));
				data[0] = time_cycle-counter;
				data[1] = counter;
				enqueue(proc->run,(void*)data,sizeof(data));
			   	output(proc);
			   	//dequeue the completed process structure
			    dequeue(ready, &temp, &sizetemp);
				//free the dequeued structure
				free(temp);			    
				counter = 0;
			}
			
			//if the quantum expires, check for new processes and enqueue them before the current process is (re)enqueued, and reset the counter
			if(counter == quantum) {
				//add the processing data to the 'run' queue in the process structure
				int *data = malloc(2*sizeof(int));
				data[0] = time_cycle-counter;
				data[1] = counter;
				enqueue(proc->run,(void*)data,sizeof(data));
				
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
				//add the processing data to the 'run' queue in the process structure
				int *data = malloc(2*sizeof(int));
				data[0] = time_cycle-counter;
				data[1] = counter;
				enqueue(proc->run,(void*)data,sizeof(data));
				output((process*)temp);
				counter = 0;
				//dequeue and free the finished process
			    dequeue(ready, &temp, &sizetemp);
			  	free(temp);
			   	
			}
		}else{
			time_cycle++;
		}
	}
}

/* takes in a pointer to the process queue completes the processes one by one, according to the queue order*/
void shortestNext(QUEUE *pqueue) {
	//initialise variables
	void *temp;
	size_t sizetemp = 0;
	QUEUE *ready = init("ready");
	int time_cycle = 0;
	int counter = 0;			//records the count (relative to the quantum)
	bool running = true;		//flag that indicates whether a process is accessing the cpu. Dictates whether the ready queue is sorted. It does not need to be sorted every cycle.
	
	//simulate time cycles with a loop and for each cycle, queue the available processes in a new queue (ready queue)
	while(!isEmpty(pqueue) || !isEmpty(ready)) {
		//find the ready processes given the current time cycle
		getReady(pqueue,ready,time_cycle);
		
		//if there are processes ready, sort them by duration and grant access to the first (shortest)
		if(!isEmpty(ready)) {
			//sort the ready queue if need be. Only need to sort when awaiting a change (new process). ie when no process in currently accessing the cpu
			if(running == false)
			{
				sortSPN(ready);
				running = true; 
			}
			
			//peek and modify the duration until the time quantum expires or until the process terminates
			peek(ready, &temp, &sizetemp);
			process *proc = temp;
			proc->duration--;
			counter++;
			time_cycle++;
			printf("Processing %s\n",proc->name);
			
			//if the process terminates, reset the count and dequeue the process
			if(proc->duration == 0) {
				//add the processing data to the 'run' queue in the process structure
				int *data = malloc(2*sizeof(int));
				data[0] = time_cycle-counter;
				data[1] = counter;
				enqueue(proc->run,(void*)data,sizeof(data));
				output((process*)temp);
				counter = 0;
				//dequeue and free the finished process
			    dequeue(ready, &temp, &sizetemp);
			    free(temp);
			  	running = false;
			}
		}else{
			time_cycle++;
		}
	}
}

/* takes in a pointer to the process queue completes the processes one by one, according to the queue order*/
void shortestRemaining(QUEUE *pqueue) {
	//initialise variables
	void *temp;
	size_t sizetemp = 0;
	QUEUE *ready = init("ready");
	int time_cycle = 0;
	int counter = 0;			//records the count (relative to the quantum)
	process *last = NULL;		//pos of the last process to access the cpu, when it changes it triggers a file write operation
	int *data;
	
	//simulate time cycles with a loop and for each cycle, queue the available processes in a new queue (ready queue)
	while(!isEmpty(pqueue) || !isEmpty(ready)) {
		//find the ready processes given the current time cycle
		getReady(pqueue,ready,time_cycle);
		sortSPN(ready);
		
		//if there are processes ready, sort them by duration and grant access to the first (shortest)
		if(!isEmpty(ready)) {
			//peek and modify the duration until the time quantum expires or until the process terminates
			peek(ready, &temp, &sizetemp);
			process *proc = temp;
			
			if(last != NULL && last != proc && last->duration != 0) {
				printf("adding data due to change ctr:%d\n",counter);
				//add the processing data to the 'run' queue in the process structure
				data = malloc(2*sizeof(int));
				data[0] = time_cycle-counter;
				data[1] = counter;
				enqueue(last->run,(void*)data,sizeof(data));
				counter=0;
			}
			
			last=proc;	
			proc->duration--;
			counter++;
			time_cycle++;
			printf("Processing %s\n",proc->name);
			
			//if the process terminates, reset the count and dequeue the process
			if(proc->duration == 0) {
				printf("finished proc adding data\n");
				//add the processing data to the 'run' queue in the process structure
				data = malloc(2*sizeof(int));
				data[0] = time_cycle-counter;
				data[1] = counter;
				enqueue(proc->run,(void*)data,sizeof(data));
				output((process*)temp);
				counter = 0;
				//dequeue and free the finished process
			    dequeue(ready, &temp, &sizetemp);
			    free(temp);
				last=NULL;
			}
		}else{
			time_cycle++;
		}
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
QUEUE * sort(process **proc, int length) {
	int swap_count;
	QUEUE *pqueue = init("pqueue");
	
	//sort the processes
	while(true) {
		swap_count = 0;
		for(int i =0;i<length-1;i++) {
			if(proc[i]->start > proc[i+1]->start) {
				process *temp = proc[i];	
				proc[i] = proc[i+1];
				proc[i+1] = temp;
				swap_count++;
			}
		}
		if(swap_count==0) {break;}
	}
	
	//enqueue the processes whilst initialising their 'run' queues (to store process time stamps)
	for(int i=0;i<length;i++) {
		proc[i]->run = init("run");
		enqueue(pqueue,proc[i],sizeof(proc[i]));
	}
	return pqueue;
}

