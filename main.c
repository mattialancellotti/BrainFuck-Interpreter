#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "main.h"

int main(const int argc, const char * const * const argv) {
	struct settings *settings_f;
	int *buf;
	int ptr=0, tmp;

	init(&buf, &settings_f);
	settings_f = handle_args(argc, argv);



	if (settings_f->flags&1)
		print_console_version();

	if (settings_f->flags&2)
		print_console_help();

	print_console_errs((tmp = check_errs(settings_f->brainfuck_code)));
        if (tmp || settings_f->flags&32)
		goto exit;
	if (settings_f->flags&4)
		print_console_warn(check_warns(settings_f->brainfuck_code));


	interpreter(settings_f->brainfuck_code, buf, ptr);
	show_buf(buf);

exit:
	xfree_sett(&settings_f);
	free(buf);
	return 0;
}

void init(int **buf, struct settings **settings_f) {
	(*buf) = malloc(sizeof(int)*BUFFER);
	memset(*buf, 0, BUFFER);
	(*settings_f) = NULL;
}

struct settings *sett_new(const char *brainfuck_code, const char *config_file, int flags) {
	struct settings *new_struct = malloc(sizeof(struct settings));
	new_struct->brainfuck_code = brainfuck_code;
	new_struct->configuration_file = config_file;
	new_struct->flags = flags;
	return new_struct;
}

struct settings *handle_args(const int argc, const char * const * const argv) {
	struct settings *new;
	const char *code_tmp = NULL, *config_file = NULL;
	int flags=4;

	for (int i=1; i<argc; i++)
		if (argv[i][0] == '-') {
			if (argv[i][1] == '-') {
				if (!strcmp(argv[i]+2, "code"))
					code_tmp = argv[++i];
				else if (!strcmp(argv[i]+2, "debug"))
					flags |= 8;
				else if (!strcmp(argv[i]+2, "configuration-file"))
					config_file = argv[++i];
				else if (!strcmp(argv[i]+2, "Warnings-on"))
					flags |= 4;
				else if (!strcmp(argv[i]+2, "Warnings-off"))
					flags ^= 4;
				else if (!strcmp(argv[i]+2, "help"))
					flags |= 2;
				else if (!strcmp(argv[i]+2, "version"))
					flags |= 1;
				else {
					flags |= 32;
					print_console_errs(UNKNOWN_ARGS);
					goto args_error;
				}
			} else {
				switch(argv[i][1]) {
				case 'c':
					code_tmp = argv[++i];
					break;
				case 'd':
					flags |= 8;
					break;
				case 'i':
					config_file = argv[++i];
					break;
				case 'h':
					flags |= 2;
					break;
				case 'v':
					flags |= 1;
					break;
				default:
					flags |= 32;
					print_console_errs(UNKNOWN_ARGS);
					goto args_error;
				}
			}
		}

args_error:
	return (new = sett_new(code_tmp, config_file, flags));
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

void interpreter(const char *code, int *buf, int ptr) {
	struct loop_t *head = NULL;

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
			printf("[@] Current position %d: %d\n", i, buf[ptr]);
			break;
		case ',':
			printf("[@] Input: ");
			buf[ptr] = getchar();
			break;
		case '[':
			if (!buf[ptr])
				while(buf[i] != ']')	
					i++;
			else 
				if (!head)
					head = push(i, NULL, NULL);
				else {
					head->next = push(i, NULL, head);
					head = head->next;
				}
			break;
		case ']':
			if (head)
				if (!buf[ptr])
					if (head->prev) {
						head = head->prev;
						xfree_loop(&(head->next));
						head->next = NULL;
					} else
						xfree_loop(&head);
				else
				i = head->start_position;
			else 
				return;
			break;
		default:
			//skip
			break;
		} 
}

void show_buf(const int *buf) {
	for (int i=0; i<BUFFER; i++)
		printf("%3d ", buf[i]);
	printf("\n");
}

int check_errs(const char *code) {
	if (!code)
		return -1;

	int count_of_loops=0;
	for (int i=0; code[i]; i++)
		if (code[i] == '[')
			count_of_loops++;
		else if (code[i] == ']')
			count_of_loops--;
		else if (code[i] != '+' && code[i] != '-' && code[i] != '<' && code[i] != '>' && code[i] != '.' && code[i] != ',')
			return UNKNOWN_KEYWORD;

	return (!count_of_loops ? 0 : (count_of_loops>0 ? NO_END_LOOP : NO_START_LOOP));
}

int check_warns(const char *code) {
	if (!code)
		return -1;

	int movement=0, flag;
	for (int i=0; code[i]; i++) {
		if (code[i] == '>')
			movement++;
		else if (code[i] == '<')
			movement--;

		if (movement < 0 || movement > BUFFER)
			return MOVING_OUT_OF_BUFFER;
	}

	return 0; 
}

#ifndef COLORS
	#define PRINT_E(X) printf(X, "", "", "");
#else
	#define PRINT_E(X) printf(X, BOLD, RED, NO_COLORS);
	#define PRINT_W(X) printf(X, BOLD, YELLOW, NO_COLORS);
#endif

void print_console_warn(const int code) {
	switch(code) {
	case 1:
		PRINT_W("%s[MOVING_OUT_OF_BUFFER] %sWarning%s: you are trying to the buffer, resetting pointer.\n");
		break;
	default:
		break;
	}
}

void print_console_errs(const int code) {
	switch(code) {
	case 1:
		PRINT_E("%s[NO_END_LOOP] %sError%s: no end of loop found.\n");
		break;
	case 2:
		PRINT_E("%s[NO_START_LOPP] %sError%s: no start of loop found.\n");
		break;
	case 3:
		PRINT_E("%s[UNKNOWN_KEYWORD] %sError%s: keyword not found.\n");
		break;
	case 10:
		PRINT_E("%s[UNKNOWN_ARGS] %sError%s: argument not found.\n");
		break;
	default:
		break;
	}
}

void print_console_help(void) {
	printf("~~~~ BRAINFUCK INTERPRETER ~~~~\n");
	printf("\t-c, --code:     accept a parameter for the code ex: \"++>>+[]\"\n");
	printf("\t--Warnings-off: disable warnings\n");
	printf("\t--Warnings-on:  enable warnings\n");
	printf("\t-h, --help :    display this message\n");
	printf("\t-v, --version:  print the version of the progrma\n");
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
