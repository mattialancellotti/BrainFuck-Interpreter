#include<stdio.h>

#include "main.h"
#include "mem.h"

static int skip(const char *, int);

int main(const int argc, char **argv) {
	struct settings *settings_f = NULL;
  char *code = NULL;

	if (argc < 2) {
		print_console_help();
		goto exit;
	}

	settings_f = handle_args(argc, argv);

  FILE *f = fopen(settings_f->finput, "r");
  code = bfile_reader(f);
  printf("%s\n", code);

	if (settings_f->args_errs_flags) {
    printf("Errors during the handling of the arguments.. exiting.\n");
		goto exit;
	}

	if (settings_f->flags&1)
		print_console_version();

	if (settings_f->flags&2)
		print_console_help();

	interpreter(code);

clean:
  fclose(f);
  xfree_sett(settings_f);

exit:
  f = NULL;
  settings_f = NULL;

	return 0;
}

void interpreter(const char *code_) {
  const unsigned len = loop_counter(code_)*3, c_len = strlen(code_); 
  register unsigned loop_index = 0;
  int loops[len], c;
  char buffer[BUFFER] = { 0 };
  char code[c_len];

  char *ptr = buffer;

  memset(loops, 0, len*sizeof(int));
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
			++ptr;
			break;
		case '<':
			--ptr;
			break;
		case '.':
      putchar(*ptr);
			break;
		case ',':
			printf("[@] Input: ");
			*ptr = (int)getchar();

      while((c=getchar()) != '\n' && c != EOF) { }
			break;
		case '[':
      //if the current value is equal to zero then try to find the end of the loop
			if (*ptr == 0) {
        if (loops[loop_index+1])
          i = loops[loop_index+1];
        else
          i = skip(code, i+1);
      } else { //else jump to the next free position and set it 
        if (loops[loop_index])
          loop_index+=2;
        loops[loop_index] = i;
      }

			break;
		case ']':
      if (*ptr) {
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
  for (int loop_count = 1; loop_count; ++i)
    if (code[i] == '[')
      ++loop_count;
    else if (code[i] == ']')
      --loop_count;

  return i-1;
}

void show_buf(const int *buf) {
	for (int i=0; i<BUFFER; i++)
		printf("%3d ", buf[i]);

	printf("\n");
}

/*
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
*/

/*
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
*/

/*
void print_code_warn_errs(const int errs_code, const int warns_code) {
	if (warns_code&1)
		if (warns_code&MOVING_OUT_OF_BUFFER)
			printf("[MOVING_OUT_OF_BUFFER] Warning: you are trying to exit the buffer, resetting pointer.\n");

	if (errs_code&NO_END_LOOP)
		printf("[NO_END_LOOP] Error: no end of loop found.\n");

	if (errs_code&NO_START_LOOP)
		printf("[NO_START_LOOP] Error: no start of loop found.\n");

	if (errs_code&UNKNOWN_KEYWORD)
		printf("[UNKNOWN_KEYWORD] Error: keyword not found.\n");
}
*/

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

  unsigned buffer[BUFFER] = { 0 };
  unsigned *ptr = buffer;

  char *new_code;

  int i = 0, jump = 0, loops = 0;

  char c;
  if (file) {
    while((c = fgetc(file)) != EOF) {
      if (jump) {
        if (c == START_WHILE)
          ++loops;
        else if (c == END_WHILE)
          --loops;

        if (!loops)
          jump = 0;

        continue;
      }

      switch(c) {
        case LEFT_ARROW:
          if (ptr != buffer)
            --ptr;
          else
            ptr = buffer+(BUFFER-1);

          break;
        case RIGHT_ARROW:
          if (ptr == buffer+(BUFFER-1))
            ptr = buffer;
          else
            ++ptr;

          break;
        case INCREMENT:
          ++(*ptr);
          break;
        case DECREMENT:
          --(*ptr);
          break;
        case START_WHILE:
          if (!i || (!*ptr && code[i-1] != ',')) {
            jump = loops = 1;
            continue;
          }

          break;
        case END_WHILE:
        case INPUT:
        case OUTPUT:
          break;
        default:
          continue;
      }

      code[i++] = c;
    }
  }

  code[i] = '\0';
  new_size = strlen(code);
  new_code = malloc(sizeof(char)*new_size);

  memcpy(new_code, code, new_size);

  return new_code;
}
