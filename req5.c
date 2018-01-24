/*	Author: James Jacobs
	Username: PSYJAJ
	Student Number: 4244791	
	Task: req5
*/
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUMBER_OF_JOBS 1000
#define BUFFER_SIZE 10
#define NUMBER_OF_CONSUMERS 4
#define JOB_INDEX 0
#define BURST_TIME 1
#define REMAINING_TIME 2
#define PRIORITY 3

void * producer(void * p);
void * consumer(void * p);
void simulateJob(int iTime);
char buffer[BUFFER_SIZE][4];

int temp = 0;
int jobsremoved = 0;
int NumOfItems = 0;
int jobsinbuffer=0;
int stcount=0;
int iStartTime;
int ast = 0;
int i = 0;
int done = 0;
int syncValue, delayValue;
int counter = 0;

sem_t delay_consumer;
sem_t sync;

pthread_t cID[NUMBER_OF_CONSUMERS];
pthread_t prod;

void printJobP(int iId, int iBurstTime, int iRemainingTime, int iPriority)
//print function make sure the right information is printed for the producer
{
	printf("Producing: JOB Id = %d, Burst Time = %d, Remaining Time = %d (Jobs Produced = %d, Jobs in buffer = %d)\n", 
	iId, iBurstTime, iRemainingTime, counter+1, NumOfItems+1);
}
void printJobC(int thread1, int iId, int iBurstTime, int iStartTime, int iPriority)
//print function make sure the right information is printed for the consumer
{
	printf("Thread %d removes: JOB Id = %d, Burst Time = %d, Start time = %d (Jobs removed = %d, Jobs in buffer = %d)\n", 
	thread1, iId, iBurstTime, iStartTime, jobsremoved+1, jobsinbuffer-1);
}

void FCFS(int i)
//the FCFS function takes in a value from a loop, which effectively moves all the values down the buffer one at a time up 
//to that value. It just reassigns the cells to the values.
{	
	buffer[i-1][JOB_INDEX] = buffer[i][JOB_INDEX];
	buffer[i-1][BURST_TIME] = buffer[i][BURST_TIME];
	buffer[i-1][REMAINING_TIME] = buffer[i][REMAINING_TIME];
	buffer[i-1][PRIORITY] = buffer[i][PRIORITY];
}

void * consumer(void * p)
{	
	simulateJob(25);
	int thread1 = pthread_self();
	int iStartTime = 0;
	sem_wait(&delay_consumer);
	while(1)
	{	
		sem_wait(&sync);
		temp = NumOfItems-1;
		printJobC(thread1, buffer[0][JOB_INDEX], buffer[0][BURST_TIME], iStartTime, buffer[0][PRIORITY]);
		//prints the consumers jobs
		jobsremoved++;
		jobsinbuffer--;
		//increments and decrements counters
		iStartTime = iStartTime + buffer[0][BURST_TIME] ;
		for(i=1; i<BUFFER_SIZE; i++)
		{
			FCFS(i);
			//this functions moves everything in the array forward
		}
		if(NumOfItems != 0)
		/*this if statement checks to see if the NumOfItems is equal to the size of the buffer, if it is, the last element
		in the array is set to equal NULL as all the other elements have been moved forward and there's nothing at the
		end. This allows the producer to add more values to the end of the buffer.*/
		{
			buffer[BUFFER_SIZE-1][JOB_INDEX] = '\0';
			buffer[BUFFER_SIZE-1][BURST_TIME] = '\0';
			buffer[BUFFER_SIZE-1][REMAINING_TIME] = '\0';
			buffer[BUFFER_SIZE-1][PRIORITY] = '\0';
		}
		NumOfItems--;
		if (done == 1 && temp == 0)
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
			sem_wait(&delay_consumer);
		}
		stcount=stcount+iStartTime;
		ast = stcount/NUMBER_OF_JOBS;
	}
	
}

void * producer(void * p)
{
	int temp = NUMBER_OF_JOBS;
	while(done == 0)
	{
		sem_wait(&sync);
		if(buffer[BUFFER_SIZE - 1][JOB_INDEX] == '\0' && NumOfItems <= BUFFER_SIZE)
		/*This if statement checks if the last element in the array is empty, if it is, a new row in for the buffer is 
		generated and put into the buffer at the position which is next in line to be filled. It then increments a new
		counter which tracks how many jobs are in the buffer and displays the correct information.*/
		{
			buffer[NumOfItems][JOB_INDEX] = counter;
			buffer[NumOfItems][BURST_TIME] = rand() % 99 + 1;
			buffer[NumOfItems][REMAINING_TIME] = buffer[NumOfItems][BURST_TIME];
			buffer[NumOfItems][PRIORITY] = rand()%10;
			printJobP(buffer[NumOfItems][JOB_INDEX], buffer[NumOfItems][BURST_TIME], buffer[NumOfItems][REMAINING_TIME], buffer[NumOfItems][PRIORITY]);
			NumOfItems++;
			temp--;
			counter++;
			jobsinbuffer++;
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

long int getDifferenceInMilliSeconds(struct timeval start, struct timeval end)
{
	int iSeconds = end.tv_sec - start.tv_sec;
	int iUSeconds = end.tv_usec - start.tv_usec;
 	long int mtime = (iSeconds * 1000 + iUSeconds / 1000.0);
	return mtime;
}

void simulateJob(int iTime)
{
	long int iDifference = 0;
	struct timeval startTime, currentTime;
	gettimeofday(&startTime, NULL);
	do
	{	
		gettimeofday(&currentTime, NULL);
		iDifference = getDifferenceInMilliSeconds(startTime, currentTime);
	} while(iDifference < iTime);
}
	
int main()
{
	sem_init(&sync, 0, 1);
	sem_init(&delay_consumer, 0, 0);
	
	pthread_create(&prod, NULL, producer, NULL);
	for(i=0; i<NUMBER_OF_CONSUMERS; i++)
	{
		pthread_create(&cID[i], NULL, consumer, NULL);
	}
	
	pthread_join(prod, NULL);
	
	sem_getvalue(&delay_consumer, &delayValue);
	sem_getvalue(&sync, &syncValue);
	printf("sSync = %d, sDelay = %d, Average start time = %d\n\n", syncValue, delayValue, ast);
	
	return 0;
}