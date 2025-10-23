#ifndef MLFQ_H
#define MLFQ_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Constants for MLFQ scheduling
#define MAX_PROCESSES 100
#define MAX_GANTT_ENTRIES 1000
#define NUM_QUEUES 3
#define Q1_QUANTUM 4
#define Q2_QUANTUM 8

// Process structure
typedef struct {
    int id;              
    int arrival_time;    
    int burst_time;      
    int remaining_time;  
    int completion_time; 
    int turnaround_time; 
    int waiting_time;   
    int current_queue;   
    bool is_completed;   
} Process;

// Gantt chart entry
typedef struct {
    int process_id;
    int start_time;
    int end_time;
} GanttEntry;

// Queue structure
typedef struct {
    int processes[MAX_PROCESSES];
    int front;
    int rear;
    int quantum;
} Queue;

// Function declarations
void init_process(Process *p, int id, int arrival, int burst);
void init_queues(Queue queues[]);
void mlfq_schedule(Process processes[], int n, GanttEntry gantt[], int *gantt_count);
void print_ascii_gantt(GanttEntry gantt[], int gantt_count);
void print_scheduling_results(Process processes[], int n);
void gantt_data(GanttEntry gantt[], int gantt_count, const char *filename);
int read_processes_from_file(const char* filename, Process processes[]);

#endif
