/*
FLOW:
	+ "Load"    -> [ Read from args, Read from files ]
	+ "Compile" -> [ Check for warnings, Check for errors ]
	+ "Execute" -> [ Execute the code ]

TODO:
	+ [ ] Add signal interrupt exception handler
	+ [ ] I/O for files
	+ [ ] Debugger
*/
#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#define VERSION 1.1
#define DATE_OF_RELEASE "2019/10/27"
#define BUFFER 50
#define COLORS

#ifdef COLORS
#define NO_COLORS "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#endif

enum ARGS_ERRORS   { UNKNOWN_ARGS  = 10 };
enum CODE_WARNINGS { MOVING_OUT_OF_BUFFER = 1 };
enum CODE_ERRORS   { NO_END_LOOP   	  = 1, NO_START_LOOP = 2, UNKNOWN_KEYWORD = 3 };

struct loop_t {
	struct loop_t *next, *prev;
	unsigned start_position, end_position;
};

struct settings {
	const char *brainfuck_code, *configuration_file;
	int flags;
};

void init(int **, struct settings **);
void interpreter(const char *, int *, int);
struct loop_t *push(unsigned, struct loop_t *, struct loop_t *);
void xfree_loop(struct loop_t **);
void xfree_sett(struct settings **);
void show_buf(const int *);
int check_errs(const char *);
int check_warns(const char *);
struct settings *sett_new(const char *, const char *, int);
struct settings *handle_args(const int argc, const char * const * const argv);
void print_console_warn(const int);
void print_console_errs(const int);
void print_console_help(void);
void print_console_version(void);

#endif
