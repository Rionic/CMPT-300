#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>

int main()
{
	char word[10]={"a\0b"};
	printf("%ld %ld\n",sizeof(word),strlen(word));
}