#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int main(void){
	sem_t sem;
	int val;
	sem_init(&sem, 0, 3);
	sem_wait(&sem);
	sem_post(&sem);
	sem_getvalue(&sem, &val);
	printf("sem: %d\n", val);
}