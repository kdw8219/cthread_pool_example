#pragma once

#ifndef __THREAD_HEADER__
#define __THREAD_HEADER__

#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

typedef void (*thread_func_t)(void*);

struct tpool_task {
	thread_func_t	func;
	void	*args;
	struct tpool_task *next;
};

typedef struct tpool_task tpool_task_t;


struct tpool {
	bool _stop_threads; 
	size_t _num_threads;
	
	pthread_mutex_t worker_mutex;
	
	tpool_task_t* task_first;
	tpool_task_t* task_last;
};


#endif
