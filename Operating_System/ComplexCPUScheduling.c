#include <stdio.h>

struct Process {
    int pid;
    int at;  // Arrival Time
    int bt;  // Burst Time
    int ct;  // Completion Time
    int tat; // Turnaround Time
    int wt;  // Waiting Time
    int completed; // Flag for SJF
};

// Function to calculate and print FCFS
void calculate_fcfs(struct Process p[], int n) {
    // Sort processes by Arrival Time (Simple Bubble Sort)
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    int current_time = 0;
    float total_tat = 0, total_wt = 0;

    printf("\n--- FCFS SCHEDULING RESULTS ---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");

    for (int i = 0; i < n; i++) {
        // If CPU is idle until this process arrives, jump time forward
        if (current_time < p[i].at) {
            current_time = p[i].at;
        }

        p[i].ct = current_time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;

        current_time = p[i].ct;

        total_tat += p[i].tat;
        total_wt += p[i].wt;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

// Function to calculate and print Non-Preemptive SJF
void calculate_sjf(struct Process p[], int n) {
    int completed_count = 0, current_time = 0;
    float total_tat = 0, total_wt = 0;

    // Reset completion flags
    for (int i = 0; i < n; i++) {
        p[i].completed = 0;
    }

    printf("\n--- NON-PREEMPTIVE SJF SCHEDULING RESULTS ---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");

    while (completed_count < n) {
        int shortest = -1;
        int min_bt = 99999;

        // Find the process that has arrived, is not completed, and has the shortest burst time
        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && !p[i].completed) {
                if (p[i].bt < min_bt) {
                    min_bt = p[i].bt;
                    shortest = i;
                }
                // Tie-breaker: if burst times are equal, pick the one that arrived earlier
                else if (p[i].bt == min_bt) {
                    if (p[i].at < p[shortest].at) {
                        shortest = i;
                    }
                }
            }
        }

        // If no process has arrived yet, advance time
        if (shortest == -1) {
            current_time++;
        } else {
            // Execute the shortest job
            current_time += p[shortest].bt;
            p[shortest].ct = current_time;
            p[shortest].tat = p[shortest].ct - p[shortest].at;
            p[shortest].wt = p[shortest].tat - p[shortest].bt;
            p[shortest].completed = 1;
            completed_count++;

            total_tat += p[shortest].tat;
            total_wt += p[shortest].wt;

            printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[shortest].pid, p[shortest].at, p[shortest].bt, p[shortest].ct, p[shortest].tat, p[shortest].wt);
        }
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p_fcfs[10], p_sjf[10];

    for (int i = 0; i < n; i++) {
        p_fcfs[i].pid = i + 1;
        printf("Process P%d Arrival Time: ", i + 1);
        scanf("%d", &p_fcfs[i].at);
        printf("Process P%d Burst Time: ", i + 1);
        scanf("%d", &p_fcfs[i].bt);
        
        // Copy inputs for SJF array as well
        p_sjf[i] = p_fcfs[i];
    }

    calculate_fcfs(p_fcfs, n);
    calculate_sjf(p_sjf, n);

    return 0;
}
