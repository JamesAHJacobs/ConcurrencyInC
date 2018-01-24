/*	Author: James Jacobs
	Username: PSYJAJ
	Student Number: 4244791	
	Task: req6
*/
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef int completed;
#define TRUE 1
#define FALSE 0

#define NUMBER_OF_JOBS 100
#define BUFFER_SIZE 10
#define NUMBER_OF_CONSUMERS 2
#define JOB_INDEX 0
#define BURST_TIME 1
#define REMAINING_TIME 2
#define PRIORITY 3
#define TIME_OF_GAP 25

void * producer(void * p);
void * consumer(void * p);
void PQ();
void RR();
void simulateJob(int iTime);
char buffer[BUFFER_SIZE][4];

int thread1;
int temp = 0;
int jobsremoved = 0;
int NumOfItems = 0;
int jobsinbuffer=0;
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
	printf("Producing: JOB Id = %d, Burst Time = %d, Remaining Time = %d, Priority = %d (Jobs Produced = %d, Jobs in buffer = %d)\n", 
	iId, iBurstTime, iRemainingTime, iPriority, counter+1, NumOfItems+1);
}
void printJobC(int thread1, int iId, int iBurstTime, int iRemainingTime, int iPriority)
//print function make sure the right information is printed for the consumer
{
	printf("Thread %d removes: JOB Id = %d, Burst Time = %d, Start time = %d, Priority = %d (Jobs removed = %d, Jobs in buffer = %d)\n", 
	thread1, iId, iBurstTime, 0, iPriority, jobsremoved+1, jobsinbuffer-1);
}

void PQ()
{
	int i, j, w, x, y, z;	

	for (i=0; i<BUFFER_SIZE; i++)
	{
		for (j = i + 1; j < BUFFER_SIZE; j++)
		{
			if (buffer[i][PRIORITY] > buffer[j][PRIORITY])
			{

				x = buffer[i][0];
				w = buffer[i][1];
				y = buffer[i][2];
				z = buffer[i][3];
				buffer[i][JOB_INDEX] = buffer[j][JOB_INDEX];
				buffer[i][BURST_TIME] = buffer[j][BURST_TIME];
				buffer[i][REMAINING_TIME] = buffer[j][REMAINING_TIME];
				buffer[i][PRIORITY] = buffer[j][PRIORITY];
				buffer[j][JOB_INDEX] = x;
				buffer[j][BURST_TIME] = w;
				buffer[j][REMAINING_TIME] = y;
				buffer[j][PRIORITY] = z;

			}
		}
	}
	RR();
}

