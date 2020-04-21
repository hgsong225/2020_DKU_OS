/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32131113
*	    Student name : 송현규
*	    Student id : 32151439
*	    Student name : 남종우
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H

#define TASK_NUM 6
#define FEEDBACK_QUEUE_SIZE 10

typedef struct Task {
	int id;
	int burstTime;
	int executeTime;
	int arrivalTime;
	int priority;

	int terminateTime;

	int _lastBurstAt;
	int _isQueued;
}Task;

typedef struct Queue {
	int data[100];
	int top;
}Queue;

void DumpTask(int currentTime, Task task[]);
int isTaskDone(Task task);
int isTaskAllDone(Task task[]);
void RR(Task task[]);
void FeedBack(Task task[]);

void InitQueue(Queue* queue);
int IsEmptyQueue(Queue* queue);
void EnQueue(Queue* queue, int index);
int  DeQueue(Queue* queue);

/* FCFS */
int findMax(int n, int targetArr[]);
void findWaitingTime(int processes[], int n, int bt[], int wt[], int at[], int ct[]); 
void findTurnAroundTime(int processes[], int n, int bt[], int wt[], int tat[], int ct[], int at[]); 
void FCFS_findavgTime(int processes[], int n, int bt[], int at[]); 

#endif /* LAB1_HEADER_H*/

