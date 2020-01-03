#ifndef ARGS_H
#define ARGS_H

#include <string.h>

//ARGS_ERRORS
#define MISSING_PARAMETER 4
#define INCOMPATIBLE_ARGS 2
#define UNKNOWN_ARGS  	  1

//ARGS_OPTIONS
#define CODE              8
#define WARNINGS_ON_OFF   4
#define GET_HELP          2
#define GET_VERSION       1

struct settings {
	const char *brainfuck_code;
	int flags, args_errs_flags;
};

//Notes:
//  [DELETE] Brainfuck code inserted via args
//      | Not needed since the input will happen via file
//  [DELETE] ARGS_ERRORS
//      | There will be a 'Real-Time' errors/warnings processing
//
//  [ADDING] Real-Time errors/warnings processing
//      | To improve speed and to waste less memory in structures
//  [ADDING] Arguments will have a higher priority then the rest of the program
//      | Because otherwise you won't be able to make a little mod. to your
//      | configuration without modifying the config.yaml
//  [ADDING] -o, --optimize: to optimize the code
//      | Reducing the amount of instructions
//  [ADDING] -s, --statistics: outputs some statistics
//      | Just to see how many instructions have been processesed and the time it 
//      | took
//  [ADDING] -g, --generate-executable: to generate an executable
//      | To execute the code without a compiler/interpreter
//
//Hypotetical structure for settings
//
// struct settings_t {
//    char *input_f, *output_f;
//    char *optional_configuration_file; //conf.yaml
//    //Warnings(on/off), generate an executable, optimization, statistics
//    int settings_flags;
// };

/*
 * Handle the arguments by reading them and saving the 'state' in a variable processed by the main function.
 * @param argc               : the number of arguments
 * @param argv               : array of strings, the arguments
 * @return struct settings * : settings structure that stores the code and some flags
 */
struct settings *handle_args(const int argc, const char * const * const argv);

#endif
