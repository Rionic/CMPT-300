/*
About this program:
- This program counts words.
- The specific words that will be counted are passed in as command-line
  arguments.
- The program reads words (one word per line) from standard input until EOF 
  or an input line starting with a dot '.'
- The program prints out a summary of the number of times each word has
  appeared.
- Various command-line options alter the behavior of the program.

E.g., count the number of times 'cat', 'nap' or 'dog' appears.
> ./main cat nap dog
Given input:
 cat
 .
Expected output:
 Looking for 3 words
 Result:
 cat:1
 nap:0
 dog:0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smp0_tests.h"
#include "switch.c"

#define LENGTH(s) (sizeof(s) / sizeof(*s))

/* Structures */
typedef struct {
  char *word;
  int counter;
} WordCountEntry2;

//entries[] contains the words typed in on the line of execution
//entry_count is the number of words typed on the line of execution
int process_stream2(WordCountEntry2 entries[], int entry_count)
{
  short line_count = 0;
  char buffer[30];
//looks like buffer contains the words typed afterwards
  while (gets(buffer)) {//get input line by line
    if (*buffer == '.')//stop if theres a .
      break;
    /* Compare against each entry */
    int i = 0;
    while (i < entry_count) {//scan through each word entered on execution line
      if (!strcmp(entries[i].word, buffer))//if typed word is same as
        entries[i].counter++;			   //input, counter++ for that word
      i++;
    }
    line_count++;// move to next line
  }
  return line_count;
}


void print_result2(WordCountEntry2 entries[], int entry_count)
{
  printf("Result:\n");
  int i=0;
  while (entry_count-- > 0) {
    printf("%s:%d\n", entries[i].word, entries[i].counter);
    i++;
  }
}


void printHelp2(const char *name)
{
  fprintf(stderr,"usage: %s [-h] <word1> ... <wordN>\n", name);
}


int main2(int argc, char **argv)
{
  const char *prog_name = *argv;

  WordCountEntry2 entries[5];
  int entryCount = 0;

  /* Entry point for the testrunner program */
  if (argc > 1 && !strcmp(argv[1], "-test")) {
    run_smp0_tests(argc - 1, argv + 1);
    return EXIT_SUCCESS;
  }
  argv++;
  while (*argv != NULL) { //while A[i]!=NULL
    if (**argv == '-') { //if A[i][0]== '-'

      switch ((*argv)[1]) { //switch A[1][1]
        case 'h':
          printHelp2(prog_name);
          break; 
        default:
          fprintf(stderr, "%s: Invalid option %s. Use -h for help.\n",
                 prog_name, *argv);
      }
    } else {
      if (entryCount < LENGTH(entries)) {
        entries[entryCount].word = *argv;
        entries[entryCount++].counter = 0;
      }
    }
    argv++;
  }
  if (entryCount == 0) {
    fprintf(stderr, "%s: Please supply at least one word. Use -h for help.\n",
           prog_name);
    return EXIT_FAILURE;
  }
  if (entryCount == 1) {
    printf("Looking for a single word\n");
  } else {
    printf("Looking for %d words\n", entryCount);
  }

  process_stream2(entries, entryCount);
  print_result2(entries, entryCount);

  return EXIT_SUCCESS;
}
