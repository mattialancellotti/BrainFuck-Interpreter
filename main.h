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

//CODE_ERRORS {{
#define NO_END_LOOP     1
#define NO_START_LOOP   2
#define UNKNOWN_KEYWORD 4
// }}

//CODE WARNINGS {{
#define INFINITE_LOOP        1
#define MOVING_OUT_OF_BUFFER 8
// }}

//Notes:
//  [DELETE] print_code_warn_errs(const int, const int) ; print_args_warn_errs(const int)
//      | Not needed since there will be an entire interface dedicated to write a pretty output
//  [MODIFY] print_console_help(void);
//      | New name 'help(void);' it will display only the explanation of the arguments of the program
//  [MODIFY] print_console_version(void);
//      | New anem 'version(void);'
//  [MOVING][compiler/] interpreter(const char *, int *, int) ; 
//                      check_errs(const char *)              ; 
//                      check_warns(const char *)             ; 
//                      loop_counter(const char *)
//      | The main part of the program will be separated from the compiler/interpreter
//    - CODE_ERRORS
//    - CODE_WARNINGS
//    - BUFFER
//    
//
//main

/*
 * Used to initialize the array passed by reference
 * @param $0 : the pointer to int to initialize
 * @return 
 */
void init(int **);

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
 * Prints the last version and the date of release
 * @return
 */
void print_console_version(void);

/*
 * Prints every value contained in the cell of the buffer
 * @param $0 : the buffer
 * @return
 */
void show_buf(const int *);


//brainfuck

/*
 * Uses a switch-case to understand which key is the current one and act consequently.
 * To handle these instructions [ '+', '-', '>', '<', '.', ',' ] it copies the C programming language, while for the last 
 * two instructions [ '[', ']' ] it uses an array to store the start index of the loop and the end of it to jump over the code.
 * @param $0 : the brainfuck code
 * @param $1 : the buffer used
 * @param $2 : the variable used as index for the buffer
 * @return
 */
void interpreter(const char *, int *, int);

/*
 * It checks if there are some errors like unknown instructions or non-ended loops or non-opened loops
 * @param $0 : the brainfuck code
 * @return   : a list of flags which represents the errors
 */
int check_errs(const char *);

/*
 * It checks if the user should pay attentio at what he writes
 * @param $0 : the brainfuck code
 * @return   : a list of flags which represent the errors
 */
int check_warns(const char *);

/*
 * Used to count the maximun number of inner loops
 * @param $0 : the brainfuck code
 * @return   : the number of loops
 */
unsigned loop_counter(const char *); 

#endif
