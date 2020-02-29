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

#define LENGTH(s) (sizeof(s) / sizeof(*s))

/* Structures */
typedef struct {
  char *word;
  int counter;
} WordCountEntry;


int process_stream(WordCountEntry entries[], int entry_count)
{
  short line_count = 0;
  char buffer[30];
  const char s[2] = " ";
  char* token;
	
  while (fgets(buffer,31,stdin)) {
    if (*buffer == '.')
      break;

    buffer[strcspn(buffer,"\n")] = 0; //get rid of \n
        	
		token = strtok(buffer, s); //use this to parse multiple words per line
		while(token!=NULL){
			for(int i=0; i<entry_count; i++){
				if (!strcmp(entries[i].word, token))
					entries[i].counter++;
			}
			token = strtok(NULL, s);	
		}
    line_count++;
  }
  return line_count;
}


void print_result(WordCountEntry entries[], int entry_count, FILE *fptr)
{
	if(fptr==NULL){
	  printf("Result:\n");
	  int i=0;
	  while (entry_count-- > 0) {
	    fprintf(stdout, "%s:%d\n", entries[i].word, entries[i].counter);
	    i++;
	  }
	}
	else{
	  fprintf(fptr, "Result:\n");
	  int i=0;
	  while (entry_count-- > 0) {
	    fprintf(fptr, "%s:%d\n", entries[i].word, entries[i].counter);
	    i++;
	  }
	}
}


void printHelp(const char *name, FILE *fptr)
{
	if(fptr==NULL)
  	fprintf(stderr,"usage: %s [-h] <word1> ... <wordN>\n", name);
	else
  	fprintf(fptr,"usage: %s [-h] <word1> ... <wordN>\n", name);
}


int main(int argc, char **argv)
{
	char* filename;
	FILE *fptr = NULL; //file pointer for file opening
  const char *prog_name = *argv;

  int entryCount = 0;
  WordCountEntry* entries;
  int numwords = 0; //new counter

  entries = malloc((argc-1)*sizeof(WordCountEntry)); //dynamic array for entries
  for(int j=1; j<argc; j++){
  	entries[j-1].word = argv[j]; //store items
  	numwords++;    //increment counter
  }

  if (argc > 1 && !strcmp(argv[1], "-test")) {
    run_smp0_tests(argc - 1, argv + 1);
    return EXIT_SUCCESS;
  }
  argv++;
  while (*argv != NULL) { 
    if (**argv == '-') { 

      switch ((*argv)[1]) { 
      	case 'f':
      		filename = *argv + 2;
      		fptr = fopen(filename, "w");
        	break; 
        case 'h':
          printHelp(prog_name, fptr);
          break; 
        default:
          fprintf(stderr, "%s: Invalid option %s. Use -h for help.\n",
                 prog_name, *argv);
      }
    } else {
      if (entryCount < numwords) {
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
  	if(fptr==NULL)
    	fprintf(stdout, "Looking for a single word\n");
  	else
  		fprintf(fptr, "Looking for a single word\n");

  } else {
  		if(fptr==NULL)
    		fprintf(stdout, "Looking for %d words\n", entryCount);
  		else
  			fprintf(fptr, "Looking for %d words\n", entryCount);
  }

  process_stream(entries, entryCount);
  print_result(entries, entryCount, fptr);

  return EXIT_SUCCESS;
}
