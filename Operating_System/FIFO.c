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
    int next_to_replace = 0; // Points to the oldest page in FIFO order

    printf("\n--- FIFO PAGE REPLACEMENT TRACE ---\n");

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
            frames[next_to_replace] = page;              // Replace the oldest page
            next_to_replace = (next_to_replace + 1) % f; // Move pointer circularly
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
