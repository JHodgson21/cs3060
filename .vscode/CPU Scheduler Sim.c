#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100
#define TIME_QUANTUM 100

typedef struct {
    int id;
    int arrival;
    int burst;
    int remaining;
    int start_time;
    int completion_time;
} Process;

Process processes[MAX_PROCESSES];
int process_count = 0;

void read_input(FILE *fp) {
    int arrival, burst;
    while (fscanf(fp, "%d %d", &arrival, &burst) == 2) {
        processes[process_count].id = process_count;
        processes[process_count].arrival = arrival;
        processes[process_count].burst = burst;
        processes[process_count].remaining = burst;
        processes[process_count].start_time = -1;
        process_count++;
    }
}

void print_statistics(const char *algorithm, double avg_response, double avg_turnaround, double avg_wait) {
    printf("%s\nAvg. Resp.: %.2f, Avg. T.A.: %.2f, Avg. Wait: %.2f\n", algorithm, avg_response, avg_turnaround, avg_wait);
}

//FCFS main alg
void fcfs() {
    int clock = 0, total_response = 0, total_wait = 0, total_turnaround = 0;
    for (int i = 0; i < process_count; i++) {
        if (clock < processes[i].arrival)
            clock = processes[i].arrival;
        processes[i].start_time = clock;
        total_response += processes[i].start_time - processes[i].arrival;
        clock += processes[i].burst;
        processes[i].completion_time = clock;
        total_turnaround += processes[i].completion_time - processes[i].arrival;
        total_wait += processes[i].completion_time - processes[i].arrival - processes[i].burst;
    }
    print_statistics("First Come, First Served", total_response / (double)process_count, total_turnaround / (double)process_count, total_wait / (double)process_count);
}

// The rest

int main(int argc, char *argv[]) {
    FILE *fp = stdin;
    if (argc > 1) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            perror("Failed to open file");
            return EXIT_FAILURE;
        }
    }

    read_input(fp);
    if (fp != stdin) fclose(fp);
    
    printf("Assignment 3: Process Scheduler Simulator by <Your Name>\n");

    fcfs();
    
    return 0;
}
