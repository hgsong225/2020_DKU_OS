/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32131113
*	    Student name : 송현규
*	    Student id : 32151439
*	    Student name : 남종우
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 */

void DumpTask(int currentTime, Task task[]) {
	printf("\nNow Tick : %d\n", currentTime);
	printf("%-10s %-10s %-10s %-10s %-10s %-10s\n", "PID", "BrustT", "ExecuteT", "ArrivalT", "Period", "TerminateT");
	for (int i = 0; i < TASK_NUM; i++) {
		printf("%-10d %-10d %-10d %-10d %-10d %-10d \n", task[i].id, task[i].burstTime, task[i].executeTime, task[i].arrivalTime, task[i].priority, task[i].terminateTime);
	}
}

int isTaskDone(Task task) {
	return task.burstTime == task.executeTime;
}

int isTaskAllDone(Task task[]) {
	for (int i = 0; i < TASK_NUM; i++) {
		if (!isTaskDone(task[i])) {
			return 0;
		}
	}
	return 1;
}

void RR(Task task[])
{
	int tick = 0;
	while (1)
	{
		int workIndex = -1;
		for (int i = 0; i < TASK_NUM; i++) {
			if (isTaskDone(task[i]) || task[i].arrivalTime > tick) {
				continue;
			}
			if (workIndex == -1) {
				workIndex = i;
				continue;
			}
			if (task[i]._lastBurstAt < task[workIndex]._lastBurstAt) {
				workIndex = i;
			}
		}

		if (workIndex > -1) {
			task[workIndex].executeTime += 1;
			task[workIndex]._lastBurstAt = tick;
			if (isTaskDone(task[workIndex])) {
				task[workIndex].terminateTime = tick;
			}
		}
		DumpTask(tick, task);
		tick = tick + 1;
		if (isTaskAllDone(task)) {
			break;
		}
	}
}

int pow(int n, int e) {
	int value = 1;
	for (int i = 0; i < e; i++) {
		value *= n;
	}
	return value;
}

void FeedBack(Task task[])
{
	Queue queue1[FEEDBACK_QUEUE_SIZE];
	Queue queue2[FEEDBACK_QUEUE_SIZE];

	for (int i = 0; i < FEEDBACK_QUEUE_SIZE; i++) {
		InitQueue(&queue1[i]);
		InitQueue(&queue2[i]);
	}

	int begin_tick = 0;
	int tick = 0;
	while (1)
	{
		int workIndex = -1;
		for (int i = 0; i < TASK_NUM; i++) {
			if (begin_tick <= task[i].arrivalTime && task[i].arrivalTime <= tick && !task[i]._isQueued) {
				EnQueue(&queue1[0], i);
				task[i]._isQueued = 1;
			}
		}
		begin_tick = tick;


		for (int i = 0; i < FEEDBACK_QUEUE_SIZE; i++) {
			int quota = pow(2, i + 1);

			while (!IsEmptyQueue(&queue1[i])) {
				int workIndex = DeQueue(&queue1[i]);
				int leftTime = task[workIndex].burstTime - task[workIndex].executeTime;
				int toBurstTime = (leftTime >= quota) ? quota : leftTime;
				task[workIndex].executeTime += toBurstTime;
				tick += toBurstTime;
				task[workIndex]._lastBurstAt = tick;
				if (isTaskDone(task[workIndex])) {
					task[workIndex].terminateTime = tick;
				}
				else {
					EnQueue(&queue2[i + 1], workIndex);
				}
				DumpTask(tick, task);
			}
		}

		for (int i = 0; i < FEEDBACK_QUEUE_SIZE; i++)
			while (!IsEmptyQueue(&queue2[i]))
				EnQueue(&queue1[i], DeQueue(&queue2[i]));
		if (isTaskAllDone(task)) {
			break;
		}
		if (begin_tick == tick) {
			tick += 1;
		}
	}
}

void InitQueue(Queue* queue)
{
	queue->top = 0;
}

int IsEmptyQueue(Queue* queue)
{
	return queue->top == 0;
}

void EnQueue(Queue* queue, int index)
{
	queue->data[queue->top++] = index;
}

int DeQueue(Queue* queue)
{
	int value = queue->data[0];
	for (int i = 0; i < queue->top - 1; i++) {
		queue->data[i] = queue->data[i + 1];
	}
	queue->top--;
	return value;
}


/* FCFS */
int findMax(int n, int targetArr[])
{
	int max = targetArr[0];
	for (int i = 1; i < n; i += 1)
	{
		if (targetArr[i - 1] < targetArr[i])
		{
			max = targetArr[i];
		}
	}

	return max;
}

// Function to find the waiting time for all 
// processes 
void findWaitingTime(int processes[], int n, int bt[], int wt[], int at[], int ct[]) 
{ 
    ct[0] = bt[0];
	// waiting time for first process is 0 
	wt[0] = 0; 
	
	// calculating waiting time 
	for (int i = 1; i < n ; i += 1)
    {
        ct[i] = ct[i - 1] + bt[i];
		wt[i] = ct[i] - at[i] - bt[i];
    
        if (wt[i] < 0) wt[i] = 0;
    }
} 
	
// Function to calculate turn around time 
void findTurnAroundTime(int processes[], int n, int bt[], int wt[], int tat[], int ct[], int at[]) 
{ 
	// calculating turnaround time by adding 
	// bt[i] + wt[i] 
	for (int i = 0; i < n ; i += 1) 
		tat[i] = ct[i] - at[i]; 
} 

//Function to calculate average time 
void FCFS_findavgTime(int processes[], int n, int bt[], int at[]) 
{ 
	int ct[n], wt[n], tat[n], total_wt = 0, total_tat = 0; 
	
	//Function to find waiting time of all processes 
	findWaitingTime(processes, n, bt, wt, at, ct); 
	
	//Function to find turn around time for all processes 
	findTurnAroundTime(processes, n, bt, wt, tat, ct, at); 
	
	//Display processes along with all details 
	printf("Processes\tArrival time\tBurst time\tCompletion time\t\tTurn around time\tWaiting time\n"); 
	
	// Calculate total waiting time and total turn 
	// around time 
	for (int i = 0; i < n; i += 1) 
	{ 
		total_wt = total_wt + wt[i]; 
		total_tat = total_tat + tat[i]; 
		printf("%d", processes[i]); 
		printf("\t\t%d ", at[i]); 
		printf("\t\t%d ", bt[i]); 
		printf("\t\t%d ", ct[i]); 
		printf("\t\t\t%d", tat[i]); 
		printf("\t\t\t%d\n", wt[i]); 
	} 
	int s = (float)total_wt / (float)n; 
	int t = (float)total_tat / (float)n; 
	printf("Average waiting time = %d", s); 
	printf("\n"); 
	printf("Average turn around time = %d ", t);
	printf("\n");

	int max;
	max = findMax(n, ct);
	printf("max: %d\n", max);

	for (int i = 0; i < max; i += 1) 
	{
		if (i % 5 == 0) printf("%d", i);
		printf("\t");
		if (i == max - 1) printf("%d", max);
	}
	printf("\n");
	for (int i = 0; i < n; i += 1) 
	{
		printf("%d", processes[i]);
		if (i == 0)
		{
			for (int j = 0; j < ct[0]; j += 1)
			{
				printf("@@@@@@\t");
			}

		}
		for (int j = 0; j < max + 1; j += 1)
		{
			if (j < ct[i] && j >= ct[i - 1]){
				printf("@@@@@@@");
			}
			printf("\t");
		}
		printf("\n");
	} 
} 