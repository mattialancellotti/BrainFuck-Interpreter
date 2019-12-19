#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#include "args.h"

#define VERSION 2.0
#define DATE_OF_RELEASE "2019/12/19"
#define BUFFER 50

#ifdef COLORS
#define NO_COLORS "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#endif

//CODE_ERRORS_WARNS
#define NO_END_LOOP     1
#define NO_START_LOOP   2
#define UNKNOWN_KEYWORD 4
#define MOVING_OUT_OF_BUFFER 2

struct loop_t {
	struct loop_t *next, *prev;
	unsigned start_position, end_position;
};

//main
void init(int **, struct settings **);
void print_code_warn_errs(const int, const int);
void print_args_warn_errs(const int);
void print_console_help(void);
void print_console_version(void);
void show_buf(const int *);


//brainfuck
void interpreter(const char *, int *, int);
int check_errs(const char *);
int check_warns(const char *);

#endif
