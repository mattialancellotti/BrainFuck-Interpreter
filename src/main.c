#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* 
 * The following three libraries are needed to use mmap as an efficient way
 * to read files. sys/stat.h and fcntl.h both provide useful methods to get
 * a file's properties.
 */
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

/* My headers */
#include <brain/args.h>
#include <brain/main.h>

#define simple_usage(); printf("bf [-h|-v|-i] <brainfuck source>\n");
#define check_bit(flags, flag, action); if (flags&flag) action();
#define do_if(condition, action); \
   if (condition) { \
      action;       \
   }

#define warn_user(errno, status, err_msg, action); \
   if (errno == status) { \
      perror(err_msg);    \
      action;             \
   }

#define fatal_err(errno, status, err_msg); \
   if (errno == status) { \
      perror(err_msg); \
      return EXIT_FAILURE; \
   }

int main(int argc, char **argv)
{
   /* If the user invokes the program with no argument exits successfully */
   if (argc == 1) {
      simple_usage();
      return EXIT_SUCCESS;
   }

   struct stat fprop;
   const char *file_name = NULL;
   char *code = NULL;
   int actions = 0, err;

   file_name = handle_args(&actions, argc, argv);
   if (actions == -1)
      return EXIT_FAILURE;

   /* Checking for flags like -h (help message) or -v (version message) */
   check_bit(actions, VERSION, print_console_version);
   check_bit(actions, HELP,    print_console_help);

   if (!file_name)
      return EXIT_SUCCESS;

   /* Opening the file in read-only mode and returning the file descriptor. */
   int fd = open(file_name, O_RDONLY);
   fatal_err(fd, -1, "open");

   /* Reading this file's infos. */
   err = fstat(fd, &fprop);
   warn_user(err, -1, "fstat", goto main_exit);

   /* 
    * Mapping the file to a virtual address and this specific function is only
    * supported in unix/linux OSes. Needs to be unmapped.
    * 
    * See `man mmap(2)` for more infos.
    */
   code = mmap(NULL, fprop.st_size, PROT_READ, MAP_SHARED, fd, 0);
   warn_user(code, NULL, "mmap", goto main_exit);

   //printf("%s\n", code);
   interpreter(code);

main_exit:
   /* Cleaning resources */
   do_if((fd != -1), fatal_err(close(fd), -1, "close"));
   do_if((code != NULL), fatal_err(munmap(code, fprop.st_size), -1, "munmap"));

   /* Exit successfully */
   return EXIT_SUCCESS;
}

void print_console_version(void)
{
   printf("BrainFuck interpreter, version %.1f, date of release %s\n",
         PROGRAM_VERSION, DATE_OF_RELEASE);
}

void print_console_help(void)
{
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

void interpreter(const char *code) {
   const unsigned loops_count = loop_counter(code);
   const unsigned code_len = strlen(code);
   register unsigned loop_index = 0;
   int loops[loops_count];

   char buffer[BUFFER] = { 0 };
   register char *ptr = buffer;

   memset(loops, 0, loops_count);

   for (unsigned i=0; i<code_len; i++)
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
         if (loops[loop_index] > 0)
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
         /* Ignoring the unrecognised character */
	 break;
      }
}

void show_buf(const int *buf) {
   for (int i=0; i<BUFFER; i++)
      printf("%3d ", buf[i]);

   printf("\n");
}

unsigned loop_counter(const char *code) {
  unsigned loops = 0;

  for (int i=0; code[i]; i++)
    if (code[i] == '[')
      loops++;

  return loops;
}
