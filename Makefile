CC = clang
RM = rm -rf

OBJS = mem.o args.o main.o
SRCS = $(wildcard src/*)
BINARY = bf

CFLAGS = -Wall -Wpedantic -Wextra -O3

ifdef DEBUG
  CFLAGS += -ggdb -fsanitize=address,undefined
endif

$(BINARY) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BINARY)

mem.o : mem.c mem.h main.h args.h
	$(CC) $(CLFAGS) -c mem.c

args.o : args.c args.h mem.h
	$(CC) $(CLFAGS) -c args.c

main.o : main.c main.h args.h mem.h
	$(CC) $(CLFAGS) -c main.c

.PHONY : clean
clean :
	$(RM) $(OBJS)
