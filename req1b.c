/*	Author: James Jacobs
	Username: PSYJAJ
	Student Number: 4244791	
	Task: req1b
*/
#include <stdio.h>
#include <sys/time.h>

typedef int completedStatus;
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
void RR();
void printJob(int iId, int iBurstTime, int iRemainingTime, int iPriority);

void generateJobs()
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

void PQ()
{
	int i, j, w, x, y, z;
	printf("Sorting by priority\n");
	printf("JOBS: \n");		

	for (i=0; i<NUMBER_OF_JOBS; i++)
	{
		for (j = i + 1; j < NUMBER_OF_JOBS; j++)
		{
			if (aiJobs[i][PRIORITY] > aiJobs[j][PRIORITY])
			{

				w = aiJobs[i][JOB_INDEX];
				x = aiJobs[i][BURST_TIME];
				y = aiJobs[i][REMAINING_TIME];
				z = aiJobs[i][PRIORITY];
				aiJobs[i][JOB_INDEX] = aiJobs[j][JOB_INDEX];
				aiJobs[i][BURST_TIME] = aiJobs[j][BURST_TIME];
				aiJobs[i][REMAINING_TIME] = aiJobs[j][REMAINING_TIME];
				aiJobs[i][PRIORITY] = aiJobs[j][PRIORITY];
				aiJobs[j][JOB_INDEX] = w;
				aiJobs[j][BURST_TIME] = x;
				aiJobs[j][REMAINING_TIME] = y;
				aiJobs[j][PRIORITY] = z;

			}
		}
	printf("Id = %d, Burst Time = %d, Remaining Time = %d, Priority = %d\n", aiJobs[i][JOB_INDEX], aiJobs[i][BURST_TIME], aiJobs[i][REMAINING_TIME], aiJobs[i][PRIORITY]);
	}
	//the code above is a bubblesort I have implemented to sort the whole array by the priorities
	//With the highest priority first and the lowest being displayed last
}

void RR()
{
	printf("\n");
	printf("ROUND ROBIN\n");
	printf("time slice: 25\n\n");
	
	int tStart = 0;
	int tEnd=0;
	int i = 0;
	int completedStatus = TRUE; 
	int jobCounter = 0;
	int ATT;
	int count =0;
	int count1 =0;
	int ART;
	int startTimeAverage = 0, endTimeAverage = 0;
	
	printf("Round Robin\n");
	
	for(i = 0; i < NUMBER_OF_JOBS; i++)
	//a for loop to count through all the jobs that have been sorted in the priority queue
	{
		if(aiJobs[i][REMAINING_TIME] >= TIME_OF_GAP)
		/*This if statement catches all the jobs which have a REMAINING_TIME value greater than or equal to that
		of the time slice which is 25 milliseconds.
		This is necessary as a job with a BURST_TIME greater than that would need to be postponed and will loop 
		within it's same priority band as is needed in RR, until all jobs are completed*/
		{
			if(aiJobs[i][PRIORITY] != aiJobs[i+1][PRIORITY] && aiJobs[i][PRIORITY] != aiJobs[i-1][PRIORITY])
			/*if the job has a unique priority value it will fall into this if statement, the reason for this is 
			that it won't have to loop if the BURST_TIME value is greater than the TIME_OF_GAP. The state
			'completed' will be set to true*/
			{
				simulateJob(TIME_OF_GAP);
				tStart = tEnd;
				tEnd = tStart + aiJobs[i][BURST_TIME];
				completedStatus = TRUE;
				aiJobs[i][REMAINING_TIME] = 0;
				printf("JOB ID: %d, Start time = %d, End time = %d, Priority = %d, Remaining Time = %d\n",aiJobs[i][JOB_INDEX], tStart, tEnd, aiJobs[i][PRIORITY], aiJobs[i][REMAINING_TIME]);
				count++;
			}
			else
			//if statements checks the right information is being displayed, the state 'completed' is
			//set to false for these jobs as they will have to loop again
			{
				simulateJob(TIME_OF_GAP);
				if(aiJobs[i][REMAINING_TIME] != aiJobs[i][BURST_TIME])
				{	
					tStart = tEnd;
					tEnd = tStart + TIME_OF_GAP;
					aiJobs[i][REMAINING_TIME] = aiJobs[i][REMAINING_TIME] - TIME_OF_GAP;
					completedStatus = FALSE;
					printf("JOB ID: %d, restart time = %d, End time = %d, Priority = %d, Remaining Time = %d\n",aiJobs[i][JOB_INDEX], tStart, tEnd, aiJobs[i][PRIORITY], aiJobs[i][REMAINING_TIME]);
					count++;
				}
				else
				//if the PRIORITY is not unique and the jobs are the first in their priority loop it will just
				//display as the start time.
				{
					tStart = tEnd;
					tEnd = tStart + TIME_OF_GAP;
					aiJobs[i][REMAINING_TIME] = aiJobs[i][REMAINING_TIME] - TIME_OF_GAP;
					completedStatus = FALSE;
					printf("JOB ID: %d, Start time = %d, End time = %d, Priority = %d, Remaining Time = %d\n",aiJobs[i][JOB_INDEX], tStart, tEnd, aiJobs[i][PRIORITY], aiJobs[i][REMAINING_TIME]);
					count++;
				}
			}
			
		}
		else if(aiJobs[i][REMAINING_TIME] > 0)
		/*if the REMAINING_TIME or BURST_TIME is less than the time of the TIME_OF_GAP, the job will be caught
		by these if statement, to make sure the right information is displayed
		it also ensures in the loop that the job won't be run again if its REMAINING_TIME is 0*/
		{
			simulateJob(TIME_OF_GAP);
			if(aiJobs[i][REMAINING_TIME] != aiJobs[i][BURST_TIME])
			{
				tStart = tEnd ;
				tEnd = tStart + aiJobs[i][REMAINING_TIME];
				aiJobs[i][REMAINING_TIME] = 0;
				completedStatus = TRUE;
				printf("JOB ID: %d, restart time = %d, End time = %d, Priority = %d, Remaining Time = %d\n",aiJobs[i][JOB_INDEX], tStart, tEnd, aiJobs[i][PRIORITY], aiJobs[i][REMAINING_TIME]);
				count++;
			}
			else
			{
				tStart = tEnd ;
				tEnd = tStart + aiJobs[i][REMAINING_TIME];
				aiJobs[i][REMAINING_TIME] = 0;
				completedStatus = TRUE;
				printf("JOB ID: %d, Start time = %d, End time = %d, Priority = %d, Remaining Time = %d\n",aiJobs[i][JOB_INDEX], tStart, tEnd, aiJobs[i][PRIORITY], aiJobs[i][REMAINING_TIME]);
				count++;
			}
			
		}
		if(aiJobs[i][PRIORITY] != aiJobs[i+1][PRIORITY])
		/*this if statement is used to make sure the jobs that have not reached a REMAINING_TIME of 0 and
		have the same priority, but which isn't the same as the priority of the next top loop until all
		the jobs have a remaining time of 0*/
		{
			if(completedStatus == TRUE)
			{
				jobCounter = i + 1;
			}
			else
			{
				i = jobCounter - 1;
				completedStatus = TRUE;
			}
		}
	
		startTimeAverage = startTimeAverage + tStart;
		endTimeAverage = endTimeAverage + tEnd;
	}
	ATT = startTimeAverage/ count;
	ART = endTimeAverage / count ;
	printf("\nATT is = %d\n", ATT);
	printf("ART is = %d\n\n", ART);
	//code to display the ART and ATT
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
	PQ();
	RR();
	return 0;
}

