#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#define total 10000000

void *count(void *arg );	//Thread Function
unsigned int counter = 0;	//Shared Data
sem_t sem_c;			//Declaration of semaphore

int main()
{

	pthread_t t1;
	pthread_t t2;
	int status;
	
	// initialize semaphore (semaphore==1)
	sem_init(&sem_c,0,1);
	
	// Declaration of thread
	// Thread 1, 2 creation
	pthread_create(&t1, NULL, count, (void *)&t1);
	pthread_create(&t2, NULL, count, (void *)&t2);
	
	// Thread, Thread 2 join
	pthread_join(t1, (void *)&status);
	pthread_join(t2, (void *)&status);
	
	// Printing Shared Data after Operation
	printf("Counter : %d\n", counter);

	if (counter==total*2)
		printf("Correct\n");
	else printf("Fail\n");
	
	// Destroy semaphore
	sem_destroy(&sem_c);
}

void *count(void *arg)
{
	int i=0;

	// if semaphore==1, execute and semaphore=0
	// if not, wait until semaphore become 1
	sem_wait(&sem_c);

	while(i<total){
		counter++;
		i++;
		//printf("%d\n", counter);
	}
	sem_post(&sem_c);

}
