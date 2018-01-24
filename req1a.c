/*	Author: James Jacobs
	Username: PSYJAJ
	Student Number: 4244791	
	Task: req1a
*/
#include <stdio.h>
#include <sys/time.h>

typedef int completed;
#define TRUE 1
#define FALSE 0

#define NUMBER_OF_JOBS 1000
#define JOB_INDEX 0
#define BURST_TIME 1
#define REMAINING_TIME 2
#define PRIORITY 3
#define TIME_OF_GAP 25

int aiJobs[NUMBER_OF_JOBS][4];
void simulateJob(int iTime);
void printJob(int iId, int iBurstTime, int iRemainingTime, int iPriority);

void generateJobs()
// provided code
{
	int i;
	for(i = 0; i < NUMBER_OF_JOBS;i++)
	{
		aiJobs[i][JOB_INDEX] = i;
		aiJobs[i][BURST_TIME] = rand() % 99 + 1;
		aiJobs[i][REMAINING_TIME] = aiJobs[i][BURST_TIME];
		aiJobs[i][PRIORITY] = rand()%10;
	}
}


void FCFS()
{
	int time;
	int iStartTime = 0;
	int iEndTime;
	int j;
	int ATT;
	int count =0;
	int count1 =0;
	int ART;
	
	for(j = 0; j<NUMBER_OF_JOBS;j++)
	{	
		simulateJob(TIME_OF_GAP);
		//simulated have a 25 millisecond time gap between each job
		iStartTime = iStartTime + aiJobs[j-1][BURST_TIME] ;
		iEndTime = iStartTime + aiJobs[j][BURST_TIME];
		//this code sets the start and end times for each job
		//the start time, is the start time of the current job plus the burst time of the previous one
		//the end time is the start time of the current job plus its burst time  
		
		count = count + iEndTime;
		count1 = count1 +iStartTime;
		//counters to work out the ATT and ART
		
		printf("JOB ID = %d, START Time = %d, END Time = %d\n", j, iStartTime, iEndTime);
	}
	
	ATT = count / NUMBER_OF_JOBS;
	ART = count1 / NUMBER_OF_JOBS ;
	printf("\nATT is = %d\n", ATT);
	printf("ART is = %d\n\n", ART);
	//code to work out and print ART and ATT
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
		printJob(aiJobs[i][JOB_INDEX], aiJobs[i][BURST_TIME], aiJobs[i][REMAINING_TIME], aiJobs[i][PRIORITY]);
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
	generateJobs();
	printJobs();
	FCFS();
	return 0;
}

