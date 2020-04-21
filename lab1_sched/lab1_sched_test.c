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
*       - Must contains scueduler algorithm test code.
*
*/

// #include <aio.h>
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
// #include <pthread.h>
// #include <asm/unistd.h>

#include "./include/lab1_sched_types.h"

/*
 * you need to implement scheduler simlator test code.
 *
 */


int main(int argc, char* argv[]) {
	//process id's 
	int processes[] = {1, 2, 3, 4, 5}; 
	int n = sizeof processes / sizeof processes[0]; 
	
	//Burst time of all processes 
	int burst_time[] = {5, 3, 8, 6, 2}; 
    
    // Arrival time of all processes 
    int arrival_time[] = {0, 1, 2, 3, 4}; 
	
    // FCFS scheduling
	printf("FCFS Scheduling>>>>>>>>>>>>>>>>>>>>>");
	printf("\n");
	FCFS_findavgTime(processes, n, burst_time, arrival_time);

	Task task[TASK_NUM] = {
		{1, 5, 0, 0, 0, -1, -1, 0},
		{2, 3, 0, 4, 0, -1, -1, 0},
		{3, 9, 0, 6, 0, -1, -1, 0},
		{4, 15, 0, 8,0, -1, -1, 0},
		{5, 3, 0, 8, 0, -1, -1, 0},
		{6, 5, 0, 10, 0, -1, -1, 0},
	};

	RR(task);
	HRRN(task);
	FeedBack(task);

	// Period 4, 6, 12 of T1, T2, T3 repeat
	Task task2[TASK_NUM] = {
		{1, 1, 0, 0, 10, -1, -1, 0},
		{2, 2, 0, 0, 5, -1, -1, 0},
		{3, 3, 0, 0, 1, -1, -1, 0},
		{4, 1, 0, 4, 10, -1, -1, 0},
		{5, 2, 0, 6, 5, -1, -1, 0},
		{6, 1, 0, 8, 10, -1, -1, 0},
	};

	RM(task2);
}
