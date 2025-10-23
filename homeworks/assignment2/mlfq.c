/**
 * Author: Deepak Govindarajan
 * CSC 4320 Operating Systems
 * Homework 2: Multilevel Feedback Queue Scheduling
 * 10/22/2025
 * 
 * MLFQ scheduling simulation with Gantt chart visualization
 */

#include "mlfq.h"

// Read processes from file
int read_processes_from_file(const char* filename, Process processes[]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    char line[256];
    int count = 0;
    int priority; // Read but not used in MLFQ

    // Skip header line
    fgets(line, sizeof(line), file);

    while (fscanf(file, "%d %d %d %d", &processes[count].id,
           &processes[count].arrival_time,
           &processes[count].burst_time,
           &priority) == 4) {
        
        processes[count].remaining_time = processes[count].burst_time;
        processes[count].completion_time = 0;
        processes[count].turnaround_time = 0;
        processes[count].waiting_time = 0;
        processes[count].current_queue = 0; // Start in Q1
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

void init_process(Process *p, int id, int arrival, int burst) {
    p->id = id;
    p->arrival_time = arrival;
    p->burst_time = burst;
    p->remaining_time = burst;
    p->completion_time = 0;
    p->turnaround_time = 0;
    p->waiting_time = 0;
    p->current_queue = 0;
    p->is_completed = false;
}

void init_queues(Queue queues[]) {
    for (int i = 0; i < NUM_QUEUES; i++) {
        queues[i].front = 0;
        queues[i].rear = 0;
    }
    queues[0].quantum = Q1_QUANTUM;
    queues[1].quantum = Q2_QUANTUM;
    queues[2].quantum = -1; // FCFS for Q3
}

bool is_queue_empty(Queue *q) {
    return q->front == q->rear;
}

void enqueue(Queue *q, int process_id) {
    q->processes[q->rear++] = process_id;
}

int dequeue(Queue *q) {
    return q->processes[q->front++];
}

// Get next process from highest priority non-empty queue        
int get_next_process(Queue queues[], Process processes[], int n) {
    for (int q = 0; q < NUM_QUEUES; q++) {
        if (!is_queue_empty(&queues[q])) {
            return queues[q].processes[queues[q].front];
        }
    }
    return -1;
}

// Add newly arrived processes to Q1
void add_new_arrivals(Queue queues[], Process processes[], int n, int current_time) {
    for (int i = 0; i < n; i++) {
        if (!processes[i].is_completed && processes[i].arrival_time == current_time) {
            processes[i].current_queue = 0; // Always start in Q1
            enqueue(&queues[0], i);
        }
    }
}

// MLFQ scheduling algorithm implementation
void mlfq_schedule(Process processes[], int n, GanttEntry gantt[], int *gantt_count) {
    Queue queues[NUM_QUEUES];
    int current_time = 0;
    int completed_processes = 0;
    *gantt_count = 0;

    init_queues(queues);

    // Add processes that arrive at time 0
    add_new_arrivals(queues, processes, n, 0);

    // Main scheduling loop
    while (completed_processes < n) {
        // If no processes ready, advance time
        int next_process = get_next_process(queues, processes, n);
        if (next_process == -1) {
            int next_arrival = -1;
            for (int i = 0; i < n; i++) {
                if (!processes[i].is_completed && 
                    (next_arrival == -1 || processes[i].arrival_time < processes[next_arrival].arrival_time)) {
                    next_arrival = i;
                }
            }
            if (next_arrival != -1) {
                current_time = processes[next_arrival].arrival_time;
                add_new_arrivals(queues, processes, n, current_time);
            }
            continue;
        }

        // Get next process from highest priority queue
        int current_process = next_process;
        Process *p = &processes[current_process];
        int current_queue = p->current_queue;
        int quantum = queues[current_queue].quantum;
        
        // Dequeue the process from its current queue
        dequeue(&queues[current_queue]);

        // Calculate execution time
        int execution_time;
        if (quantum == -1) { 
            execution_time = p->remaining_time;
        } else {
            execution_time = (p->remaining_time > quantum) ? quantum : p->remaining_time;
        }

        gantt[*gantt_count].process_id = p->id;
        gantt[*gantt_count].start_time = current_time;
        gantt[*gantt_count].end_time = current_time + execution_time;
        (*gantt_count)++;

        current_time += execution_time;
        p->remaining_time -= execution_time;

        add_new_arrivals(queues, processes, n, current_time);

        if (p->remaining_time == 0) {
            p->completion_time = current_time;
            p->turnaround_time = p->completion_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            p->is_completed = true;
            completed_processes++;
        } else {
            if (current_queue < NUM_QUEUES - 1) {
                p->current_queue = current_queue + 1;
                enqueue(&queues[p->current_queue], current_process);
            } else {
                enqueue(&queues[current_queue], current_process);
            }
        }
    }
}

void print_ascii_gantt(GanttEntry gantt[], int gantt_count) {
    printf("\nGantt Chart:\n");
    
    for (int i = 0; i < gantt_count; i++) {
        printf("----");
    }
    printf("-\n");

    printf("|");
    for (int i = 0; i < gantt_count; i++) {
        printf("P%d |", gantt[i].process_id);
    }
    printf("\n");
    
    for (int i = 0; i < gantt_count; i++) {
        printf("----");
    }
    printf("-\n");

    printf("%-2d", gantt[0].start_time);
    for (int i = 0; i < gantt_count; i++) {
        printf("  %-2d", gantt[i].end_time);
    }
    printf("\n\n");
}

void gantt_data(GanttEntry gantt[], int gantt_count, const char *filename) {
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

// Print scheduling results including waiting and turnaround times
void print_scheduling_results(Process processes[], int n) {
    double total_waiting = 0;
    double total_turnaround = 0;

    printf("\nProcess Statistics:\n");
    printf("PID\tArrival\tBurst\tWaiting\tTurnaround\tFinal Queue\n");
    printf("--------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t\tQ%d\n",
               processes[i].id,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].current_queue + 1);

        total_waiting += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / n);
}

int main() {
    Process processes[MAX_PROCESSES];
    GanttEntry gantt[MAX_GANTT_ENTRIES];
    int n, gantt_count;

    n = read_processes_from_file("processes.txt", processes);
    if (n == 0) {
        return 1;
    }

    printf("Multilevel Feedback Queue Scheduling Simulation\n");
    printf("Q1 Quantum = %d, Q2 Quantum = %d, Q3 = FCFS\n\n", Q1_QUANTUM, Q2_QUANTUM);

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

    mlfq_schedule(processes, n, gantt, &gantt_count);

    print_ascii_gantt(gantt, gantt_count);
    
    print_scheduling_results(processes, n);

    gantt_data(gantt, gantt_count, "mlfq.txt");
    printf("\nGantt chart data has been written to 'mlfq.txt'.\n");

    return 0;
}
