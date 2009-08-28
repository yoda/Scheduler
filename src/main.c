#include <stdlib.h>
#include <stdio.h>
#include "cmdline.h"
#include "handler.h"

#define debug

int main(int argc, char **argv) {

	struct gengetopt_args_info args_info;

	printf("Prosim simulator\n");
	
	if(cmdline_parser(argc, argv, &args_info) != 0)
	{	
		exit(1);
	}

#ifdef debug	
	if( args_info.input_given)
	{
		printf("Input filename: %s\n", args_info.input_arg);
	}
	
	if(args_info.quantum_given)
	{
		printf("Quantum specified: %d\n", args_info.quantum_arg);
	}

	if(args_info.expire_given)
	{
		printf("Expire specified: %d\n", args_info.expire_arg);
	}

	if(args_info.algorithm_given)
	{
		printf("Algorithm specified: %s\n", args_info.algorithm_arg);
	}
#endif
	
	int *q; // Quantum
	int *e; // Exit point
	char **a; // Algorithm

	if(args_info.scheduler_given)
	{
#ifdef debug
		printf("Running scheduler\n");
#endif
		q = &args_info.quantum_arg;
		
		if(args_info.algorithm_given)
		{
			a = &args_info.algorithm_arg;
		}
		else
		{
			a = &args_info.algorithm_orig;
		}

		scheduler(&args_info.input_arg, a, q);
		
	} 
	else if(args_info.virtualmem_given)
	{
#ifdef debug
		printf("Running virtualmem\n");
#endif
	}

	cmdline_parser_free(&args_info);

	return 0;

}
