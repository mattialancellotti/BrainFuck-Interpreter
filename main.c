#include<stdio.h>

#include "main.h"
#include "mem.h"

int main(const int argc, char **argv) {
  FILE *f = NULL;
	struct settings *settings_f = NULL;
  char *code = NULL;

	if (argc < 2) {
		print_console_help();
		goto exit;
	}

	settings_f = handle_args(argc, argv);

	if (settings_f->args_errs_flags) {
    printf("Errors during the handling of the arguments.. exiting.\n");
		goto exit;
	}

	if (settings_f->flags&1)
		print_console_version();

	if (settings_f->flags&2)
		print_console_help();

  if (!settings_f->finput)
    goto clean;

  f = fopen(settings_f->finput, "r");
  code = bfile_reader(f);
  if (!code)
    goto clean;

  printf("%s\n", code);
	interpreter(code);

clean:
  if (f)
    fclose(f);
  xfree_sett(settings_f);

exit:
  f = NULL;
  settings_f = NULL;

	return 0;
}

void interpreter(const char *code_) {
  const unsigned len = loop_counter(code_), c_len = strlen(code_); 
  register unsigned loop_index = 0;
  int loops[len];
  char code[c_len];

  char buffer[BUFFER] = { 0 };
  register char *ptr = buffer;

  memset(loops, 0, len);
  memcpy(code, code_, c_len);

	for (unsigned i=0; i<c_len; i++)
		switch(code[i]) {
		case '+':
      ++(*ptr);
      break;
		case '-':
      --(*ptr);
			break;
		case '>':
      if (ptr == buffer+(BUFFER-1))
        ptr = buffer;
      else
        ++ptr;
			break;
		case '<':
      if (ptr == buffer)
        ptr = buffer+(BUFFER-1);
      else
        --ptr;
			break;
		case '.':
      putchar(*ptr);
			break;
		case ',':
      *ptr = getchar();
			break;
		case '[':
      if (loops[loop_index])
        ++loop_index;
      loops[loop_index] = i;

			break;
		case ']':
      if (*ptr)
        i = loops[loop_index];
      else {
        loops[loop_index] = 0;
        if (loop_index)
          --loop_index;
      }
		  break;
		default:
			break;
		} 
}

void show_buf(const int *buf) {
	for (int i=0; i<BUFFER; i++)
		printf("%3d ", buf[i]);

	printf("\n");
}

void print_console_help(void) {
	printf("~~~~ BRAINFUCK INTERPRETER ~~~~\n");
	printf("\t-h, --help    : display this message\n");
	printf("\t-v, --version : print the version of the program\n");
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

char *bfile_reader(FILE *file) {
  fseek(file, 0L, SEEK_END);
  const unsigned size = ftell(file);
  unsigned new_size;
  char code[size];
  fseek(file, 0L, SEEK_SET);

  char *new_code = NULL;
  int i = 0;

  char c;
  if (file)
    while((c = fgetc(file)) != EOF)
      if (c == LEFT_ARROW || c == RIGHT_ARROW
                          || c == INCREMENT
                          || c == DECREMENT
                          || c == START_WHILE
                          || c == END_WHILE
                          || c == INPUT
                          || c == OUTPUT)
      code[i++] = c;

  code[i] = '\0';

  new_size = strlen(code);
  new_code = malloc(sizeof(char)*new_size);

  memcpy(new_code, code, new_size);

  return new_code;
}
