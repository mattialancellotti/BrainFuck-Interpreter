#ifndef ARGS_H
#define ARGS_H

#include <string.h>

//ARGS_ERRORS
#define UNKNOWN_ARGS  	  1
#define INCOMPATIBLE_ARGS 2
#define MISSING_PARAMETER 4

struct settings {
	const char *brainfuck_code;
	int flags, args_errs_flags;
};

struct settings *handle_args(const int argc, const char * const * const argv);

#endif
