/*	Author: James Jacobs
	Username: PSYJAJ
	Student Number: 4244791	
	Task: req3
*/
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUMBER_OF_JOBS 1000
#define BUFFER_SIZE 100

void * producer(void * p);
void * consumer(void * p);
void print();
char buffer[BUFFER_SIZE];
//a character buffer of fixed size BUFFER_SIZE is initialised

int i=0;
int temp = 0;
int NumOfItems = 0;
int done = 0;
int syncValue, delayValue;

sem_t delay_consumer;
sem_t sync;

void print()
//this print functions loops through the buffer and prints each element
{
	printf("iIndex = %d\t", NumOfItems); 
	for(i=0; i<NumOfItems; i++)
	{
		printf("%c",buffer[i]);
	}
	printf("\n");
}

void * consumer(void * p)
{
	sem_wait(&delay_consumer);
	while(1)
	{
		sem_wait(&sync);
		NumOfItems--;
		buffer[NumOfItems] = '\0';
		/*when the consumer 'removes a job' it just replaces the value at that position
		in the array with the NULL character '\0' which allows the producer to add more
		elements to the buffer
		This is a FIFO buffer so the NumOfItems is decrements and the asterisk at that 
		position is removed*/		
		print(NumOfItems);
		temp = NumOfItems;
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
{
	int temp = NUMBER_OF_JOBS;
	while(done == 0)
	{
		sem_wait(&sync);
		if(buffer[NumOfItems] != '*' && NumOfItems < BUFFER_SIZE)
		/*Instead of just adding a value to an unbounded buffer here this if statement
		need to check that there is room in the buffer to add a new asterisk, so here
		it check that there is no asterisk in the current position and checks that the
		NumOfItems is not greater than the BUFFER_SIZE*/
		{
			buffer[NumOfItems] = '*';
			NumOfItems++;
			print(NumOfItems);
			temp--;
		}
		if(NumOfItems == 1)
		{
			sem_post(&delay_consumer);
		}
		sem_post(&sync);
		if(temp ==0)
		{
			done = 1;
		}
	}
}

int main()
{
	sem_init(&sync, 0, 1);
	sem_init(&delay_consumer, 0, 0);
	
	pthread_t cons;
	pthread_t prod;
	
	pthread_create(&prod, NULL, producer, NULL);
	pthread_create(&cons, NULL, consumer, NULL);
	
	pthread_join(cons, NULL);
	pthread_join(prod, NULL);
	
	sem_getvalue(&delay_consumer, &delayValue);
	sem_getvalue(&sync, &syncValue);
	
	printf("sSync = %d, sDelay = %d\n\n", syncValue, delayValue);
	
	return 0;
}