/*	Author: James Jacobs
	Username: PSYJAJ
	Student Number: 4244791	
	Task: req2
*/
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUMBER_OF_JOBS 1000

void * producer(void * p);
void * consumer(void * p);
void print();

int temp = 0;
int NumOfItems = 0;
int done = 0;
int syncValue, delayValue;
//define variables

sem_t delay_consumer;
sem_t sync;
//define two semaphores

void print()
//code to print number of items in the 'unbounded buffer'
{
	printf("iIndex = %d\n", NumOfItems); 
	
}

void * consumer(void * p)
/*The consumer decrements the value inside the NumOfItems variable as it takes items from the buffer.
The temp variable is where the NumOfItems value is stored while inside the critical section and
an if statement is needed to break out of the loop when the thread is done.*/
{
	sem_wait(&delay_consumer);
	while(1)
	{
		sem_wait(&sync);
		NumOfItems--;
		temp = NumOfItems;
		print(NumOfItems);
		sem_post(&sync);
		if(temp == 0)
		{
			if(done == 1)
			{
				break;
			}
			sem_wait(&delay_consumer);
		}
	}
}

void * producer(void * p)
/*Inside the variable temp, the NUMBER_OF_JOBS is stored, this allows the producer to
run as many times (equal to NUMBER_OF_JOBS) until it ends as there is an unbounded buffer */
{
	int temp = NUMBER_OF_JOBS;
	while(done == 0)
	{
		sem_wait(&sync);
		NumOfItems++;
		temp--;
		print(NumOfItems);
		if(NumOfItems == 1)
		//when the NumOfItems equals 1 the delay_consumer semaphore is incremented
		//which wakes up the consumer
		{
			sem_post(&delay_consumer);
		}
		sem_post(&sync); //always runs
		if(temp ==0)
		//once the number of jobs left to be run is 0, the state 'done' is set to
		//1 which allows the consumer to break the loop
		{
			done = 1;
		}
	}
}

int main()
{
	sem_init(&sync, 0, 1);
	sem_init(&delay_consumer, 0, 0);
	//the initial values of the semaphores are set
	
	pthread_t cons;
	pthread_t prod;
	//the consumer and producer threads are initialised
	
	pthread_create(&prod, NULL, producer, NULL);
	pthread_create(&cons, NULL, consumer, NULL);
	//the consumer and producer threads are created
	
	pthread_join(cons, NULL);
	pthread_join(prod, NULL);
	//the two threads are joined to the main thread 
	
	sem_getvalue(&delay_consumer, &delayValue);
	sem_getvalue(&sync, &syncValue);
	//these functions get the values of the semaphores so they can be displayed
	
	printf("sSync = %d, sDelay = %d\n\n", syncValue, delayValue);
	
	return 0;
}