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
    for (int i = 0; i < f; i++) {
        frames[i] = -1; // -1 indicates an empty frame
    }

    int page_faults = 0;

    printf("\n--- OPTIMAL PAGE REPLACEMENT TRACE ---\n");

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;

        // 1. Check if the page is already in memory (Page Hit)
        for (int j = 0; j < f; j++) {
            if (frames[j] == page) {
                found = 1;
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

            // If no empty frame, find the optimal victim by looking ahead
            if (target == -1) {
                int farthest = -1;
                int farthest_index = -1;

                for (int j = 0; j < f; j++) {
                    int found_in_future = 0;
                    int next_use = -1;

                    // Scan future reference string for frames[j]
                    for (int k = i + 1; k < n; k++) {
                        if (frames[j] == pages[k]) {
                            next_use = k;
                            found_in_future = 1;
                            break;
                        }
                    }

                    // If a page is NEVER used again in the future, pick it immediately
                    if (!found_in_future) {
                        farthest_index = j;
                        break;
                    }

                    // Otherwise, find the one used furthest in the future
                    if (next_use > farthest) {
                        farthest = next_use;
                        farthest_index = j;
                    }
                }
                target = farthest_index;
            }

            // Replace or place the page
            frames[target] = page;
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
