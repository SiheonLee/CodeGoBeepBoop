#include <stdio.h>
#include "clock.h"

int main() {
    int memFrames;
    int pageFaults = 0;
    scanf("%d", &memFrames);
    Clock clock = newClock(memFrames);
    int page;
    while (scanf("%d", &page) != EOF) {
        // Check if the page is in the clock
        int exists = checkClock(&clock, page);
        if (!exists) {
            addToClock(&clock, page);
            pageFaults++;
        }
    }
    printf("%d\n", pageFaults);
    freeClock(clock);
    return 0;
}