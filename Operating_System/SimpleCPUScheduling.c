#include <stdio.h>

int main() {
    int n = 3;
    int bt[] = {5, 3, 8}; // Burst times for 3 processes
    int wt = 0, tat = 0;
    int total_wt = 0, total_tat = 0;

    printf("PID\tBurst Time\tWaiting Time\tTurnaround Time\n");
    
    for (int i = 0; i < n; i++) {
        tat = wt + bt[i]; // Total time in system
        
        total_wt += wt;
        total_tat += tat;

        printf("P%d\t%d\t\t%d\t\t%d\n", i + 1, bt[i], wt, tat);
        
        wt += bt[i]; // Current waiting time becomes next process's start wait
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_tat / n);
    
    return 0;
}
