#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#include "args.h"

#define VERSION 3.0
#define DATE_OF_RELEASE "2020/01/02"
#define BUFFER 30000

#ifdef COLORS
#define NO_COLORS "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#endif

#ifndef COLORS
	#define PRINT_E(X) printf(X, "", "", "");
  #define PRINT_W(X) printf(X, "", "", "");
#else
	#define PRINT_E(X) printf(X, BOLD, RED, NO_COLORS);
	#define PRINT_W(X) printf(X, BOLD, YELLOW, NO_COLORS);
#endif

//CODE_ERRORS_WARNS
#define NO_END_LOOP     1
#define NO_START_LOOP   2
#define UNKNOWN_KEYWORD 4
#define MOVING_OUT_OF_BUFFER 8


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
unsigned loop_counter(const char *); 

#endif
