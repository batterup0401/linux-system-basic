#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_TRY 100000000

int shared_value = 0;
pthread_mutex_t mutex;

void *child_thread_main(void *arg)
{
	unsigned int i;
	
	printf("main-> child pthread id: %lu\n", pthread_self());

	for (i=0; i<MAX_TRY; i++)
	{
		pthread_mutex_lock(&mutex);
		shared_value++;
		pthread_mutex_unlock(&mutex);
	}

	for (i=0; i<MAX_TRY; i++)
	{
		pthread_mutex_lock(&mutex);
		shared_value--;
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

int main(void)
{
	pthread_t child_threads[2];

	pthread_mutex_init(&mutex, NULL);

	printf("main pthread id: %lu\n", pthread_self());

	pthread_create(&child_threads[0], NULL, child_thread_main, NULL);
	pthread_create(&child_threads[1], NULL, child_thread_main, NULL);

	printf("waiting to join thread[id: %lu]\n", child_threads[0]);
	pthread_join(child_threads[0], NULL);
	
	printf("waiting to join thread[id: %lu]\n", child_threads[1]);
	pthread_join(child_threads[1], NULL);
	
	printf("shared value: %d\n", shared_value);
	printf("Done.\n");

	return 0;
}
