#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "reader.h"

int paget[200][2];		//the page table array
char * page[200];		//the virtual memory array
char *mem_out;			//virtual memory print string
char *page_out;			//page table print string

int getName(process *proc) {
	char *name = malloc(2*sizeof(char)+1);
	int pid;
	if(strlen(proc->name) == 4) {
		name[0] = proc->name[3];
		name[1] = '\0';		
	}else if (strlen(proc->name) == 5) {
		name[0] = proc->name[3];
		name[1] = proc->name[4];
		name[2] = '\0';	
	}else{
		printf("ERROR: Job Name '%s' Is Not In Valid Numerical Range (0 - 99)\n",proc->name);
		exit(EXIT_FAILURE);
	}
	pid = atoi(name);
	return pid;
}

/**	Generates the 4 digit, double concatenated process name (integer), to be stored in the page table and virtual memory.
	@param proc The process with which to generate the name
	@return The 4 digit integer representing the process id
*/
char * getNameQuad(process *proc) {
	char *str = malloc(4*sizeof(char) +1);
	int length = strlen(proc->name);

	if(length == 4) {
		str[0] = '0';
		str[1] = proc->name[3];
		str[2] = '0';
		str[3] = proc->name[3];
		str[4] = '\0';
	}else if(length == 5) {
		str[0] = proc->name[3];
		str[1] = proc->name[4];
		str[2] = proc->name[3];
		str[3] = proc->name[4];
		str[4] = '\0';
	}
	return str;
} 

void clonePage(int *expire) {
	//malloc for the first part of the page table header
	page_out = malloc(20*sizeof(char)+1);
	strcat(page_out,"<-- Page Table At t=");

	//malloc for the first part of the virtual memory header
	mem_out = malloc(24*sizeof(char)+1);
	strcat(mem_out,"<-- Virtual Memory At t=");
	
	//get the expire time as a string
	char buffer[4*sizeof(int)+1];
	sprintf(buffer,"%d",*expire);
	
	//realloc for time and end characters. +7 for the string ' -->\n\n' and the nullbyte
	mem_out = (char*) realloc(mem_out, (strlen(mem_out)*sizeof(char)) + (strlen(buffer)*sizeof(char)) + 7);
	strcat(mem_out,buffer);
	strcat(mem_out," -->\n\n");

	//+7 for the string ' -->\n\n' and the nullbyte
	page_out = (char*) realloc(page_out, (strlen(page_out)*sizeof(char)) + (strlen(buffer)*sizeof(char)) + 7);
	strcat(page_out,buffer);
	strcat(page_out," -->\n\n");
	
	
	char i_buffer[4*sizeof(int)+1];
	
	for(int i=0;i<200;i++) {
		if(i%4 == 0) {
			//convert the name and position into strings
			sprintf(buffer,"%d\n",paget[i][0]);
			sprintf(i_buffer,"%d",(i/4) +1);
			
			//append to the page table string. +11 is for the strings "Frame " & " -- " and the nullbyte
			page_out = (char*) realloc(page_out, (strlen(page_out)*sizeof(char)) + (strlen(i_buffer)*sizeof(char)) +11);
			strcat(page_out,"Frame ");
			strcat(page_out,i_buffer);
			strcat(page_out," -- ");
			
			//if the page is used, print the job. +5 for the string "Job " plus the nullbyte
			if(strcmp(buffer,"-1\n") != 0) {
				page_out = (char*) realloc(page_out, (strlen(page_out)*sizeof(char)) + (strlen(buffer)*sizeof(char)) +5);
				strcat(page_out,"Job ");
				strcat(page_out,buffer);
			}else{
				//else page is empty, so print empty. +7 for the string "Empty\n" and the nullbyte
				page_out = (char*) realloc(page_out, (strlen(page_out)*sizeof(char)) + 7);
				strcat(page_out,"Empty\n");
			}
		}
		
		//fill memory string. +2 for the newline and the nullbyte
		mem_out = (char*) realloc(mem_out, (strlen(mem_out)*sizeof(char)) + (strlen(page[i])*sizeof(char)) + 2);
		strcat(mem_out,page[i]);
		strcat(mem_out,"\n");
	}
}

