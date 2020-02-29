#include "scheduler.h"
#include "sched_impl.h"

/* Fill in your scheduler implementation code below: */

static void init_thread_info(thread_info_t *info, sched_queue_t *queue){
	info->queue = queue;

	list_elem_t *elt = (list_elem_t*) malloc(sizeof(list_elem_t));

	list_elem_init(elt, info);
	info->item = elt;
	
	if(sem_init(&(info->tsem),0,0))
		printf("Semaphore is moody. Initialization failed.\n");
}

static void destroy_thread_info(thread_info_t *info){
	free(info->item);
	info->queue=NULL;
	info->item=NULL;

	if(sem_destroy(&(info->tsem)))
		printf("Semaphore is too strong. It could not be destroyed.\n");
}
/* Block until the thread can enter the scheduler queue. */
static void enter_sched_queue(thread_info_t *info){
	if(sem_wait(&(info->queue->qsem)))
		printf("Impatient semaphore. Failed to wait.\n");

	list_insert_tail(info->queue->s_queue, info->item);
}
/* Remove the thread from the scheduler queue. */
static void leave_sched_queue(thread_info_t *info){
	list_remove_elem(info->queue->s_queue, info->item);
	if(sem_post(&(info->queue->qsem)))
		printf("Angry semaphore. Failed to unblock.\n");	

	info->queue->pos--;
}
/* While on the scheduler queue, block until thread is scheduled. */
static void wait_for_cpu(thread_info_t *info){
	if(sem_wait(&(info->tsem)))
		printf("Impatient semaphore. Failed to wait.\n");
	
}
/* Voluntarily relinquish the CPU when this thread's timeslice is
 * over (cooperative multithreading). */
static void release_cpu(thread_info_t *info){
	if(sem_post(&(info->queue->csem)))
		printf("Angry semaphore. Failed to unblock.\n");	
}




static void init_sched_queue(sched_queue_t *queue, int queue_size){
	if(queue==NULL)
		queue = (sched_queue_t*)malloc(sizeof(sched_queue_t));
	//printf("test %p\n", queue->s_queue);
	queue->s_queue =(list_t*) malloc(sizeof(list_t));
	queue->pos = 0;

	list_init(queue->s_queue);

	if(sem_init(&(queue->qsem), 0, queue_size)
	   || sem_init(&(queue->csem), 0, 0))
		printf("Semaphore is moody. Initialization failed.\n");
}

static void destroy_sched_queue(sched_queue_t *queue){
	if(queue!=NULL){
		free(queue->s_queue);
		queue=NULL;
	}
	if(sem_destroy(&(queue->csem)) || sem_destroy(&(queue->qsem)))
		printf("Semaphore is too strong. It could not be destroyed.\n");
}

/* Allow a worker thread to execute. */
static void wake_up_worker(thread_info_t *info){
	if(sem_post(&(info->tsem)))
		printf("Angry semaphore. Failed to unblock.\n");	
}

/* Block until the current worker thread relinquishes the CPU. */
static void wait_for_worker(sched_queue_t *queue){
	if(sem_wait(&(queue->csem)))
		printf("Impatient semaphore. Failed to wait.\n");
}

/* Select the next worker thread to execute.
 * Returns NULL if the scheduler queue is empty. */
static thread_info_t* next_worker_FIFO(sched_queue_t *queue){
	if (list_size(queue->s_queue)>0){//check if not empty
		thread_info_t *next = (thread_info_t*)(queue->s_queue->head->datum);
		return next;
	}
	return NULL;
}

static thread_info_t* next_worker_RR(sched_queue_t *queue){
	if (list_size(queue->s_queue)>0){ // issues?
		if(queue->pos==list_size(queue->s_queue))
			queue->pos = 0;

		list_elem_t *head = list_get_head(queue->s_queue);
		for (int i = 0; i<queue->pos; i++){
			if(head==NULL)
				head = list_get_head(queue->s_queue);
			else
				head = head->next;
		}
		thread_info_t *next = (thread_info_t*)(head->datum);
		queue->pos++;
		return next;
	}

	return NULL;
}

/* Block until at least one worker thread is in the scheduler queue. */
static void wait_for_queue(sched_queue_t *queue){
	int count = list_size(queue->s_queue);
	while(count==0)
		count = list_size(queue->s_queue);

}

/* You need to statically initialize these structures: */
sched_impl_t sched_fifo = {
	{ init_thread_info, destroy_thread_info, enter_sched_queue,
	 leave_sched_queue, wait_for_cpu, release_cpu }, 
	{ init_sched_queue, destroy_sched_queue, wake_up_worker,
	 wait_for_worker, next_worker_FIFO, wait_for_queue} 
};
sched_impl_t sched_rr = {
	{ init_thread_info, destroy_thread_info, enter_sched_queue, 
		leave_sched_queue, wait_for_cpu, release_cpu }, 
	{ init_sched_queue, destroy_sched_queue, wake_up_worker,
	 wait_for_worker, next_worker_RR, wait_for_queue}
};