#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#define total 10000000

void *count(void *arg );	//Thread Function
unsigned int counter = 0;	//Shared Data

// Declaration of Mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{

	pthread_t t1;
	pthread_t t2;
	int status;
	
	// Thread 1, 2 creation
	pthread_create(&t1, NULL, count, (void *)&t1);
	pthread_create(&t2, NULL, count, (void *)&t2);
	
	// Thread 1, Thread 2 join
	pthread_join(t1, (void *)&status);
	pthread_join(t2, (void *)&status);

	//7. Printing Shared Data after Operation
	printf("Counter : %d\n", counter);
	
	if (counter==total*2)
		printf("Correct\n");
	else printf("Fail\n");

	pthread_mutex_destroy(&mutex);
}

void *count(void *arg)
{
	int i=0;
	while(i<total){

		// lock mutex and other thread have to wait
		pthread_mutex_lock(&mutex);

		counter++;
		i++;		
		
		//unlock mutex and other thread can access
		pthread_mutex_unlock(&mutex);
	}
}
