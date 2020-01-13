#include "mem.h"
#include "main.h"

struct settings *sett_new(char *file, int flags, int args_errs_flags) {
	struct settings *new_struct = malloc(sizeof(struct settings));

  new_struct->finput = file;
	new_struct->flags = flags;
	new_struct->args_errs_flags = args_errs_flags;

	return new_struct;
}

void xfree_sett(struct settings *to_free) {
	if (to_free)
		free(to_free);
}
