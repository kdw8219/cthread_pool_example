#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "tpool.h"
#include "thread_header.h"

struct stParams_t {
	int t;
	int id;
};

typedef struct stParams_t stParams;

void work(void* args){
	stParams* params;
	params = (stParams*)args;
	printf("%d start\n", params->id);
	sleep(params->t);
	printf("%d end after %d sec\n", params->id, params->t);

	return ;
}

int main(int argc, char** argv) {
	if(argc != 3)
	{
		printf("Format error!\n");
		printf("FORMAT : <program> <count of thread> <test case>\n");
		printf("EXAMPLE: ./test_tp 2 10\n");
		return 0;
	}
	int thread_num, test_case, i;
	tpool_t* tp;
	stParams* params;
	
	thread_num = atoi(argv[1]);
	test_case = atoi(argv[2]);
	printf("1. thread_pool create\n");
	tp = tpool_create(thread_num);
	params = calloc(test_case, sizeof(stParams));

	for(i = 0; i < test_case; i++)
	{
		printf("2.%d Add tasks\n", i+1);
		params[i].t = i % 3 + 1;
		params[i].id = i;
		tpool_task_add(tp, (thread_func_t)&work, (void*)&(params[i]));
		sleep(1);
	}

	sleep(15);

	free(params);
	tpool_destroy(tp);
	printf("TEST END\n");
	return 0;
}
