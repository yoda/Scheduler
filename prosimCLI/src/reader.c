#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "reader.h"

/** The page table array*/
int paget[50][3];
/** The physical memory array*/
char *mem[50];
/**	The Swap Space Array*/
char *swap[150];
/** The physical memory print string*/
char *mem_out;
/**	The virtual memory print string*/
char *swap_out;
/** The page table print string*/
char *page_out;

/**	Generates the process name (integer), to be stored in the page table
	@param proc The process with which to generate the name
	@return The integer representing the process id
*/
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

/**	Generates the 4 digit, double concatenated process name (integer), to be stored in the  virtual memory.
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

/**	Copies the current memory and page table states to respective strings for printing at endtime.
	@param	expire The time at which the memory clone is taking place. Used for time stamping the output file.
	@return 1 if the clone is successful, 0 otherwise
*/
int clonePage(int *expire) {
	//malloc for the first part of the page table header
	page_out = malloc(20*sizeof(char)+1);
	strcat(page_out,"<-- Page Table At t=");

	//malloc for the header of the physical memory string
	mem_out = malloc(25*sizeof(char)+1);
	strcat(mem_out,"<-- Physical Memory At t=");
	
	//malloc for the header of the virtual memory string
	swap_out = malloc(24*sizeof(char)+1);
	strcat(swap_out,"<-- Virtual Memory At t=");
		
	//get the expire time as a string
	char buffer[4*sizeof(int)+1];
	sprintf(buffer,"%d",*expire);
	
	//realloc for time and end characters. +7 for the string ' -->\n\n' and the nullbyte
	mem_out = (char*) realloc(mem_out, (strlen(mem_out)*sizeof(char)) + (strlen(buffer)*sizeof(char)) + 7);
	strcat(mem_out,buffer);
	strcat(mem_out," -->\n\n");
	
	swap_out = (char*) realloc(swap_out, (strlen(swap_out)*sizeof(char)) + (strlen(buffer)*sizeof(char)) + 7);
	strcat(swap_out,buffer);
	strcat(swap_out," -->\n\n");

	page_out = (char*) realloc(page_out, (strlen(page_out)*sizeof(char)) + (strlen(buffer)*sizeof(char)) + 7);
	strcat(page_out,buffer);
	strcat(page_out," -->\n\n");
	
	
	char i_buffer[4*sizeof(int)+1];
	char access_buffer[4*sizeof(int)+1];
	
	for(int i=0;i<50;i++) {
		//convert the name and position into strings
		sprintf(buffer,"%d ",paget[i][0]);
		sprintf(access_buffer,"%d\n",paget[i][1]);
		sprintf(i_buffer,"%d",i);
		
		//append to the page table string. +11 is for the strings "Frame " & " -- " and the nullbyte
		page_out = (char*) realloc(page_out, (strlen(page_out)*sizeof(char)) + (strlen(i_buffer)*sizeof(char)) +11);
		strcat(page_out,"Frame ");
		strcat(page_out,i_buffer);
		strcat(page_out," -- ");
		
		//if the page is used, print the job. +5 for the strings "Job " plus the nullbyte
		if(strcmp(buffer,"-1 ") != 0) {
			page_out = (char*) realloc(page_out, (strlen(page_out)*sizeof(char)) + (strlen(buffer)*sizeof(char)) + (strlen(access_buffer)*sizeof(char)) +5);
			strcat(page_out,"Job ");
			strcat(page_out,buffer);
			strcat(page_out,access_buffer);
		}else{
			//else page is empty, so print empty. +7 for the string "Empty\n" and the nullbyte
			page_out = (char*) realloc(page_out, (strlen(page_out)*sizeof(char)) + 7);
			strcat(page_out,"Empty\n");
		}
		
		//fill memory string. +2 for the newline and the nullbyte
		mem_out = (char*) realloc(mem_out, (strlen(mem_out)*sizeof(char)) + (strlen(mem[i])*sizeof(char)) + 2);
		strcat(mem_out,mem[i]);
		strcat(mem_out,"\n");
	}
	
	for(int i=0;i<150;i++) {
		//fill swap string. +2 for the newline and the nullbyte
		swap_out = (char*) realloc(swap_out, (strlen(swap_out)*sizeof(char)) + (strlen(swap[i])*sizeof(char)) + 2);
		strcat(swap_out,swap[i]);
		strcat(swap_out,"\n");
	}
	return 1;
}

