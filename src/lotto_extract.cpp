//============================================================================
// Name        : lotto_extract.cpp
// Author      : F. Strati
// Version     :
// Copyright   : Apache License v.2.0
// Description : Computes statistics on Lotto historical extractions
//============================================================================

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

#include "utilities.h"
using namespace lotto_base;

#include "parse_and_process.h"
using namespace lotto_extract;

void print_usage(std::vector<std::string> in_args);
int32_t parse_process_cmd(std::vector<std::string> in_args, int argc, char *argv[]);

int main(int argc, char *argv[])
{
	std::fprintf(stderr,"!!! This is %s !!!\n",argv[0]);

	std::vector<std::string> in_args = parse_arguments(argc, argv);

	if( in_args.size() < 2 )
	{
		print_usage(in_args);
		return -1;
	}

	int32_t ret = parse_process_cmd(in_args, argc, argv);

	return ret;
}

void print_usage(std::vector<std::string> in_args)
{
	std::fprintf(stderr,"Usage: %s command [options]\n",in_args[0].c_str());
	std::fprintf(stderr,"\twhere command can be:\n");
	std::fprintf(stderr,"\t\t %s\n",IN_CMD_RESTART_FROM_SCRATCH);
	std::fprintf(stderr,"\t\t %s\n",IN_CMD_UPDATE);
	std::fprintf(stderr,"\t\t %s {frequency,delay} {number_to_print}\n",IN_CMD_COMPUTE);
	std::fprintf(stderr,"\t\t %s\n",IN_CMD_HELP);
}

int32_t parse_process_cmd(std::vector<std::string> in_args, int argc, char *argv[])
{
	int32_t ret = -1;

	if(parse_process_init())
		return -1;

	if( std::string(IN_CMD_RESTART_FROM_SCRATCH) == in_args[1] )
	{
		ret = parse_process_restart(in_args, argc, argv);
	}
	else if( std::string(IN_CMD_UPDATE) == in_args[1] )
	{
		ret = parse_process_update(in_args, argc, argv);
	}
	else if( std::string(IN_CMD_COMPUTE) == in_args[1] )
	{
		ret = parse_process_compute(in_args, argc, argv);
	}
	else
	{
		print_usage(in_args);
		return -1;
	}

	return ret;
}

