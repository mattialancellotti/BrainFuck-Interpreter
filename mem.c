#include "mem.h"
#include "main.h"

struct settings *sett_new(const char *brainfuck_code, int flags, int args_errs_flags) {
	struct settings *new_struct = malloc(sizeof(struct settings));
	new_struct->brainfuck_code = brainfuck_code;
	new_struct->flags = flags;
	new_struct->args_errs_flags = args_errs_flags;
	return new_struct;
}

struct loop_t *push(unsigned start, struct loop_t *next, struct loop_t *prev) {
	struct loop_t *new = malloc(sizeof(struct loop_t));
	new->start_position = start;
	new->end_position = start;
	new->next = next;
	new->prev = prev;
	return new;
}

void xfree_loop(struct loop_t **to_free) {
	if (*to_free) {
		if ((*to_free)->next) 
			free((*to_free)->next);
		free(*to_free);
	}
}

void xfree_sett(struct settings **to_free) {
	if (*to_free)
		free(*to_free);
}
