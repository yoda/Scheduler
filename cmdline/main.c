#include <stdlib.h>
#include <stdio.h>
#include "cmdline.h"

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
#endif debug

	cmdline_parser_free(&args_info);

	return 0;
}