void RR()
{
	int tStart = 0;
	int tEnd=0;
	int i = 0;
	int completed = TRUE; 
	int counter = 0; 
	int timesrun = 0;
	int ATT;
	int count =0;
	int count1 =0;
	int ART;
	int startTimeAverage = 0, endTimeAverage = 0;
	for(i = 0; i < NUMBER_OF_JOBS; i++)
	{
		if(buffer[i][REMAINING_TIME] >= TIME_OF_GAP)
		{
			if(buffer[i][PRIORITY] != buffer[i+1][PRIORITY] && buffer[i][PRIORITY] != buffer[i-1][PRIORITY])
			{
				tStart = tEnd;
				tEnd = tStart + buffer[i][BURST_TIME];
				completed = TRUE;
				buffer[i][REMAINING_TIME] = 0;
				printf("Thread %d removes: JOB ID: %d, Start time = %d, End time = %d, Priority = %d, Remaining Time = %d\n",thread1, buffer[i][JOB_INDEX], tStart, tEnd, buffer[i][PRIORITY], buffer[i][REMAINING_TIME]);
				count++;
			}
			else
			{
				if(buffer[i][REMAINING_TIME] != buffer[i][BURST_TIME])
				{	
					tStart = tEnd;
					tEnd = tStart + TIME_OF_GAP;
					buffer[i][REMAINING_TIME] = buffer[i][REMAINING_TIME] - TIME_OF_GAP;
					completed = FALSE;
					printf("Thread %d removes: JOB ID: %d, restart time = %d, End time = %d, Priority = %d, Remaining Time = %d\n",thread1, buffer[i][JOB_INDEX], tStart, tEnd, buffer[i][PRIORITY], buffer[i][REMAINING_TIME]);
					count++;
				}
				else
				{
					tStart = tEnd;
					tEnd = tStart + TIME_OF_GAP;
					buffer[i][REMAINING_TIME] = buffer[i][REMAINING_TIME] - TIME_OF_GAP;
					completed = FALSE;
					printf("Thread %d removes: JOB ID: %d, Start time = %d, End time = %d, Priority = %d, Remaining Time = %d\n",thread1, buffer[i][JOB_INDEX], tStart, tEnd, buffer[i][PRIORITY], buffer[i][REMAINING_TIME]);
					count++;
				}
			}
			
		}
		else if(buffer[i][REMAINING_TIME] > 0)
		{
			if(buffer[i][REMAINING_TIME] != buffer[i][BURST_TIME])
			{
				tStart = tEnd ;
				tEnd = tStart + buffer[i][REMAINING_TIME];
				buffer[i][REMAINING_TIME] = 0;
				completed = TRUE;
				printf("Thread %d removes: JOB ID: %d, restart time = %d, End time = %d, Priority = %d, Remaining Time = %d\n",thread1, buffer[i][JOB_INDEX], tStart, tEnd, buffer[i][PRIORITY], buffer[i][REMAINING_TIME]);
				count++;
			}
			else
			{
				tStart = tEnd ;
				tEnd = tStart + buffer[i][REMAINING_TIME];
				buffer[i][REMAINING_TIME] = 0;
				completed = TRUE;
				printf("Thread %d removes: JOB ID: %d, Start time = %d, End time = %d, Priority = %d, Remaining Time = %d\n",thread1, buffer[i][JOB_INDEX], tStart, tEnd, buffer[i][PRIORITY], buffer[i][REMAINING_TIME]);
				count++;
			}
			
		}
		if(buffer[i][PRIORITY] != buffer[i+1][PRIORITY])
		{
			if(completed == TRUE)
			{
				counter = i + 1;
			}
			else
			{
				i = counter - 1;
				completed = TRUE;
			}
		}
	
		startTimeAverage = startTimeAverage + tStart;
		endTimeAverage = endTimeAverage + tEnd;
	}
}

void * consumer(void * p)
{
	simulateJob(25);
	sem_wait(&delay_consumer);
	while(1)
	{	
		sem_wait(&sync);
		PQ();
		int thread1 = pthread_self();
		for(i=0; i<BUFFER_SIZE; i++)
		{
			jobsremoved++;
			//after consuming all the elements, this makes every value equal to null ready to be 
			//re-populated by the producer
			buffer[i][JOB_INDEX] = '\0';
			buffer[i][BURST_TIME] = '\0';
			buffer[i][REMAINING_TIME] = '\0';
			buffer[i][PRIORITY] = '\0';
		}
		jobsinbuffer--;
		NumOfItems--;
		temp = NumOfItems;
		if(done == 1 && NumOfItems == 0)
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
		sem_post(&sync);
		if(NumOfItems == 1)
		{
			sem_post(&delay_consumer);
		}
		if(temp ==0)
		{
			done = 1;
		}
	}
}

void printJob(int iId, int iBurstTime, int iRemainingTime, int iPriority)
{
	printf("Id = %d, Burst Time = %d, Remaining Time = %d, Priority = %d\n", iId, iBurstTime, iRemainingTime, iPriority);
}

void printJobs()
{
	int i;
	printf("JOBS: \n");
	for(i = 0; i < NUMBER_OF_JOBS; i++)
		printJob(buffer[i][JOB_INDEX], buffer[i][BURST_TIME], buffer[i][REMAINING_TIME], buffer[i][PRIORITY]);
	printf("\n");
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
	printf("sSync = %d, sDelay = %d\n\n", syncValue, delayValue);
	return 0;
}