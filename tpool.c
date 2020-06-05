#include "thread_header.h"
#include "tpool.h"
#include <stdlib.h>
#include <pthread.h>

tpool_task_t * tpool_task_create(thread_func_t func, void * args)
{
	tpool_task_t* task;

	if(func == NULL) return NULL;

	task = malloc(sizeof(*task));
	task->func = func;
	task->args = args;
	task->next = NULL;

	return task;
}

void tpool_task_destroy(tpool_task_t* task) {
	if(task == NULL) return ;

	free(task);
}

//Get task from linked_list. it will works like FCFS
tpool_task_t* tpool_task_get(tpool_t* tp) {
	
	tpool_task_t* task;

	if(tp == NULL) return NULL;

	task = tp->task_first;

	if(task == NULL) return NULL;

	if(task->next == NULL) {
		tp->task_first = NULL;
		tp->task_last = NULL;
	}
	else
		tp->task_first = task->next;

	return task;
}

bool tpool_task_add(tpool_t* tp, thread_func_t func, void* args) {
	tpool_task_t* task;

	if(tp == NULL)
		return false;

	task = tpool_task_create(func, args);
	pthread_mutex_lock(&(tp->worker_mutex));

	if(tp->task_first == NULL) {
		tp->task_first = task;
		tp->task_last = tp->task_first;
	} else {
		tp->task_last->next = task;
		tp->task_last = task;
	}

	pthread_mutex_unlock(&(tp->worker_mutex));
	
	return true;
}

void worker(void *args) {
	tpool_t	*tp = (tpool_t*)args;
	tpool_task_t *task;

	while(true) {
		pthread_mutex_lock(&(tp->worker_mutex));

		if(tp->_stop_threads){
			pthread_mutex_unlock(&(tp->worker_mutex));
			break;
		}

		task = tpool_task_get(tp);
		pthread_mutex_unlock(&(tp->worker_mutex));

		if(task != NULL) {
			task->func(task->args);
			tpool_task_destroy(task);
		}
	}
}

//Make Thread Pool
tpool_t *tpool_create(size_t num) {
	tpool_t *tp;
	int i;
	pthread_t thread;

	if(num == 0)
		num = DEFAULT_THREAD_COUNT;

	tp = calloc(1, sizeof(tpool_t));
	tp->_num_threads = num;

	pthread_mutex_init(&(tp->worker_mutex), NULL);

	tp->task_first = NULL;
	tp->task_last = NULL;

	for(i = 0; i < num; i++) {
		pthread_create(&thread, NULL, (void*)&worker,(void*)tp);
		pthread_detach(thread);
	}

	return tp;
}

void tpool_destroy(tpool_t* tp) {
	tpool_task_t *task;
	tpool_task_t *task_next;
	tp->_stop_threads = true;
	
	//if works left, destroy all
	pthread_mutex_lock(&(tp->worker_mutex));
	task = tp->task_first;
	while(task != NULL) {
		task_next = task->next;
		tpool_task_destroy(task);
		task = task_next;
	}
	pthread_mutex_unlock(&(tp->worker_mutex));

	//delete mutex;
	pthread_mutex_destroy(&(tp->worker_mutex));
	free(tp);
}
