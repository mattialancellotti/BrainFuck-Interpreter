#ifndef ARGS_H
#define ARGS_H

//ARGS_OPTIONS
#define VERSION (1)
#define GET_HELP    2
#define GET_INFO    3
#define GET_STATS   4

/* TODO: update this doc
 * Handles the arguments by reading them and saving the 'state' in a variable processed by the main function.
 * @param argc               : the number of arguments
 * @param argv               : array of strings, the arguments
 * @return struct settings * : settings structure that stores the code and some flags
 */
const char *handle_args(int * const /*actions*/, const int /*argc*/,
                        char** /*argv*/);

#endif
