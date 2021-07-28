CC = clang
RM = rm -rf
INCLUDE = include
CFLAGS = -Wall -Wpedantic -Wextra -std=c11 -O3 -I$(INCLUDE)

OBJSDIR = bin
SRCSDIR = src

OBJS := $(addprefix $(OBJSDIR)/, mem.o args.o main.o)
DEPS := $(patsubst %.o, %.d, $(OBJS))
SRCS := $(wildcard src/*.c)

BINARY = bf

ifdef DEBUG
  CFLAGS += -ggdb -fsanitize=address,undefined
endif

$(BINARY): | $(OBJSDIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BINARY)

$(OBJSDIR):
	@echo $(DEPS) - $(SRCS) - $(OBJS)
	@echo Creating $(abspath $(OBJSDIR))
	@mkdir -p $(OBJSDIR)

$(OBJSDIR)/%.o: $(SRCSDIR)/%.c $(OBJSDIR)/%.d $(INCLUDE)/brain/%.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJSDIR)/%.d: $(SRCSDIR)/%.c
	$(CC) $(CFLAGS) -MM -MT '$(patsubst %.c, %(OBJSDIR)/%.o, $(notdir $<))' $< -MF $@


.PHONY : clean
clean :
	$(RM) $(BINARY) $(OBJSDIR)
