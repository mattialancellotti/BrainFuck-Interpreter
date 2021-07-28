#include <brain/args.h>
#include <brain/mem.h>

struct settings *handle_args(const int argc, char **argv) {
   char *file_input = NULL;
   int flags=4, args_errs_flags=0;

   for (int i=1; i<argc; i++)
      if (argv[i][0] == '-') {
         if (strlen(argv[i]) > 1) {
            switch(argv[i][1]) {
            case 'i':
               flags |= GET_INFO;
               break;
            case 'h':
               flags |= GET_HELP;
               break;
            case 'v':
          flags |= GET_VERSION;
          break;
        default:
          args_errs_flags = 1;
          break;
        }
     } else
       printf("Incomplete parameter %s.... Ignoring it.\n", argv[i]);

    } else {
      if (!file_input)
        file_input = argv[i];
      else {
        printf("Multiple input files: %s - %s.", file_input, argv[i]);
        args_errs_flags = 1;
      }
    }

	return sett_new(file_input, flags, args_errs_flags);
}