/** Prints the page table and the page contents
*/
void printPage() {
	//create the output file for writing to	
	FILE *file;
	//try to open the file
	if ((file = fopen("vout.file","w")) == NULL) {
		printf("Failed to create/open the file\n");
		exit(EXIT_FAILURE);
	}
	//write the memory output string to the file
	fprintf(file,"%s\n\n",page_out);
	fprintf(file,"%s",mem_out);
}
	
/** Initialises an empty virtual memory block
*/
void initMemory() {
	//set all pages in memory to zero, indicating 'free' status. also set "last-used" time to zero and adjust the page table accordingly.
	for(int i=0;i<200;i++) {
		page[i] = malloc(4*sizeof(char) +1);
		page[i][0] = '0';
		page[i][1] = '\0';
		
		paget[i][0] = -1;
		paget[i][1] = -1;
	}
}

/** Returns the number of free/unused frames within virtual memory, by scanning the page table
	@return The number of free/unused frames
*/
int findFree() {
	int count = 0;
	for(int i=0;i<200;i++) {
		if(paget[i][0] == -1) {
			count++;
		}
	}
	return count/4;	
}

/** Checks whether the indicated process is currently in memory by scanning the page table.
	@param pid The name of the process
	@return true if the process is found in memory, false otherwise
*/
bool isCached(int pid) {
	for(int i=0;i<200;i++) {
		if(paget[i][0] == pid) {
			return true;
		}
	}
	return false;
}

/**	Frees the pages of the memory frame found at the desired index
	@param index The index representing the start of the frame in the virtual memory.
*/
void freeFrame(int index) {
	for(int i=index;i<(index+4);i++) {
		//free the memory frame
		page[i][0] = '0';
		page[i][1] = '\0';
		//debug
		printf("Freed page [%d] that was used by process %d\n",i,paget[i][0]);
		//adjust the page table
		paget[i][0] = -1;
		paget[i][1] = -1;
	}
}

/** Scans the page table for age of the first process listed
*/
int getFirstAge() {
	int index = 0;
	while(true) {
		if(paget[index][1] != -1) {
			return paget[index][1];
		}
		index++;
	}
}

/**	Frees the desired number of memory frames, oldest first
	@param num The number of frames to be cleared
*/
void freeFrames(int num) {
	int oldest = getFirstAge();				//initialise the age of the oldest process (youngest time_cycle value within the page table)
	char *old_proc = malloc(4*sizeof(char)+1);
	char buffer[4*sizeof(int)+1];
	sprintf(old_proc,"%d",paget[0][0]);		//initialise the name of the oldest process
				
	while(num>0) {
		//find oldest process
		for(int i=0;i<200;i++) {
			if(paget[i][1] != -1 && paget[i][1] < oldest) {
			printf("%d\n",paget[i][1]);
			 	oldest = paget[i][1];
				sprintf(old_proc,"%d",paget[i][0]);		//initialise the name of the oldest process
			}
		}

		//free all frames relating to the oldest process, and adjust the page table accordingly
		for(int i=0;i<200;i++) {
			sprintf(buffer,"%d",paget[i][0]);
			if(strcmp(buffer,old_proc) == 0) {
				//free entire frame and adjust page table (ensures that the '1' value fragments get reset)
				freeFrame(i);
				//decrement frame counter
				num--;
				//jump to start of next frame (i+3 due to loop having i+1, thus finally i+4)
				i=i+3;
			}
		}
		//end case
		if(num <= 0) {
			break;
		}
		//reset for start of next loop
		int oldest = getFirstAge();
		sprintf(old_proc,"%d",paget[0][0]);
	}
}

