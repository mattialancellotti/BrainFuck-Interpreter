#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "main.h"

int main(const int argc, const char * const * const argv) {
	struct settings *settings_f;
	int *buf;
	//int ptr=0;

	init(&buf, &settings_f);
	settings_f = handle_args(argc, argv);
	//analyzer(code_tmp, buf, ptr);
	//show_buf(buf);

	free(buf);
	return 0;
}

void init(int **buf, struct settings **settings_f) {
	*buf = malloc(sizeof(int)*BUFFER);
	*buf = memset(buf, 0, BUFFER);
	*settings_f = NULL;
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
	const char *code_tmp, *config_file;
	int flags=0;

	for (int i=1; i<argc; i++)
		if (argv[i][0] == '-') {
			if (argv[i][1] == '-') {
				if (!strcmp(argv[i]+2, "code"))
					code_tmp = argv[++i];
				else if (!strcmp(argv[i]+2, "debug"))
					flags |= 1;
				else if (!strcmp(argv[i]+2, "colors"))
					flags |= (!strcmp(argv[++i], "true") ? 2 : 0);
				else if (!strcmp(argv[i]+2, "configuration-file"))
					config_file = argv[++i];
				else if (!strcmp(argv[i]+2, "Warnings-on"))
					flags |= 4;
				else if (!strcmp(argv[i]+2, "Warnings-off"))
					flags ^= 4;
				else if (!strcmp(argv[i]+2, "Errors-on"))
					flags |= 8;
				else if (!strcmp(argv[i]+2, "Errors-off"))
					flags ^= 8;
				else {
					;
				}
			} else {
				switch(argv[i][1]) {
				case 'c':
					code_tmp = argv[++i];
					break;
				case 'd':
					flags |= 1;
					break;
				case 'l':
					flags |= (!strcmp(argv[++i], "true") ? 2 : 0);
					break;
				case 'i':
					config_file = argv[++i];
					break;
				default:
					//send_err(UNKNOWN_ARG, i);
					break;
				}
			}
		}

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
	if (*to_free) {
		if ((*to_free)->brainfuck_code)
			free((*to_free)->brainfuck_code);

		if ((*to_free)->configuration_file)
			free((*to_free)->configuration_file);

		free(*to_free);
	}
}

void analyzer(const char *code, int *buf, int ptr) {
	struct loop_t *head = NULL;
	int i=0;

	while(code[i]) {
		switch(code[i]) {
		case '+':
			buf[ptr]++;
			break;
		case '-':
			buf[ptr]--;
			break;
		case '>':
			if (ptr == BUFFER-1)
				//send_warns(MOVING_OUT_BUFFER, i);
				ptr = 0;
			else
				ptr++;
			break;
		case '<':
			if (ptr == 0)
				//send_warns(MOVING_OUT_BUFFER, i);
				ptr = BUFFER-1;
			else
				ptr--;
			break;
		case '.':
			printf("%d\n", buf[ptr]);
			break;
		case ',':
			printf("+");
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
			else //send_err(NO_START_LOOP, i);
				return;
			break;
		default:
			//send_err(UNKNOWN_KEYWORD, i)
			break;
		}
		i++;
	}

	if (head) {
		//send_err(NO_END_LOOP, i);
		return;
	}
}

void show_buf(int *buf) {
	for (int i=0; i<BUFFER; i++)
		printf("%3d ", buf[i]);
	printf("\n");
}

int check_errs(const char *code) {
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
	int movement=0;
	for (int i=0; code[i]; i++)
		if (code[i] == '>')
			movement++;
		else if (code[i] == '<')
			movement--;
	return (!movement ? 0 : MOVING_OUT_OF_BUFFER);
}
