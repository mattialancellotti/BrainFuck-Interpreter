/*
FLOW:
	+ "Load"    -> [ Read from args, Read from files ]
	+ "Compile" -> [ Check for warnings, Check for errors ]
	+ "Execute" -> [ Execute the code ]

TODO:
	+ [ ] Add signal interrupt exception handler
	+ [ ] Errors/Warnings handler
	+ [ ] I/O for files
*/
#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#define BUFFER 50

enum ARGS_ERRORS   { UNKNWON_ARGS  = 1 };
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
void analyzer(const char *, int *, int);
struct loop_t *push(unsigned, struct loop_t *, struct loop_t *);
void xfree_loop(struct loop_t **);
void xfree_sett(struct settings **);
void help(void);
void show_buf(int *);
int check_errs(const char *);
int check_warns(const char *);
struct settings *sett_new(const char *, const char *, int);
struct settings *handle_args(int argc, const char * const * const argv);

#endif
