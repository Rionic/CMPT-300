#ifndef	__SCHED_IMPL__H__
#define	__SCHED_IMPL__H__
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "list.h"

struct thread_info {
	list_elem_t *item;
	sched_queue_t *queue;
	sem_t tsem;
};

struct sched_queue {
	list_t *s_queue;
	int pos;
	sem_t qsem;
	sem_t csem;
};

#endif /* __SCHED_IMPL__H__ */