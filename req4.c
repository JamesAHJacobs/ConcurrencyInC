/*	Author: James Jacobs
	Username: PSYJAJ
	Student Number: 4244791	
	Task: req4
*/
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUMBER_OF_JOBS 1000
#define BUFFER_SIZE 10
#define NUMBER_OF_CONSUMERS 3
//define the number of consumers

void * producer(void * p);
void * consumer(void * p);
void print();
char buffer[BUFFER_SIZE];

int temp = 0;
int NumOfItems = 0;
int i = 0;
int end = 0;
int syncValue, delayValue;

sem_t delay_consumer;
sem_t sync;

pthread_t cID[NUMBER_OF_CONSUMERS];
pthread_t prod;
//initialise an array of consumer threads of fixed size and the producer thread

void print()
{
	printf("iIndex = %d, id = %d\t", NumOfItems, pthread_self()); 
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
		print(NumOfItems);
		temp = NumOfItems;
		if(end == 1 && temp == 0)
		/*In order to have multiple consumers, this if statement checks that the number
		of items left to create is 0, and if so it joins all the consumers together so
		they join properly with the main thread and no deadlock occurs*/ 
		{
			sem_post(&sync);
			for(i=0; i<NUMBER_OF_CONSUMERS; i++)
			{
				pthread_join(cID[i], NULL);
			}
		}
		sem_post(&sync);
		if(temp == 0)
		{
			if(end == 1)
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
	while(end == 0)
	{
		sem_wait(&sync);
		if(buffer[NumOfItems] != '*' && NumOfItems < BUFFER_SIZE)
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
			end = 1;
		}
	}
}

int main()
{
	sem_init(&sync, 0, 1);
	sem_init(&delay_consumer, 0, 0);
	
	pthread_create(&prod, NULL, producer, NULL);
	
	for(i=0; i<NUMBER_OF_CONSUMERS; i++)
	//for loop to populate the array of fixed number of consumers
	{
		pthread_create(&cID[i], NULL, consumer, NULL);
	}
	pthread_join(prod, NULL);
	sem_getvalue(&delay_consumer, &delayValue);
	sem_getvalue(&sync, &syncValue);
	printf("\nsSync = %d, sDelay = %d\n\n", syncValue, delayValue);
	return 0;
}