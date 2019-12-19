#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

#include "args.h"

struct settings *sett_new(const char *, int, int);
struct loop_t *push(unsigned, struct loop_t *, struct loop_t *);
void xfree_loop(struct loop_t **);
void xfree_sett(struct settings **);
#endif
