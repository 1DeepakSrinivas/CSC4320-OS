/**
 * Author: Deepak Govindarajan
 * CSC 4320 Operating Systems
 * Homework 2: Round Robin Scheduling
 * 10/21/2025
 * 
 * Round Robin scheduling simulation with Gantt chart visualization
 */

#include "round_robin.h"

/**
 * Read processes from file
 */
int read_processes_from_file(const char* filename, Process processes[]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    char line[256];
    int count = 0;
    int priority; // Read but not used in RR

    // Skip header line
    fgets(line, sizeof(line), file);

    // Read process data
    while (fscanf(file, "%d %d %d %d", &processes[count].id,
           &processes[count].arrival_time,
           &processes[count].burst_time,
           &priority) == 4) {
        
        processes[count].remaining_time = processes[count].burst_time;
        processes[count].completion_time = 0;
        processes[count].turnaround_time = 0;
        processes[count].waiting_time = 0;
        processes[count].is_completed = false;
        
        count++;
        if (count >= MAX_PROCESSES) {
            printf("Warning: Maximum number of processes reached\n");
            break;
        }
    }

    fclose(file);
    return count;
}

/**
 * Initialize a process with given parameters
 */
void init_process(Process *p, int id, int arrival, int burst) {
    p->id = id;
    p->arrival_time = arrival;
    p->burst_time = burst;
    p->remaining_time = burst;
    p->completion_time = 0;
    p->turnaround_time = 0;
    p->waiting_time = 0;
    p->is_completed = false;
}

/**
 * Round Robin scheduling algorithm implementation
 */
void round_robin_schedule(Process processes[], int n, int quantum, GanttEntry gantt[], int *gantt_count) {
    int current_time = 0;
    int completed_processes = 0;
    int queue[MAX_PROCESSES];
    int front = 0, rear = 0;
    bool in_queue[MAX_PROCESSES] = {false};
    *gantt_count = 0;

    // Add processes that arrive at time 0
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time == 0) {
            queue[rear++] = i;
            in_queue[i] = true;
        }
    }

    // Main scheduling loop
    while (completed_processes < n) {
        // If queue is empty, advance time to next arrival
        if (front == rear) {
            int next_arrival = -1;
            for (int i = 0; i < n; i++) {
                if (!processes[i].is_completed && !in_queue[i] && 
                    (next_arrival == -1 || processes[i].arrival_time < processes[next_arrival].arrival_time)) {
                    next_arrival = i;
                }
            }
            if (next_arrival != -1) {
                current_time = processes[next_arrival].arrival_time;
                queue[rear++] = next_arrival;
                in_queue[next_arrival] = true;
            }
            continue;
        }

        // Get next process from queue
        int current_process = queue[front++];
        Process *p = &processes[current_process];
        in_queue[current_process] = false;

        // Calculate execution time for this quantum
        int execution_time = (p->remaining_time > quantum) ? quantum : p->remaining_time;

        // Record gantt entry
        gantt[*gantt_count].process_id = p->id;
        gantt[*gantt_count].start_time = current_time;
        gantt[*gantt_count].end_time = current_time + execution_time;
        (*gantt_count)++;

        // Update time and remaining burst
        current_time += execution_time;
        p->remaining_time -= execution_time;

        // Check for newly arrived processes
        for (int i = 0; i < n; i++) {
            if (!processes[i].is_completed && !in_queue[i] && 
                processes[i].arrival_time <= current_time) {
                queue[rear++] = i;
                in_queue[i] = true;
            }
        }

        // Process completion check
        if (p->remaining_time == 0) {
            p->completion_time = current_time;
            p->turnaround_time = p->completion_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            p->is_completed = true;
            completed_processes++;
        } else {
            queue[rear++] = current_process;
            in_queue[current_process] = true;
        }
    }
}

/**
 * Round Robin with memory management integration
 */


/**
 * Print ASCII Gantt chart
 */
void print_ascii_gantt(GanttEntry gantt[], int gantt_count) {
    printf("\nGantt Chart:\n");
    
    // Print top border
    for (int i = 0; i < gantt_count; i++) {
        printf("----");
    }
    printf("-\n");

    // Print process IDs
    printf("|");
    for (int i = 0; i < gantt_count; i++) {
        printf("P%d |", gantt[i].process_id);
    }
    printf("\n");
    
    // Print bottom border
    for (int i = 0; i < gantt_count; i++) {
        printf("----");
    }
    printf("-\n");

    // Print time markers
    printf("%-2d", gantt[0].start_time);
    for (int i = 0; i < gantt_count; i++) {
        printf("  %-2d", gantt[i].end_time);
    }
    printf("\n\n");
}

/**
 * Generate Gantt chart data for Python visualization
 */
void generate_gantt_data(GanttEntry gantt[], int gantt_count, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: Could not create Gantt chart data file\n");
        return;
    }

    for (int i = 0; i < gantt_count; i++) {
        fprintf(fp, "%d,%d,%d\n", 
                gantt[i].process_id,
                gantt[i].start_time,
                gantt[i].end_time);
    }

    fclose(fp);
}

/**
 * Print scheduling results including waiting and turnaround times
 */
void print_scheduling_results(Process processes[], int n) {
    double total_waiting = 0;
    double total_turnaround = 0;

    printf("\nProcess Statistics:\n");
    printf("PID\tArrival\tBurst\tWaiting\tTurnaround\n");
    printf("----------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n",
               processes[i].id,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].waiting_time,
               processes[i].turnaround_time);

        total_waiting += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / n);
}



/**
 * Main function
 */
int main() {
    Process processes[MAX_PROCESSES];
    GanttEntry gantt[MAX_GANTT_ENTRIES];
    int n, gantt_count;

    // Read processes from file
    n = read_processes_from_file("processes.txt", processes);
    if (n == 0) {
        return 1;
    }

    printf("Round Robin Scheduling Simulation\n");
    printf("Time Quantum = %d\n\n", TIME_QUANTUM);

    // Print input processes
    printf("Input Processes:\n");
    printf("PID\tArrival\tBurst\n");
    printf("-------------------\n");
    for(int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\n", 
               processes[i].id,
               processes[i].arrival_time,
               processes[i].burst_time);
    }
    printf("\n");

    // Run round robin scheduling
    round_robin_schedule(processes, n, TIME_QUANTUM, gantt, &gantt_count);

    // Print ASCII Gantt chart
    print_ascii_gantt(gantt, gantt_count);
    
    // Print process statistics
    print_scheduling_results(processes, n);

    // Generate data file for Python visualization
    generate_gantt_data(gantt, gantt_count, "gantt_data.txt");
    printf("\nGantt chart data has been written to 'gantt_data.txt'.\n");

    return 0;
}