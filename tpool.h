#pragma once
#ifndef __TPOOL_H_
#define __TPOOL_H_

#include "thread_header.h"

//1. make thread pool
//2. make task queue

#define DEFAULT_THREAD_COUNT	1

typedef struct tpool tpool_t;

typedef void (*thread_func_t)(void *arg);

tpool_t *tpool_create(size_t num);
void tpool_destroy(tpool_t *tm);

bool tpool_task_add(tpool_t* tp, thread_func_t func, void* args); //add work to queue
void tpool_wait(tpool_t *tm);

tpool_task_t * tpool_task_create(thread_func_t func, void* args);

void tpool_task_destroy(tpool_task_t* task);

tpool_task_t* tpool_task_get(tpool_t* tp);

void worker();

#endif