void fillFrames(process *proc,int pid,int time_cycle) {
	//scan through memory, assigning the process to the first free frame, until the desired number is met
	//any fragmentation in the last frame is distinguished by a value of 1. n.b. fragmentation = unused pages within the frame.
	int pages_req = proc->pages;
	int frag_val = 4 - (proc->pages%4);	//the number of pages that will be unused in the last frame
	if(frag_val == 4) frag_val = 0;
	char *pquad = getNameQuad(proc);
	while(true) {
		for(int i=0;i<200;i++) {
			//fill the frame accordingly
			if(strcmp(page[i],"0") == 0) {
				//set page value
				strcpy(page[i],pquad);

				//map to page table
				paget[i][0] = pid;
				paget[i][1] = time_cycle;
				printf("Filled page[%d] with %s at time %d\n",i,page[i],time_cycle);
				pages_req--;
				//handle the spare pages within the last frame
				if(pages_req == 0) {
					//fill remaining pages in the current frame with value '1'. ie fragmented
					for(frag_val;frag_val>0;frag_val--) {
						//set page value
						page[i+frag_val][0] = '-';
						page[i+frag_val][1] = '1';
						page[i+frag_val][2] = '0';
						page[i+frag_val][3] = '\0';
						//map to page table
						paget[i+frag_val][0] = -10;
						paget[i+frag_val][1] = time_cycle;
						printf("Filled page[%d] with '%s' at time %d\n",i+frag_val,page[i+frag_val],time_cycle);
					}
					break;
				}
			}
		}
		break;
	}
}

/** Writes the process's cpu access history to the output file
	@param proc The process that has just completed/terminated
*/
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

/**	Reads the contents of the input file into an unsorted array of process structures
	@param filename The string representing the name of the input file
	@param length A counter that records the number of jobs present in the input file
	@return The array containing the process structures
*/
process **readFile(char * filename,int *length,int *mode) {
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
	
	while(fgets(tempc,sizeof(tempc),file) != NULL) {
		//malloc for current structure
		proc[*length] = malloc(sizeof(process));
		if(*mode == -1) {
			
			//scan data to structure
			if(sscanf(tempc,"%s%d%d",tempstr,&proc[*length]->start,&proc[*length]->duration) != 3) {
				break;
			}else {
				//add the name to the structure. strdup automatically mallocs for the size of the string
				proc[*length]->name = strdup(tempstr);
			
				//reallocate for the next structure to be filled, and adjust length variable
				(*length)++;
				proc = (process**) realloc(proc,(*length+1)*sizeof(process*));
			}
		}else if(*mode == 1) {
			//set up the virtual memory
			initMemory();
			//scan data to structure
			if(sscanf(tempc,"%s%d%d%d",tempstr,&proc[*length]->start,&proc[*length]->duration,&proc[*length]->pages) != 4) {
				break;
			}else {
				//if too much memory is required
				if(proc[*length]->pages > 200 || proc[*length]->pages < 1) {
					printf("Invalid Page Size For %s, Must Be Within The Range (1-200)\n",tempstr); 
					exit(EXIT_FAILURE); 
				}
				//add the name to the structure. strdup automatically mallocs for the size of the string
				proc[*length]->name = strdup(tempstr);
			
				//reallocate for the next structure to be filled, and adjust length variable
				(*length)++;
				proc = (process**) realloc(proc,(*length+1)*sizeof(process*));
			}
		}
	}
  	fclose(file);
  	return proc;
}

/**	Debugging method that prints the contents of a given queue 
	@param queue The queue to be printed
*/
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

/**	Sorts the ready queue by process duration, shortest process first.
	@param ready The queue of ready processes 
*/
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

/**	Appends all available processes at the given time to the ready queue.
	@param pqueue The queue of all available processes
	@param ready The queue of all ready processes
	@param time_cycle The current time cycle of the processor
*/
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