/** Updates the time stamp within the page table for the desired process.
	@param pid The process id of the process to be modified within the page table
	@param time_cycle The new access time
*/
void updateTime(int *pid,int *time_cycle) {
	for(int i=0;i<50;i++) {
		if(paget[i][0] == *pid) {
			paget[i][1] = *time_cycle;	
		}	
	}
}

/** Prints the page table and the page contents to the memory output file
*/
void printPage() {
	//create the output file pointer
	FILE *file;
	//try to open the file for writing
	if ((file = fopen("vout.file","w")) == NULL) {
		printf("Failed to create/open the file\n");
		exit(EXIT_FAILURE);
	}
	//write the memory output string to the file
	fprintf(file,"%s\n\n",page_out);
	fprintf(file,"%s\n\n",mem_out);
	fprintf(file,"%s\n",swap_out);
}
	
/** Initialises an empty virtual memory block, with each slot having space for four characters.
*/
void initMemory() {
	//set all pages in memory to 0, indicating 'free' status. also set "last-used" time to -1 and adjust the page table accordingly.
	for(int i=0;i<50;i++) {
		mem[i] = malloc(4*sizeof(char) +1);
		mem[i][0] = '0';
		mem[i][1] = '\0';
		
		paget[i][0] = -1;
		paget[i][1] = -1;
		paget[i][2] = -1;
	}
	for(int i=0;i<150;i++) {
		swap[i] = malloc(4*sizeof(char) +1);
		swap[i][0] = '0';
		swap[i][1] = '\0';
	}
}

/** Returns the number of free/unused frames within physical memory, by scanning the page table
	@return The number of free/unused frames
*/
int findFree() {
	int count = 0;
	for(int i=0;i<50;i++) {
		if(paget[i][0] == -1) {
			count++;
		}
	}
	return count;	
}

/** Returns the number of free/unused frames within virtual memory, by scanning the memory space itself
	@return The number of free/unused frames
*/
int findFreeSwap() {
	int count = 0;
	for(int i=0;i<150;i++) {
		if(strcmp(swap[i],"0") == 0) {
			count++;
		}
	}
	return count;	
}

/** Checks whether the indicated process is currently in memory by scanning the page table.
	@param pid The name of the process
	@return true if the process is found in memory, false otherwise
*/
bool isCached(int pid) {
	for(int i=0;i<50;i++) {
		if(paget[i][0] == pid) {
			return true;
		}
	}
	return false;
}

/**	Frees the pages of the memory frame found at the desired index
	@param index The index representing the start of the frame in the virtual memory array.
*/
void freeFrame(int index) {
	//free the memory frame
	mem[index][0] = '0';
	mem[index][1] = '\0';
	//debug
	//printf("Freed frame [%d] that was used by process %d\n",index,paget[index][0]);
	//adjust the page table
	paget[index][0] = -1;
	paget[index][1] = -1;
	paget[index][2] = -1;
}

/**	
*/
void fillSwap(char *name,int pages) {
	for(int i=0;i<150;i++) {
		if(strcmp(swap[i],"0") == 0) {
			strcpy(swap[i],name);
			pages--;
			printf("Filled swap [%d] with %s\n",i,swap[i]);
		}
		if(pages == 0) {break;}
	}
}

/**
*/
void clearSwap(char *name) {
	for(int i=0;i<150;i++) {
		if(strcmp(swap[i],name) == 0) {
			swap[i][0] = '0';
			swap[i][1] = '\0';
		}
	}
	printf("Swap Complete --> cleared process %s from swap space\n",name);
}

/**
*/
bool inSwap(char *name) {
	for(int i=0;i<150;i++) {
		if(strcmp(swap[i],name) == 0) {
			return true;
		}
	}
	return false;
}

/** Gets the access time of the first valid process listed
	@return The access time of the first valid process within the page table
*/
int getFirstAge() {
	int index = 0;
	while(true) {
		if(paget[index][0] != -1) {
			return paget[index][1];
		}
		index++;
	}
}

