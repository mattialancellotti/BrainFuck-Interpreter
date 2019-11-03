#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#define VERSION 1.2
#define DATE_OF_RELEASE "2019/11/03"
#define BUFFER 50
#define COLORS

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

//ARGS_ERRORS
#define UNKNOWN_ARGS  	  1
#define INCOMPATIBLE_ARGS 2
#define MISSING_PARAMETER 4

struct loop_t {
	struct loop_t *next, *prev;
	unsigned start_position, end_position;
};

struct settings {
	const char *brainfuck_code;
	int flags, args_errs_flags;
};

//main
void init(int **, struct settings **);
struct settings *handle_args(const int argc, const char * const * const argv);
void print_code_warn_errs(const int, const int);
void print_args_warn_errs(const int);
void print_console_help(void);
void print_console_version(void);
void show_buf(const int *);

//mem
struct loop_t *push(unsigned, struct loop_t *, struct loop_t *);
void xfree_loop(struct loop_t **);
void xfree_sett(struct settings **);

//brainfuck
void interpreter(const char *, int *, int);
int check_errs(const char *);
int check_warns(const char *);
struct settings *sett_new(const char *, int, int);

#endif
