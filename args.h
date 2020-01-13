#ifndef ARGS_H
#define ARGS_H

#include <string.h>
#include <stdio.h>

//ARGS_OPTIONS
#define GET_VERSION 1
#define GET_HELP    2
#define GET_INFO    3
#define GET_STATS   4

struct settings {
  char *finput;
	int flags, args_errs_flags;
};

/*
 * Handles the arguments by reading them and saving the 'state' in a variable processed by the main function.
 * @param argc               : the number of arguments
 * @param argv               : array of strings, the arguments
 * @return struct settings * : settings structure that stores the code and some flags
 */
struct settings *handle_args(const int argc, char **argv);

#endif
