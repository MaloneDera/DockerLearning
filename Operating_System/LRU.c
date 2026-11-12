#include <stdio.h>

int main() {
    int n, f;

    printf("Enter number of frames: ");
    scanf("%d", &f);

    printf("Enter number of pages in reference string: ");
    scanf("%d", &n);

    int pages[50];
    printf("Enter the page reference string (space-separated): ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    int frames[10];
    int time_stamp[10]; // Keeps track of when each frame was last used
    for (int i = 0; i < f; i++) {
        frames[i] = -1;     // -1 indicates an empty frame
        time_stamp[i] = 0;
    }

    int page_faults = 0;
    int current_time = 0;

    printf("\n--- LRU PAGE REPLACEMENT TRACE ---\n");

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;
        current_time++; // Increment logical clock on each reference

        // 1. Check if the page is already in memory (Page Hit)
        for (int j = 0; j < f; j++) {
            if (frames[j] == page) {
                found = 1;
                time_stamp[j] = current_time; // Update timestamp on hit
                break;
            }
        }

        // 2. If not found, it's a Page Fault
        if (!found) {
            int target = -1;

            // First, check if there is an empty frame available
            for (int j = 0; j < f; j++) {
                if (frames[j] == -1) {
                    target = j;
                    break;
                }
            }

            // If no empty frame, find the Least Recently Used page (smallest timestamp)
            if (target == -1) {
                int lru_time = time_stamp[0];
                target = 0;
                for (int j = 1; j < f; j++) {
                    if (time_stamp[j] < lru_time) {
                        lru_time = time_stamp[j];
                        target = j;
                    }
                }
            }

            // Replace or place the page
            frames[target] = page;
            time_stamp[target] = current_time;
            page_faults++;
        }

        // 3. Print current state of frames
        printf("Ref %2d -> [ ", page);
        for (int j = 0; j < f; j++) {
            if (frames[j] == -1)
                printf("- ");
            else
                printf("%d ", frames[j]);
        }
        printf("] %s\n", found ? "(Hit)" : "(Fault)");
    }

    printf("\nTotal Page Faults: %d\n", page_faults);

    return 0;
}
