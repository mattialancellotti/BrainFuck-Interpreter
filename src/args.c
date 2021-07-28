#include <string.h>
#include <stdio.h>

#include <brain/args.h>

/*
 * This function is not using getopt (see `man getopt.3`) because it would be
 * non-sense for just 3/4 parameters without argument.
 */
const char *handle_args(int * const actions, const int argc,
                        char **argv)
{
   const char *file_input = NULL;

   /* Looping through all the arguments given to the program except its name */
   for (int i=1; i<argc; i++)
      if (argv[i][0] == '-' && strlen(argv[i]) > 1) {
         switch(argv[i][1]) {
         case 'i':
            *actions |= INFO;
            break;
         case 'h':
            *actions |= HELP;
            break;
         case 'v':
            *actions |= VERSION;
            break;
         default:
            /* Warning the user and exiting */
            printf("Unknown argument %s. Aborting..\n", argv[i]);
            return NULL;
        }
      } else {
         if (file_input) {
            printf("Unknown argument %s. Aborting..\n", argv[i]);
            return NULL;
         }
         
         file_input = argv[i];
      }

   return file_input;
}
