#include "args.h"
#include "mem.h"

struct settings *handle_args(const int argc, const char * const * const argv) {
	struct settings *new = NULL;
	const char *code_tmp = NULL;
	int flags=4, args_errs_flags=0;

	for (int i=1; i<argc; i++)
		if (argv[i][0] == '-') {
			if (argv[i][1] == '-') {
				if (!strcmp(argv[i]+2, "code")) {
					if (i == argc-1) {
						args_errs_flags|=MISSING_PARAMETER;
						goto args_error;
					} else {
						flags |= CODE;
						code_tmp = argv[++i];
					}
				} else if (!strcmp(argv[i]+2, "Warnings-on"))
					flags |= WARNINGS_ON_OFF;
				else if (!strcmp(argv[i]+2, "Warnings-off"))
					flags ^= WARNINGS_ON_OFF;
				else if (!strcmp(argv[i]+2, "help"))
					flags |= GET_HELP;
				else if (!strcmp(argv[i]+2, "version"))
					flags |= GET_VERSION;
				else {
					args_errs_flags|=UNKNOWN_ARGS;
					goto args_error;
				}
			} else {
				switch(argv[i][1]) {
				case 'c':
					if (i == argc-1) {
						args_errs_flags|=MISSING_PARAMETER;
						goto args_error;
					} else { 
						code_tmp = argv[++i];
						flags |= CODE;
						break;
					}
				case 'h':
					flags |= GET_HELP;
					break;
				case 'v':
					flags |= GET_VERSION;
					break;
				default:
					args_errs_flags|=UNKNOWN_ARGS;
					goto args_error;
				}
			}
		} else {
			args_errs_flags|=UNKNOWN_ARGS;
			goto args_error;
		}

args_error:
	return (new = sett_new(code_tmp, flags, args_errs_flags));
}
