#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#define MAX_PROCESSES 20
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
    printf("%s\nAverage Response: %.2f, Avgerage Turnaround Time: %.2f, Average Wait: %.2f\n\n", // 2 decimals required... and looks good.
            algorithm, avg_response, avg_turnaround, avg_wait);
}

//First come first serverd
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
    print_statistics("First Come, First Served", 
                     total_response / (double)process_count, 
                     total_turnaround / (double)process_count, 
                     total_wait / (double)process_count);
}

// shortest job first
void sjf() {
    int completed = 0, clock = 0, total_response = 0, total_wait = 0, total_turnaround = 0;
    int visited[MAX_PROCESSES] = {0};

    while (completed < process_count) {
        int shortest_index = -1, shortest_time = INT_MAX;

        for (int i = 0; i < process_count; i++) {
            if (!visited[i] && processes[i].arrival <= clock && processes[i].burst < shortest_time) {
                shortest_time = processes[i].burst;
                shortest_index = i;
            }
        }

        if (shortest_index == -1) {
            clock++;
            continue;
        }

        visited[shortest_index] = 1;
        processes[shortest_index].start_time = clock;
        total_response += clock - processes[shortest_index].arrival;
        clock += processes[shortest_index].burst;
        processes[shortest_index].completion_time = clock;
        total_turnaround += processes[shortest_index].completion_time - processes[shortest_index].arrival;
        total_wait += processes[shortest_index].completion_time - processes[shortest_index].arrival - processes[shortest_index].burst;
        completed++;
    }

    print_statistics("Shortest Job First", 
                     total_response / (double)process_count, 
                     total_turnaround / (double)process_count, 
                     total_wait / (double)process_count);
}

//The shortest remaining time first job
void srtf() {
    int clock = 0, completed = 0, total_response = 0, total_wait = 0, total_turnaround = 0;
    int remaining_time[MAX_PROCESSES];
    int first_execution[MAX_PROCESSES];
    for (int i = 0; i < process_count; i++) {
        remaining_time[i] = processes[i].burst;
        first_execution[i] = 1;
    }

    while (completed < process_count) {
        int shortest_index = -1, min_remaining = INT_MAX;

        for (int i = 0; i < process_count; i++) {
            if (processes[i].arrival <= clock && remaining_time[i] > 0 && remaining_time[i] < min_remaining) {
                min_remaining = remaining_time[i];
                shortest_index = i;
            }
        }

        if (shortest_index == -1) {
            clock++;
            continue;
        }

        if (first_execution[shortest_index]) {
            total_response += clock - processes[shortest_index].arrival;
            first_execution[shortest_index] = 0;
        }

        remaining_time[shortest_index]--;
        clock++;

        if (remaining_time[shortest_index] == 0) {
            completed++;
            processes[shortest_index].completion_time = clock;
            total_turnaround += processes[shortest_index].completion_time - processes[shortest_index].arrival;
            total_wait += processes[shortest_index].completion_time - processes[shortest_index].arrival - processes[shortest_index].burst;
        }
    }

    print_statistics("Shortest Remaining Time First", 
                     total_response / (double)process_count, 
                     total_turnaround / (double)process_count, 
                     total_wait / (double)process_count);
}

// Round Robin with the quantum time of 100ms
void round_robin() {
    int clock = 0, completed = 0, total_response = 0, total_wait = 0, total_turnaround = 0;
    int remaining_time[MAX_PROCESSES];
    int first_execution[MAX_PROCESSES];

    for (int i = 0; i < process_count; i++) {
        remaining_time[i] = processes[i].burst;
        first_execution[i] = 1;
    }

    int queue[MAX_PROCESSES], front = 0, rear = 0;
    for (int i = 0; i < process_count; i++) queue[rear++] = i;

    while (completed < process_count) {
        int index = queue[front++];
        

        if (first_execution[index]) {
            total_response += clock - processes[index].arrival;
            first_execution[index] = 0;
        }

        int execute_time = (remaining_time[index] > TIME_QUANTUM) ? TIME_QUANTUM : remaining_time[index];
        clock += execute_time;
        remaining_time[index] -= execute_time;

        if (remaining_time[index] == 0) {
            completed++;
            processes[index].completion_time = clock;
            total_turnaround += processes[index].completion_time - processes[index].arrival;
            total_wait += processes[index].completion_time - processes[index].arrival - processes[index].burst;
        } else {
            queue[rear++] = index;
        }
    }

    print_statistics("Round Robin with Time Quantum of 100", 
                     fabs(total_response / (double)process_count),
                     total_turnaround / (double)process_count, 
                     total_wait / (double)process_count);
}

int main(int argc, char *argv[]) {
    FILE *fp = stdin;
    if (argc > 1) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            perror("FAILED! Couldn't open file.");
            return EXIT_FAILURE;
        }
    }

    read_input(fp);
    if (fp != stdin) fclose(fp);
    
    printf("Assn3: Thee Process Scheduler Simulator by Jakob Hodgson\n");
    printf("ALL TIMES ARE IN ms\n");

    fcfs();
    sjf();
    srtf();
    round_robin();
    
    return 0;
}

