#include "clock.h"

/**
 * Creates a new clock with the given size
 * @param s size of the clock
 * @return the clock
 */
Clock newClock (int s) {
    Clock clock;
    clock.array = malloc(s*sizeof(Element));
    for (int i = 0; i < s; i++) {
        clock.array[i].page = -1;
        clock.array[i].checked = true;
    }
    assert(clock.array != NULL);
    clock.size = s;
    clock.hand = 0;
    return clock;
}


/**
 * Checks if the page is in the clock
 * @param clock the clock
 * @param page the page to check
 * @return true if the page is in the clock, false otherwise
 */
bool checkClock (Clock *clock, int page) {
    for (int i = 0; i < clock->size; i++) {
        if (clock->array[i].page == page) {
            clock->array[i].checked = true;
            return true;
        }
    }
    return false;
}

/**
 * Adds a page to the clock
 * @param clock the clock
 * @param page the page to add
 */
void addToClock (Clock *clock, int page) {
    while (clock->array[clock->hand].checked) {
        clock->array[clock->hand].checked = false;
        clock->hand = (clock->hand + 1) % clock->size;
    }
    clock->array[clock->hand].page = page;
    clock->array[clock->hand].checked = false;
    clock->hand = (clock->hand + 1) % clock->size;
}


/**
 * Frees the clock
 * @param clock the clock to free
 */
void freeClock(Clock clock) {
    free(clock.array);
}
