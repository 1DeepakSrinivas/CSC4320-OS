#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Constants for Round Robin scheduling
#define MAX_PROCESSES 100
#define MAX_GANTT_ENTRIES 1000
#define TIME_QUANTUM 4  // Default time quantum

// Process structure
typedef struct {
    int id;              // Process ID
    int arrival_time;    // Arrival time
    int burst_time;      // Total CPU burst time needed
    int remaining_time;  // Remaining burst time
    int completion_time; // Time when process completes
    int turnaround_time; // Total time in system
    int waiting_time;    // Time spent waiting
    bool is_completed;   // Whether process has finished
} Process;

// Gantt chart entry
typedef struct {
    int process_id;
    int start_time;
    int end_time;
} GanttEntry;

// Function declarations
void init_process(Process *p, int id, int arrival, int burst);
void round_robin_schedule(Process processes[], int n, int quantum, GanttEntry gantt[], int *gantt_count);
void print_gantt_chart(GanttEntry gantt[], int gantt_count);
void print_scheduling_results(Process processes[], int n);
void print_ascii_gantt(GanttEntry gantt[], int gantt_count);
void generate_gantt_data(GanttEntry gantt[], int gantt_count, const char *filename);

#endif // ROUND_ROBIN_H
