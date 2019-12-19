objects = mem.o args.o main.o
args = -Wall -Wpedantic -Wextra -ggdb

BrainFuckInterpreter : $(objects)
	cc -o BrainFuckInterpreter $(objects)

mem.o : mem.c mem.h main.h args.h
	cc $(args) -c mem.c

args.o : args.c args.h mem.h
	cc $(args) -c args.c

main.o : main.c main.h args.h mem.h
	cc $(args) -c main.c

.PHONY : clean
clean :
	rm $(objects)