/**	Frees the desired number of memory frames, oldest first. Each process has its corresponding frames entirely cleared, not partially.
	@param num The number of frames to be cleared
	@return 1 if frames are successfully freed, 0 otherwise
*/
int freeFrames(int num) {
	int oldest = getFirstAge();					//initialise the age of the oldest process (youngest time_cycle value within the page table)
	char *old_proc = malloc(4*sizeof(char)+1);	//allocate for the name of the oldest process
	char buffer[4*sizeof(int)+1];				//buffer to hold the name of the current process being checked in the page table.
	int swap = 0;								//swap space needed
	char procname[5];							//full name of the process as seen in memory ie 2323 etc
	sprintf(old_proc,"%d",paget[0][0]);			//initialise the name of the oldest process
	while(num>0) {
		//find oldest process & corresponding number of pages that will require swapping
		for(int i=0;i<50;i++) {
			//if not empty or fragmented
			if(paget[i][1] != -1 && paget[i][1] <= oldest) {
			 	oldest = paget[i][1];
			 	swap = paget[i][2];
				sprintf(old_proc,"%d",paget[i][0]);		//sets the name of the oldest process
				sprintf(procname,"%s",mem[i]);			//set the full name (as seen in memory) of the oldest process
			}
		}
		//determine is swap is possible. if not simply break
		if(swap > findFreeSwap() ) {
			printf("More swap space is needed to accomodate process %s. Currently have %d free frames\n",old_proc,findFreeSwap());
			return 0;
		}
		
		//move process to swap space
		fillSwap(procname,swap);
		
		//clear the physical memory
		for(int i=0;i<50;i++) {
			sprintf(buffer,"%d",paget[i][0]);
			if(strcmp(buffer,old_proc) == 0) {
				//free the frame and adjust page table
				freeFrame(i);
				//decrement frames-needed counter
				num--;
			}
		}
		//end case
		if(num <= 0) {
			return 1;
		}
		//reset for start of next loop
		oldest = getFirstAge();
		sprintf(old_proc,"%d",paget[0][0]);
	}
}

