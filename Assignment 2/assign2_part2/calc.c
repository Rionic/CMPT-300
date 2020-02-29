

/* calc.c - Multithreaded calculator */
#include "calc.h"

pthread_t adderThread;
pthread_t degrouperThread;
pthread_t multiplierThread;
pthread_t readerThread;
pthread_t sentinelThread;

char buffer[BUF_SIZE];

sem_t lock;
int num_ops;

/* Utiltity functions provided for your convenience */

/* int2string converts an integer into a string and writes it in the
   passed char array s, which should be of reasonable size (e.g., 20
   characters).  */
char *int2string(int i, char *s)
{
    sprintf(s, "%d", i);
    return s;
}

/* string2int just calls atoi() */
int string2int(const char *s)
{
    return atoi(s);
}

/* isNumeric just calls isdigit() */
int isNumeric(char c)
{
    return isdigit(c);
}

/* End utility functions */


void printErrorAndExit(char *msg)
{
    msg = msg ? msg : "An unspecified error occured!";
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

int timeToFinish()
{
    /* be careful: timeToFinish() also accesses buffer */
    return buffer[0] == '.';
}

/* Looks for an addition symbol "+" surrounded by two numbers, e.g. "5+6"
   and, if found, adds the two numbers and replaces the addition subexpression 
   with the result ("(5+6)*8" becomes "(11)*8")--remember, you don't have
   to worry about associativity! */
void *adder(void *arg){	 
    while (1) {
    int bufferlen, i, result;
    char value1[500] = {};
    char value2[500] = {};
    char temp[500] = {};
    int startOffset = -1;
    int remainderOffset = -1;
    char exp[500] = {};

   	if(sem_wait(&lock)) 
		printErrorAndExit("An error has occurred"); 
	startOffset = remainderOffset = -1;

	if (timeToFinish()){
	    if(sem_post(&lock))
	     	printErrorAndExit("An error has occurred");  
   		return NULL;
	}
	/* storing this prevents having to recalculate it in the loop */
	bufferlen = strlen(buffer);
	for (i = 0; i < bufferlen; i++) { 
		if(buffer[i]==';')
			break;
		if(buffer[i] == '+' && buffer[i+1]=='+'){
			fprintf(stdout, "No progress can be made\n");
			exit(EXIT_FAILURE);
			break;
			}
		if(isdigit(buffer[i])){	

			startOffset = i;
			while(isdigit(buffer[i])) //get to next value [+,),*,(]
				i++;
			if(buffer[i] == '+' && buffer[i+1]=='+'){
				fprintf(stdout, "No progress can be made\n");
				exit(EXIT_FAILURE);
				break;
			}
			if(buffer[i]!='+' || !isdigit(buffer[i+1]))	// exit if its incorrect format
				continue;

			else{
				i++; //increment past operator
				while(isdigit(buffer[i]))
					i++;
				
				remainderOffset = i-1;
				break;					
			}	//offsets now encompass the expression to be added
		}
	}
	if(remainderOffset!=-1){ //if we have a valid expression		
		for(i=startOffset; i<=remainderOffset; i++){ //iterate through the target expression
			while(isdigit(buffer[i])){
				strncat(value1, &buffer[i],1);
				i++;
			}
			i++; //skip over the operator
			while(isdigit(buffer[i])){
				strncat(value2, &buffer[i],1);
				i++;
			}
		}
				
		result = string2int(value1);		//convert to ints for operation
		result += string2int(value2);
		sprintf(exp, "%d", result);			//exp is now a string
		strncat(temp, buffer, startOffset); //temp = beforestuff
		strcat(temp, exp);					//temp = beforestuff+exp
		strcat(temp, buffer+remainderOffset+1);//temp = beforestuff+exp+afterstuff
		strcpy(buffer, temp);
        buffer[strlen(temp)] = '\0';
        num_ops++;
	}

    if(sem_post(&lock)) 
        printErrorAndExit("An error has occurred"); 
	sched_yield();
    }
}

/* Looks for a multiplication symbol "*" surrounded by two numbers, e.g.
   "5*6" and, if found, multiplies the two numbers and replaces the
   mulitplication subexpression with the result ("1+(5*6)+8" becomes
   "1+(30)+8"). */
void *multiplier(void *arg){		
    while (1) {
    int bufferlen, i, result;
    char value1[500] = {};
    char value2[500] = {};
    char temp[500] = {};
    int startOffset = -1;
    int remainderOffset = -1;
    char exp[500] = {};

	if (timeToFinish()){
		if(sem_post(&lock))
			printErrorAndExit("An error has occurred");  
	    return NULL;
	}
	/* storing this prevents having to recalculate it in the loop */
	bufferlen = strlen(buffer);
	for (i = 0; i < bufferlen; i++) { 
		if(buffer[i]==';')
			break;
		if(buffer[i] == '*' && buffer[i+1]=='*'){
			fprintf(stdout, "No progress can be made\n");
			exit(EXIT_FAILURE);
			break;
		}
		if(isdigit(buffer[i])){	
			startOffset = i;
			while(isdigit(buffer[i])) //get to next value [+,),*,(]
				i++;
			if(buffer[i] == '*' && buffer[i+1]=='*'){
				fprintf(stdout, "No progress can be made\n");
				exit(EXIT_FAILURE);
				break;
			}
			if(buffer[i]!='*' || !isdigit(buffer[i+1]))	// exit if its incorrect format
				continue;

			else{
				i++; //increment past operator
				while(isdigit(buffer[i]))
					i++;
				
				remainderOffset = i-1;
				break;					
			}	//offsets now encompass the expression to be added
		}
	}
	if(remainderOffset!=-1){ //if we have a valid expression
        if(sem_wait(&lock))
        	printErrorAndExit("An error has occurred"); 

		for(i=startOffset; i<=remainderOffset; i++){ //iterate through the target expression
			while(isdigit(buffer[i])){
				strncat(value1, &buffer[i],1);
				i++;
			}
			i++; //skip over the operator
			while(isdigit(buffer[i])){
				strncat(value2, &buffer[i],1);
				i++;
			}
		}
				
		result = string2int(value1);		//convert to ints for operation
		result *= string2int(value2);
		sprintf(exp, "%d", result);			//exp is now a string
		strncat(temp, buffer, startOffset); //temp = beforestuff
		strcat(temp, exp);					//temp = beforestuff+exp
		strcat(temp, buffer+remainderOffset+1);//temp = beforestuff+exp+afterstuff
		strcpy(buffer, temp);
        num_ops++;
	}

   	if(sem_post(&lock))
		printErrorAndExit("An error has occurred"); 
	sched_yield();
    }
}


/* Looks for a number immediately surrounded by parentheses [e.g.
   "(56)"] in the buffer and, if found, removes the parentheses leaving
   only the surrounded number. */
void *degrouper(void *arg){
    while (1) {
	int bufferlen, i;
    int startOffset = -1;
    int remainderOffset = -1;
    char temp[500] = {};
    char exp[500] = {};

    if(sem_wait(&lock)) 
		printErrorAndExit("An error has occurred"); 

	if (timeToFinish()){ 
    	if(sem_post(&lock))
      		printErrorAndExit("An error has occurred");  
	    return NULL;
    }
	
	/* storing this prevents having to recalculate it in the loop */
	bufferlen = strlen(buffer);
	for (i = 0; i < bufferlen; i++) {
		
		if(buffer[i]==';')
			break;
		if(buffer[i]=='('){
			startOffset = i;
			i++;
      while(buffer[i]=='('){
        startOffset = i;
        i++;
      }
			if(!isdigit(buffer[i]))
				continue;
			else{
				while(isdigit(buffer[i]))
					i++;
			}
			if(buffer[i]==')'){
				remainderOffset = i; 	// we've found our target boys
				break;					// end if we found (num)
			}	
		}
	}
	if(remainderOffset!=-1){ //valid expression?
		for(i=startOffset+1; i<remainderOffset; i++) //iterate through the target expression
			strncat(exp, &buffer[i], 1);
		
		strncat(temp, buffer, startOffset); //temp = beforestuff
		strcat(temp, exp);				//temp = beforestuff+exp
		strcat(temp, buffer+remainderOffset+1);//temp = beforestuff+exp+afterstuff
		strcpy(buffer, temp);
        num_ops++;
	}

	if(sem_post(&lock)) 
		printErrorAndExit("An error has occurred"); 
	sched_yield();
    }
}


/* sentinel waits for a number followed by a ; (e.g. "453;") to appear
   at the beginning of the buffer, indicating that the current
   expression has been fully reduced by the other threads and can now be
   output.  It then "dequeues" that expression (and trailing ;) so work can
   proceed on the next (if available). */
void *sentinel(void *arg)
{
    char numberBuffer[20];
    int bufferlen;
    int i;

    while (1) {
    sem_wait(&lock);

	if (timeToFinish()) {
		sem_post(&lock); 
	    return NULL;
	}
	/* storing this prevents having to recalculate it in the loop */
	bufferlen = strlen(buffer);

	for (i = 0; i < bufferlen; i++) {
	    if (buffer[i] == ';') {
	        //printf("sent buf: %s\n", buffer);
			if (i == 0) {
			    printErrorAndExit("Sentinel found empty expression!");
				}
			else {
			    /* null terminate the string */
			    numberBuffer[i] = '\0';
			    /* print out the number we've found */
			    fprintf(stdout, "%s\n", numberBuffer);
			    /* shift the remainder of the string to the left */
			    strcpy(buffer, &buffer[i + 1]);
			    break;
			}
		}
	    else if (!isNumeric(buffer[i])) {
	    	sem_post(&lock);
			break;
	    } 
	    else 
			numberBuffer[i] = buffer[i];
	}
	sem_post(&lock); 
	sched_yield();
	// something missing?
    }
}

/* reader reads in lines of input from stdin and writes them to the
   buffer */
void *reader(void *arg)
{
    while (1) {
	char tBuffer[100];
	int currentlen;
	int newlen;
	int free;

	fgets(tBuffer, sizeof(tBuffer), stdin);

	/* Sychronization bugs in remainder of function need to be fixed */

	newlen = strlen(tBuffer);
	currentlen = strlen(buffer);

	/* if tBuffer comes back with a newline from fgets, remove it */
	
	if (tBuffer[newlen - 1] == '\n') {
	    /* shift null terminator left */
	    tBuffer[newlen - 1] = tBuffer[newlen];
	    newlen--;
	}
	/* -1 for null terminator, -1 for ; separator */
	free = sizeof(buffer) - currentlen - 2;

	while (free < newlen) {
		sem_wait(&lock);
		currentlen = strlen(buffer);
		free = sizeof(buffer) - currentlen - 2;
		sem_post(&lock);
		sched_yield();
	}

	/* we can add another expression now */
	sem_wait(&lock);
	strcat(buffer, tBuffer);
	strcat(buffer, ";");
	
	/* Stop when user enters '.' */
	if (tBuffer[0] == '.') 
	    return NULL;
	
  	sem_post(&lock);
  	sched_yield();
    }
}


/* Where it all begins */
int smp3_main(int argc, char **argv)
{	
	//pthread_mutex_t device;
    //sem_t lock;
	sem_init(&lock, 0, 1);
    void *arg = 0;		/* dummy value */

    /* let's create our threads */
    if (pthread_create(&multiplierThread, NULL, multiplier, arg)
	|| pthread_create(&adderThread, NULL, adder, arg)
	|| pthread_create(&degrouperThread, NULL, degrouper, arg)
	|| pthread_create(&sentinelThread, NULL, sentinel, arg)
	|| pthread_create(&readerThread, NULL, reader, arg)) {
	printErrorAndExit("Failed trying to create threads");
    }

    /* you need to join one of these threads... but which one? */
    pthread_join(readerThread, NULL);
    pthread_detach(sentinelThread);
    pthread_detach(adderThread);
    pthread_detach(multiplierThread);
    pthread_detach(degrouperThread);
    usleep(30000);
    //pthread_detach(readerThread);


    /* everything is finished, print out the number of operations performed */
    fprintf(stdout, "Performed a total of %d operations\n", num_ops);
    return EXIT_SUCCESS;
}