/**	Simulates the Round Robin scheduling algorithm for the given time quantum.
	@param pqueue The queue of all available processes
	@param quantum The time quantum
*/
void roundRobin(QUEUE *pqueue,int quantum,int *expire, int *mode) {
	//initialise variables
	void *temp;
	size_t sizetemp = 0;
	QUEUE *ready = init("ready");
	int time_cycle = 0;
	int counter = 0;	//records the count (relative to the quantum)
	int *data;
	int frames;
	int pid;
	
	//simulate time cycles with a loop and for each cycle, queue the available processes in a new queue (ready queue)
	while(!isEmpty(pqueue) || !isEmpty(ready)) {
		//find the ready processes given the current time cycle
		getReady(pqueue,ready,time_cycle);
		
		//if there are processes ready, grant access to the first
		if(!isEmpty(ready)) {
			//peek and modify the duration until the time quantum expires or until the process terminates
			peek(ready, &temp, &sizetemp);
			process *proc = temp;
			
			//for memory			
			if(*mode == 1) {
				frames = proc->pages/4;
				int rem = proc->pages%4;
				if(rem>0) frames = frames+1;
				int numfree = findFree();
				pid = getName(proc);
				printf("Need %d frames, have %d frames. Free %d frames\n",frames,numfree,frames-numfree);
				if(!isCached(pid)) {
					if(frames > numfree) {freeFrames(frames-numfree); }
					fillFrames(proc,pid,time_cycle);
				}
				//backup the memory at the specified time
				if(time_cycle+1 == *expire) {
					clonePage(expire);
				}
			}
			
			proc->duration--;
			counter++;
			time_cycle++;
			printf("Processing %s\n",proc->name);
			
			//if the process terminates, reset the count and dequeue the process
			if(proc->duration == 0) {
				printf("Completed  %s at t=%d\n",proc->name,time_cycle); 
			   	//add the processing data to the 'run' queue in the current process structure
				data = malloc(2*sizeof(int));
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
				data = malloc(2*sizeof(int));
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
	//if the expire time is larger than the required completion time, let the output string represent the final memory state
	if(*mode == 1 && *expire > time_cycle) {
		clonePage(expire);
	}
}

/**	Simulates the First-Come First-Serve scheduling algorithm.
	@param pqueue The queue of all available processes
*/
void firstCome(QUEUE *pqueue, int *expire, int *mode) {
	//initialise variables
	void *temp;
	size_t sizetemp = 0;
	QUEUE *ready = init("ready");
	int time_cycle = 0;
	int counter = 0;	//records the count (relative to the quantum)
	int *data;
	int frames;
	int pid;
	
	//simulate time cycles with a loop and for each cycle, queue the available processes in a new queue (ready queue)
	while(!isEmpty(pqueue) || !isEmpty(ready)) {
		//find the ready processes given the current time cycle
		getReady(pqueue,ready,time_cycle);
		
		//if there are processes ready, grant access to the first
		if(!isEmpty(ready)) {
			//peek and modify the duration until the time quantum expires or until the process terminates
			peek(ready, &temp, &sizetemp);
			process *proc = temp;

			//for memory			
			if(*mode == 1) {
				frames = proc->pages/4;
				int rem = proc->pages%4;
				if(rem>0) frames = frames+1;
				int numfree = findFree();
				pid = getName(proc);
				
				if(!isCached(pid)) {
					if(frames > numfree) {freeFrames(frames-numfree); }
					fillFrames(proc,pid,time_cycle);
				}
				//backup the memory at the specified time
				if(time_cycle+1 == *expire) {
					clonePage(expire);
				}
			}
			
			proc->duration--;
			counter++;
			time_cycle++;
			printf("Processing %s\n\n",proc->name);
			
			//if the process terminates, reset the count and dequeue the process
			if(proc->duration == 0) {
				printf("Completed  %s at t=%d\n",proc->name,time_cycle); 
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
			}
		}else{
			time_cycle++;
		}
	}
	//if the expire time is larger than the required completion time, let the output string represent the final memory state
	if(*mode == 1 && *expire > time_cycle) {
		clonePage(expire);
	}
}

/**	Simulates the Shortest Process Next scheduling algorithm.
	@param pqueue The queue of all available processes
*/
void shortestNext(QUEUE *pqueue,int *expire, int *mode) {
	//initialise variables
	void *temp;
	size_t sizetemp = 0;
	QUEUE *ready = init("ready");
	int time_cycle = 0;
	int counter = 0;			//records the count (relative to the quantum)
	bool running = true;		//flag that indicates whether a process is accessing the cpu. Dictates whether the ready queue is sorted. 
	int *data;
	int frames;
	int pid;
	
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
			
			//for memory			
			if(*mode == 1) {
				frames = proc->pages/4;
				int rem = proc->pages%4;
				if(rem>0) frames = frames+1;
				int numfree = findFree();
				pid = getName(proc);
				
				if(!isCached(pid)) {
					if(frames > numfree) {freeFrames(frames-numfree); }
					fillFrames(proc,pid,time_cycle);
				}
				//backup the memory at the specified time
				if(time_cycle+1 == *expire) {
					clonePage(expire);
				}
			}
			
			proc->duration--;
			counter++;
			time_cycle++;
			printf("Processing %s\n",proc->name);
			
			//if the process terminates, reset the count and dequeue the process
			if(proc->duration == 0) {
				printf("Completed  %s at t=%d\n",proc->name,time_cycle); 
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
			  	running = false;
			}
		}else{
			time_cycle++;
		}
	}
	//if the expire time is larger than the required completion time, let the output string represent the final memory state
	if(*mode == 1 && *expire > time_cycle) {
		clonePage(expire);
	}
}

/**	Simulates the Shortest Remaining Time scheduling algorithm.
	@param pqueue The queue of all available processes
*/
void shortestRemaining(QUEUE *pqueue,int *expire, int *mode) {
	//initialise variables
	void *temp;
	size_t sizetemp = 0;
	QUEUE *ready = init("ready");
	int time_cycle = 0;
	int counter = 0;			//records the count (relative to the quantum)
	process *last = NULL;		//pos of the last process to access the cpu, when it changes it triggers a file write operation
	int *data;
	int frames;
	int pid;
	
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
			
			//for memory			
			if(*mode == 1) {
				frames = proc->pages/4;
				int rem = proc->pages%4;
				if(rem>0) frames = frames+1;
				int numfree = findFree();
				pid = getName(proc);
				
				if(!isCached(pid)) {
					if(frames > numfree) {freeFrames(frames-numfree); }
					fillFrames(proc,pid,time_cycle);
				}
				//backup the memory at the specified time
				if(time_cycle+1 == *expire) {
					clonePage(expire);
				}
			}
			
			//if the process using the cpu changes
			if(last != NULL && last != proc && last->duration != 0) {
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
				printf("Completed  %s at t=%d\n",proc->name,time_cycle); 
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
			    
			    //reset the last process logger
				last=NULL;
			}
		}else{
			time_cycle++;
		}
	}
	//if the expire time is larger than the required completion time, let the output string represent the final memory state
	if(*mode == 1 && *expire > time_cycle) {
		clonePage(expire);
	}
}

/**	Debug method for printing the unsorted array of processes, taken fro the input file.
	@param proc The array of processes read from the input file
	@param length The number of processes
*/
void print(process *proc,int length) {
	int cur=0;
  	while(cur < length) {
  		printf("read '%s %d %d'\n",proc[cur].name,proc[cur].start,proc[cur].duration);
  		cur++;
  	}
}	

/**	Sorts the initial process array by start time,then enqueues the processes into the process queue
	@param proc The array of processes read from the input file
	@param length The number of processes
	@return The (sorted) process queue
*/
QUEUE * sort(process **proc, int length) {
	int swap_count;
	QUEUE *pqueue = init("pqueue");
	
	//sort the processes within the array
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
	
	//enqueue the processes whilst initialising their 'run' queues (to store process run-time information)
	for(int i=0;i<length;i++) {
		proc[i]->run = init("run");
		enqueue(pqueue,proc[i],sizeof(proc[i]));
	}
	return pqueue;
}
