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
  char *finput, *foutput;
	int flags, args_errs_flags;
};

//Notes:
//
//  [ADDING] -i: to output general info
//      | Combine -h and -v
//  [ADDING] -s: outputs some statistics
//      | Just to see how many instructions have been processesed and the time it 
//      | took
//  [ADDING] -E: to generate an executable
//      | To execute the code without a compiler/interpreter
//  [ADDING] New argument format: -o:true, -s:true, -e:false, -fi:<nome>, -fo:<nome>
//
//Hypotetical structure for settings
//
// struct settings_t {
//    char *input_f, *output_f;
//    //Warnings(on/off), generate an executable, optimization, statistics
//    int settings_flags;
// };

/*
 * Handles the arguments by reading them and saving the 'state' in a variable processed by the main function.
 * @param argc               : the number of arguments
 * @param argv               : array of strings, the arguments
 * @return struct settings * : settings structure that stores the code and some flags
 */
struct settings *handle_args(const int argc, char **argv);

#endif
