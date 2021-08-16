#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#define PROGRAM_VERSION (3.3)
#define DATE_OF_RELEASE "2021/08/12"
#define BUFFER (30000)

#define RIGHT_ARROW '>'
#define LEFT_ARROW  '<'
#define INCREMENT   '+'
#define DECREMENT   '-'
#define INPUT       ','
#define OUTPUT      '.'
#define START_WHILE '['
#define END_WHILE   ']'

/*
 * Used to interpret the flags passed and to notify the used if there are errors or warnings in the *code*
 * @param $0 : flags of errors
 * @param $1 : flags of warnings
 * @return
 */
void print_code_warn_errs(const int, const int);

/*
 * As the 'print_code_warn_errs(const int, const int);' but for args flags
 * @param $0 : flags of errors in parameters
 * @return
 */
void print_args_warn_errs(const int);

/*
 * Just prints an help message to tell the user how to use the program and the brainfuck language
 * @return
 */
void print_console_help(void);

/*
 * Prints the last version and the date of release.
 * @return
 */
void print_console_version(void);

/*
 * Prints every value contained in the cell of the buffer.
 * @param $0 : the buffer
 * @return
 */
void show_buf(const int *);


//brainfuck

/*
 * Uses a switch-case to understand which key is the current one and act
 * consequently. To handle these instructions [ '+', '-', '>', '<', '.', ',' ]
 * it copies the C programming language, while for the last two instructions
 * [ '[', ']' ] it uses an array to store the start index of the loop and the
 * end of it to jump over the code.
 *
 * @param $0 : the brainfuck code
 * @return
 */
void interpreter(const char *);

/*
 * Used to count the maximun number of inner loops
 * @param $0 : the brainfuck code
 * @return   : the number of loops
 */
unsigned loop_counter(const char *); 

#endif
