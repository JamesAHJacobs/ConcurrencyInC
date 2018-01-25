void RR()
{
	printf("\n");
	printf("ROUND ROBIN\n");
	printf("time slice: 25\n\n");
	
	int tStart = 0, tEnd=0, i = 0, counter = 0, int count =0;
	int startTimeAverage = 0, endTimeAverage = 0;
	int completed = TRUE, ATT, ART;
	
	printf("Round Robin\n");
	for(i = 0; i < NUMBER_OF_JOBS; i++)
	{
		if(aiJobs[i][REMAINING_TIME] >= TIME_OF_GAP)
		{
			tStart = tEnd;
			tEnd = tStart + aiJobs[i][BURST_TIME];
			completed = TRUE;
			aiJobs[i][REMAINING_TIME] = 0;
			printf("JOB ID: %d, Start time = %d, End time = %d, Priority = %d, Remaining Time = %d\n",aiJobs[i][JOB_INDEX], tStart, tEnd, aiJobs[i][PRIORITY], aiJobs[i][REMAINING_TIME]);
			count++;
		}
		else if(aiJobs[i][REMAINING_TIME] > 0)
		{
			tStart = tEnd ;
			tEnd = tStart + aiJobs[i][REMAINING_TIME];
			aiJobs[i][REMAINING_TIME] = 0;
			completed = TRUE;
			printf("JOB ID: %d, restart time = %d, End time = %d, Priority = %d, Remaining Time = %d\n",aiJobs[i][JOB_INDEX], tStart, tEnd, aiJobs[i][PRIORITY], aiJobs[i][REMAINING_TIME]);
			count++;
			
		}
		if(aiJobs[i][PRIORITY] != aiJobs[i+1][PRIORITY])
		{
			if(completed == TRUE)
			{
				counter = i + 1;
			}
			else
			{
				i = counter - 1;
			}
		}
	
		startTimeAverage = startTimeAverage + tStart;
		endTimeAverage = endTimeAverage + tEnd;
	}
	ATT = startTimeAverage/ count;
	ART = endTimeAverage / count ;
	printf("start times = %d", startTimeAverage);
	printf("count = %d", count);
	printf("\nATT is = %d\n", ATT);
	printf("ART is = %d\n\n", ART);
	
}