/**	Fills the free frames in physical memory with the desired process.
	@param	proc The process to be added to memory
	@param	pid The name of the process
	@param	time_cycle The current cpu time cycle
*/
void fillFrames(process *proc,int pid,int time_cycle) {
	//scan through memory, assigning the process to the first free frame, until the desired number is met
	int pages_req = proc->pages;
	char *pquad = getNameQuad(proc);
	while(true) {
		for(int i=0;i<50;i++) {
			//fill the frame accordingly
			if(strcmp(mem[i],"0") == 0) {
				//set frame value
				strcpy(mem[i],pquad);
				//map to page table
				paget[i][0] = pid;
				paget[i][1] = time_cycle;
				paget[i][2] = proc->pages;
				printf("Filled frame[%d] with %s at time %d\n",i,mem[i],time_cycle);
				//decrement counter
				pages_req--;			
			}
			if(pages_req == 0) {break;}
		}
		break;
	}
	//clear from swap space if it was there
	if(inSwap(pquad)) {clearSwap(pquad);}
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
	@param 	filename The string representing the name of the input file
	@param 	length A counter that records the number of jobs present in the input file
	@param 	mode The program mode. 0 represents Scheduler mode, 1 represents Virtual Memory mode
	@return The array of a processes
*/
process **readFile(char * filename,int *length,int *mode) {
	//try to open the file
	FILE * file;
  	file = fopen (filename,"r");
  	if(file==NULL) {
    	printf("Input file '%s' failed to open, or does not exist! Exiting Program..\n",filename);
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
		if(*mode == 0) {
			
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
				if(proc[*length]->pages > 50 || proc[*length]->pages < 1) {
					printf("Invalid Page Size For %s, Must Be Within The Range (1-50)\n",tempstr); 
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

/**	Stores the given process in physical memory, freeing frames if required
	@param	proc The Process to be cached
	@param	pid The ID of the process to be cached
	@param 	time_cycle The current time cycle of the processor
	@return 1 if the process is successfully cached, 0 otherwise
*/
int cache(process *proc,int *pid,int *time_cycle) {
	//get the number of frames required
	int frames = proc->pages;
	int numfree = findFree();
	int success = 1;
	printf("Need %d frames for %s, have %d frames. Need to free %d frames\n",frames,proc->name,numfree, (frames-numfree) > 0 ? (frames-numfree) : 0 );
	
	//free frames if necessary
	if(frames > numfree) {success = freeFrames(frames-numfree); }
	
	//cache the process if freeing was successful
	if(success == 1) {
		fillFrames(proc,*pid,*time_cycle);
		return 1;
	}
	return 0;
}

/**	Removes the process (specified by the process id) from the physical memory
	@param pid The ID of the process to be removed from memory
*/
void freeProc(int *pid) {
	for(int i=0;i<50;i++) {
		if(paget[i][0] == *pid) {
			freeFrame(i);
		}
	}
	printf("Freed Process %d Upon Completion\n",*pid);
}

/**	Simulates the Round Robin scheduling algorithm for the given time quantum.
	@param pqueue The queue of all available processes
	@param quantum The time quantum
	@param expire The time at which the memory state is cloned for printing to the output file
	@param mode The program mode. 0 represents Scheduler mode, 1 represents Virtual Memory mode
*/
void roundRobin(QUEUE *pqueue,int quantum,int *expire, int *mode) {
	void *temp;						//pointer to the current process
	QUEUE *ready = init("ready");	//queue holding all ready processes at the given time cycle
	size_t sizetemp = 0;			//size of the current element being peeked at or dequeued	
	int time_cycle = 0;				//counter representing cpu ticks (Hz)
	int counter = 0;				//records the process cpu access count
	int *data;						//holds the start time and duration of each cycle of each process
	int pid;						//the process name as an integer. used to reference the page table to determine whether the process is cached
	int success = 1;				//indicates whether the process was successfully cached
	int state_clone = 0;			//indicates whether the memory state has already been cloned
	
	//simulate time cycles with a loop and for each cycle, queue the available processes in a new queue (ready queue)
	while(!isEmpty(pqueue) || !isEmpty(ready)) {
		//find the ready processes given the current time cycle
		getReady(pqueue,ready,time_cycle);
		
		//if there are processes ready, grant access to the first
		if(!isEmpty(ready)) {
			//grab the process
			peek(ready, &temp, &sizetemp);
			process *proc = temp;
			success = 1;
			
			//load the process into memory, if not already there
			if(*mode == 1) {
				pid = getName(proc);
				if(!isCached(pid)) {
					success = cache(proc,&pid,&time_cycle);
				}else {
					updateTime(&pid,&time_cycle);
				}
			}

			//simulate cpu access if the mode is scheduler, or if the mode is memory and the process IS cached
			//if the process has not been cached it cannot be processed, so another must be found for the current cycle
			if(success == 1) {
				proc->duration--;
				printf("Processing %s\n",proc->name);
				counter++;
				time_cycle++;
			}else {
				//add any full-system shuffle algs here
				//add current process to back of the queue (cycles through processes in the case of a bloated system)
				dequeue(ready,&temp,&sizetemp);
				enqueue(ready,(process*)temp,sizetemp);
			}

			//if the process terminates, reset the count and dequeue the process
			if(proc->duration == 0) {
				printf("Completed  %s at t=%d\n",proc->name,time_cycle); 
			   	//add the processing data to the 'run' queue in the current process structure
				data = malloc(2*sizeof(int));
				data[0] = time_cycle-counter;
				data[1] = counter;
				enqueue(proc->run,(void*)data,sizeof(data));
				//print the process data to the output file
			   	output(proc);
			   	//free virtual memory
			   	if(*mode == 1) {
			   		freeProc(&pid);	
			   	}
			   	//free the virtual memory
			   	//dequeue and free the finished process
			    dequeue(ready, &temp, &sizetemp);
				free(temp);
				//reset access counter		    
				counter = 0;
			}
			
			//if the quantum expires, check for new processes and enqueue them before the current process is (re)enqueued, and reset the counter
			if(counter == quantum) {
				//add the processing data to the 'run' queue in the process structure
				data = malloc(2*sizeof(int));
				data[0] = time_cycle-counter;
				data[1] = counter;
				enqueue(proc->run,(void*)data,sizeof(data));
				//reset access counter
				counter = 0;
				//get new processes
				getReady(pqueue,ready,time_cycle);
				//add current process to back of queue
				dequeue(ready,&temp,&sizetemp);
				enqueue(ready,(process*)temp,sizetemp);
			}
			//handle the memory save
			if(*mode == 1 && time_cycle == *expire && state_clone == 0) {state_clone=clonePage(expire);}
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
	@param expire The time at which the memory state is cloned for printing to the output file
	@param mode The program mode. 0 represents Scheduler mode, 1 represents Virtual Memory mode
*/
void firstCome(QUEUE *pqueue, int *expire, int *mode) {
	void *temp;						//pointer to the current process
	QUEUE *ready = init("ready");	//queue holding all ready processes at the given time cycle
	size_t sizetemp = 0;			//size of the current element being peeked at or dequeued	
	int time_cycle = 0;				//counter representing cpu ticks (Hz)
	int counter = 0;				//records the process cpu access count
	int *data;						//holds the start time and duration of each cycle of each process
	int pid;						//the process name as an integer. used to reference the page table to determine whether the process is cached
	int success = 1;				//indicates whether the process was successfully cached
	int state_clone = 0;			//indicates whether the memory state has already been cloned
	
	//simulate time cycles with a loop and for each cycle, queue the available processes in a new queue (ready queue)
	while(!isEmpty(pqueue) || !isEmpty(ready)) {
		//find the ready processes given the current time cycle
		getReady(pqueue,ready,time_cycle);

		//if there are processes ready, grant access to the first
		if(!isEmpty(ready)) {
			//grab the process
			peek(ready, &temp, &sizetemp);
			process *proc = temp;
			success = 1;

			//load the process into memory, if not already there
			if(*mode == 1) {
				pid = getName(proc);
				if(!isCached(pid)) {
					success = cache(proc,&pid,&time_cycle);
				}else {
					updateTime(&pid,&time_cycle);
				}
			}
			
			//simulate cpu access if the mode is scheduler, or if the mode is memory and the process IS cached
			//if the process has not been cached it cannot be processed, so another must be found for the current cycle
			if(success == 1) {
				proc->duration--;
				printf("Processing %s\n",proc->name);
				counter++;
				time_cycle++;
			}else {
				//add any full-system shuffle algs here
				//add current process to back of the queue (cycles through processes in the case of a bloated system)
				dequeue(ready,&temp,&sizetemp);
				enqueue(ready,(process*)temp,sizetemp);
			}
			
			//if the process terminates, reset the count and dequeue the process
			if(proc->duration == 0) {
				printf("Completed  %s at t=%d\n",proc->name,time_cycle); 
				//add the processing data to the 'run' queue in the process structure
				data = malloc(2*sizeof(int));
				data[0] = time_cycle-counter;
				data[1] = counter;
				enqueue(proc->run,(void*)data,sizeof(data));
				//print the process data to the output file
				output((process*)temp);
				//free virtual memory
			   	if(*mode == 1) {
			   		freeProc(&pid);	
			   	}
				//dequeue and free the finished process
			    dequeue(ready, &temp, &sizetemp);
			  	free(temp);
			  	//reset the access counter
				counter = 0;
			}
			//handle the memory save
			if(*mode == 1 && time_cycle == *expire && state_clone == 0) {state_clone = clonePage(expire);}
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
	@param expire The time at which the memory state is cloned for printing to the output file
	@param mode The program mode. 0 represents Scheduler mode, 1 represents Virtual Memory mode
*/
void shortestNext(QUEUE *pqueue,int *expire, int *mode) {
	void *temp;						//pointer to the current process
	QUEUE *ready = init("ready");	//queue holding all ready processes at the given time cycle
	size_t sizetemp = 0;			//size of the current element being peeked at or dequeued	
	int time_cycle = 0;				//counter representing cpu ticks (Hz)
	int counter = 0;				//records the process cpu access count
	int *data;						//holds the start time and duration of each cycle of each process
	int pid;						//the process name as an integer. used to reference the page table to determine whether the process is cached
	bool running = true;			//flag that indicates whether a process is accessing the cpu. Dictates whether the ready queue is sorted or not. 
	int success = 1;				//indicates whether the process was successfully cached
	int state_clone = 0;			//indicates whether the memory state has already been cloned
	
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
			
			//grab the process
			peek(ready, &temp, &sizetemp);
			process *proc = temp;
			success = 1;
			
			//load the process into memory, if not already there
			if(*mode == 1) {
				pid = getName(proc);
				if(!isCached(pid)) {
					success = cache(proc,&pid,&time_cycle);
				}else {
					updateTime(&pid,&time_cycle);
				}
			}
			
			//simulate cpu access if the mode is scheduler, or if the mode is memory and the process IS cached
			//if the process has not been cached it cannot be processed, so another must be found for the current cycle
			if(success == 1) {
				proc->duration--;
				printf("Processing %s\n",proc->name);
				counter++;
				time_cycle++;
			}else {
				//add any full-system shuffle algs here
				//add current process to back of the queue (cycles through processes in the case of a bloated system)
				dequeue(ready,&temp,&sizetemp);
				enqueue(ready,(process*)temp,sizetemp);
			}
			
			//if the process terminates, reset the count and dequeue the process
			if(proc->duration == 0) {
				printf("Completed  %s at t=%d\n",proc->name,time_cycle); 
				//add the processing data to the 'run' queue in the process structure
				data = malloc(2*sizeof(int));
				data[0] = time_cycle-counter;
				data[1] = counter;
				enqueue(proc->run,(void*)data,sizeof(data));
				//print the process data to the output file
				output((process*)temp);
				//free virtual memory
			   	if(*mode == 1) {
			   		freeProc(&pid);	
			   	}
				//dequeue and free the finished process
			    dequeue(ready, &temp, &sizetemp);
			    free(temp);
			    //reset the access counter
				counter = 0;
			    //reset the run flag
			  	running = false;
			}
			//handle the memory save
			if(*mode == 1 && time_cycle == *expire && state_clone == 0) {state_clone = clonePage(expire);}
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
	@param expire The time at which the memory state is cloned for printing to the output file
	@param mode The program mode. 0 represents Scheduler mode, 1 represents Virtual Memory mode	
*/
void shortestRemaining(QUEUE *pqueue,int *expire, int *mode) {
	void *temp;						//pointer to the current process
	QUEUE *ready = init("ready");	//queue holding all ready processes at the given time cycle
	size_t sizetemp = 0;			//size of the current element being peeked at or dequeued	
	int time_cycle = 0;				//counter representing cpu ticks (Hz)
	int counter = 0;				//records the process cpu access count
	int *data;						//holds the start time and duration of each cycle of each process
	int pid;						//the process name as an integer. used to reference the page table to determine whether the process is cached
	process *last = NULL;			//pos of the last process to access the cpu, when it changes it triggers a file write operation
	int success = 1;				//indicates whether the process was successfully cached
	int state_clone = 0;			//indicates whether the memory state has already been cloned
	
	//simulate time cycles with a loop and for each cycle, queue the available processes in a new queue (ready queue)
	while(!isEmpty(pqueue) || !isEmpty(ready)) {
		//find the ready processes given the current time cycle, and sort by remaing time
		getReady(pqueue,ready,time_cycle);
		sortSPN(ready);
		
		//if there are processes ready, sort them by duration and grant access to the first (shortest)
		if(!isEmpty(ready)) {
			//grab the process
			peek(ready, &temp, &sizetemp);
			process *proc = temp;
			success = 1;
			
			//load the process into memory, if not already there
			if(*mode == 1) {
				pid = getName(proc);
				if(!isCached(pid)) {
					success = cache(proc,&pid,&time_cycle);
				}else {
					updateTime(&pid,&time_cycle);
				}
			}
			
			//if the process using the cpu has changed, store the access data from the last process
			if(last != NULL && last != proc && last->duration != 0) {
				//add the processing data to the 'run' queue in the process structure
				data = malloc(2*sizeof(int));
				data[0] = time_cycle-counter;
				data[1] = counter;
				enqueue(last->run,(void*)data,sizeof(data));
				counter=0;
			}
			
			///simulate cpu access if the mode is scheduler, or if the mode is memory and the process IS cached
			//if the process has not been cached it cannot be processed, so another must be found for the current cycle
			if(success == 1) {
				last = proc;
				proc->duration--;
				printf("Processing %s\n",proc->name);
				counter++;
				time_cycle++;
			}else {
				//add any full-system shuffle algs here
				//add current process to back of the queue (cycles through processes in the case of a bloated system)
				dequeue(ready,&temp,&sizetemp);
				enqueue(ready,(process*)temp,sizetemp);
			}
			
			//if the process terminates, reset the count and dequeue the process
			if(proc->duration == 0) {
				printf("Completed  %s at t=%d\n",proc->name,time_cycle); 
				//add the processing data to the 'run' queue in the process structure
				data = malloc(2*sizeof(int));
				data[0] = time_cycle-counter;
				data[1] = counter;
				enqueue(proc->run,(void*)data,sizeof(data));
				//print the process data to the output file
				output((process*)temp);
				//free virtual memory
			   	if(*mode == 1) {
			   		freeProc(&pid);	
			   	}
				//dequeue and free the finished process
			    dequeue(ready, &temp, &sizetemp);
			    free(temp);
			    //reset the access counter
				counter = 0;
			    //reset the last process logger
				last=NULL;
			}
			//handle the memory save
			if(*mode == 1 && time_cycle == *expire && state_clone == 0) {state_clone = clonePage(expire);}
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
