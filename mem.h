#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

#include "args.h"

struct settings *sett_new(const char *, int, int);
void xfree_sett(struct settings *);

#endif
