#include<stdio.h>

#include "main.h"
#include "mem.h"

static int skip(const char *, int);

int main(const int argc, const char * const * const argv) {
	struct settings *settings_f = NULL;
	int *buf = NULL;
	int ptr=0, tmp;

	if (argc < 2) {
		print_console_help();
		goto exit;
	}

	init(&buf, &settings_f);
	settings_f = handle_args(argc, argv);

	if (settings_f->args_errs_flags) {
		print_args_warn_errs(settings_f->args_errs_flags);
		goto exit;
	}

	if (settings_f->flags&1)
		print_console_version();

	if (settings_f->flags&2)
		print_console_help();

	if (settings_f->flags&8) {
		if (settings_f->flags&4)
			print_code_warn_errs((tmp = check_errs(settings_f->brainfuck_code)), (check_warns(settings_f->brainfuck_code)+1));
		else
			print_code_warn_errs((tmp = check_errs(settings_f->brainfuck_code)), 0);

    if (tmp)
			goto exit;

		interpreter(settings_f->brainfuck_code, buf, ptr);
		//show_buf(buf);
	}

exit:
	xfree_sett(settings_f);
  settings_f = NULL;
	free(buf);
  buf = NULL;
	return 0;
}

void init(int **buf, struct settings **settings_f) {
	(*buf) = malloc(sizeof(int)*BUFFER);
	memset(*buf, 0, BUFFER);
	(*settings_f) = NULL;
}

void interpreter(const char *code, int *buf, int ptr) {
  const unsigned len = loop_counter(code)*3; 
  unsigned loop_index = 0;
  int loops[len];
  memset(loops, 0, len*sizeof(int));

	for (int i=0; code[i]; i++)
		switch(code[i]) {
		case '+':
			buf[ptr]++;
			break;
		case '-':
			buf[ptr]--;
			break;
		case '>':
			if (ptr == BUFFER-1)
				ptr = 0;
			else
				ptr++;
			break;
		case '<':
			if (ptr == 0)
				ptr = BUFFER-1;
			else
				ptr--;
			break;
		case '.':
			printf("%c", buf[ptr]);
			break;
		case ',':
			printf("[@] Input: ");
			buf[ptr] = getchar();
			break;
		case '[':
      //if the current value is equal to zero then try to find the end of the loop
			if (!buf[ptr])
          i = skip(code, i+1);
      else { //else jump to the next free position and set it 
        if (loops[loop_index])
          loop_index+=2;
        loops[loop_index] = i;
      }

			break;
		case ']':
      if (buf[ptr]) {
        if (!loops[loop_index+1])
          loops[loop_index+1] = i;
        i = loops[loop_index];
      } else {
        loops[loop_index] = 0;
        loops[loop_index+1] = 0;
        if (loop_index)
          loop_index-=2;
      }
        
		  break;
		default:
			break;
		} 
}

static int skip(const char *code, int i) {
  for (int loop_count = 1; loop_count; i++)
    if (code[i] == '[')
      loop_count++;
    else if (code[i] == ']')
      loop_count--;

  return i-1;
}

void show_buf(const int *buf) {
	for (int i=0; i<BUFFER; i++)
		printf("%3d ", buf[i]);
	printf("\n");
}

int check_errs(const char *code) {
	int count_of_loops=0, flags=0;
	for (int i=0; code[i]; i++)
		if (code[i] == '[')
			count_of_loops++;
		else if (code[i] == ']')
			count_of_loops--;
		else if (code[i] != '+' && code[i] != '-' && code[i] != '<' && code[i] != '>' && code[i] != '.' && code[i] != ',' && code[i] != '[' && code[i] != ']')
			flags|=UNKNOWN_KEYWORD;

  if (count_of_loops > 0)
    return (flags|=NO_END_LOOP);
  else if (count_of_loops < 0)
    return (flags|=NO_START_LOOP);
  else
    return flags;
}

int check_warns(const char *code) {
	int movement=0, flags=0;
	for (int i=0; code[i]; i++) {
		if (code[i] == '>')
			movement++;
		else if (code[i] == '<')
			movement--;

		if (movement < 0 || movement > BUFFER)
			return flags|=MOVING_OUT_OF_BUFFER;
	}

	return flags; 
}

void print_code_warn_errs(const int errs_code, const int warns_code) {
	if (warns_code&1)
		if (warns_code&MOVING_OUT_OF_BUFFER)
			PRINT_W("%s[MOVING_OUT_OF_BUFFER] %sWarning%s: you are trying to the buffer, resetting pointer.\n");

	if (errs_code&NO_END_LOOP)
		PRINT_E("%s[NO_END_LOOP] %sError%s: no end of loop found.\n");

	if (errs_code&NO_START_LOOP)
		PRINT_E("%s[NO_START_LOOP] %sError%s: no start of loop found.\n");

	if (errs_code&UNKNOWN_KEYWORD)
		PRINT_E("%s[UNKNOWN_KEYWORD] %sError%s: keyword not found.\n");
}

void print_args_warn_errs(const int errs_code) {
	if (errs_code&UNKNOWN_ARGS)
		PRINT_E("%s[UNKNOWN_ARGS] %sError%s: argument not found.\n");
	if (errs_code&INCOMPATIBLE_ARGS)
		PRINT_E("%s[INCOMPATIBLE_ARGS] %sError%s: you can't check warnings in debugging mode.\n");
	if (errs_code&MISSING_PARAMETER)
		PRINT_E("%s[MISSING_PARAMETER] %sError%s: this argument needs a parameter.\n");
}

void print_console_help(void) {
	printf("~~~~ BRAINFUCK INTERPRETER ~~~~\n");
	printf("\t-c, --code    : accept a parameter for the code ex (add the quotes): \"++>>+[]\"\n");
	printf("\t-h, --help    : display this message\n");
	printf("\t-v, --version : print the version of the progrma\n");
	printf("\t--Warnings-off: disable warnings\n");
	printf("\t--Warnings-on : enable warnings\n");
	printf("\n\n~~~~ BRAINFUCK LANGUAGE ~~~~\n");
	printf("\t'+' increment by one the current position pointed\n");
	printf("\t'-' decrement by one the current position pointed\n");
	printf("\t'>' move the pointer to the next position\n");
	printf("\t'<' move the pointer to the previous position\n");
	printf("\t'.' print the number stored in the current position pointed\n");
	printf("\t',' accept an input\n");
	printf("\t'[' while the value of the position pointed is not zero\n");
	printf("\t']' end of the loop\n\n");
}

void print_console_version(void) {
	printf("BrainFuck interpreter, version %.1f, date of release %s\n", VERSION, DATE_OF_RELEASE);
}

unsigned loop_counter(const char *code) {
  unsigned loops = 0, max_loops=0;

  for (int i=0; code[i]; i++) {
    if (code[i] == '[')
      loops++;
    else if (code[i] == ']') {
      if (loops > max_loops)
        max_loops = loops;
      loops--;
    }
  }

  return max_loops;
}
