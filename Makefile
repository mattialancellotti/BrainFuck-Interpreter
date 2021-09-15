CC = clang
RM = rm -rf
INCLUDE = include
CFLAGS = -Wall -Wpedantic -Wextra -std=c11 -O3 -I$(INCLUDE)
BINARY = brain

OBJSDIR = objs
SRCSDIR = src
BINDIR  = bin
INSTDIR = /usr/local/stow/$(BINARY)

SRCS := $(wildcard src/*.c)
OBJS := $(patsubst %.c, $(OBJSDIR)/%.o, $(notdir $(SRCS)))
DEPS := $(patsubst %.c, %.d, $(SRCS))


ifdef DEBUG
  CFLAGS += -ggdb -fsanitize=address,undefined
endif

$(BINARY): | $(BINDIR) $(OBJSDIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BINDIR)/$(BINARY)

$(BINDIR):
	@echo Creating $(abspath $(BINDIR))
	mkdir -p $(BINDIR)

$(OBJSDIR):
	@echo $(DEPS) - $(SRCS) - $(OBJS) - $(basename $(SRCS))
	@echo Creating $(abspath $(OBJSDIR))
	mkdir -p $(OBJSDIR)

$(OBJSDIR)/%.o: $(SRCSDIR)/%.c $(OBJSDIR)/%.d $(INCLUDE)/brain/%.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJSDIR)/%.d: $(SRCSDIR)/%.c
	$(CC) $(CFLAGS) -MM -MT '$(patsubst %.c, %(OBJSDIR)/%.o, $(notdir $<))' $< -MF $@


.PHONY : clean stow unstow
clean :
	$(RM) $(BINDIR) $(OBJSDIR)

stow:
	@echo Creating $(INSTDIR)
	mkdir -p $(INSTDIR)
	cp -r $(BINDIR) $(INSTDIR)
	stow --dir=/usr/local/stow --target=/usr/local --stow $(BINARY)
	@echo Installation finished

unstow:
	stow --dir=/usr/local/stow --target=/usr/local --delete $(BINARY)
	@echo Successfully uninstalled
