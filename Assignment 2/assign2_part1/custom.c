#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv){
	char* a = "a";
	//int *a = (int*)malloc(sizeof(int*));
	//*a = 5;
	printf("%c\n",*a);
	return 1;
	
}