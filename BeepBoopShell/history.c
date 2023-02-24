#include "history.h"

/**
 * @brief Adds the input line to the history array.
 * 
 * @param histSize the size of the history array.
 * @param history the history of previously executed commands.
 * @param histIndex the current index in the history array.
 * @param histTop the maximum index in the history array.
 * @param inputLine the current input line.
 */
void addInputToHistory(int histSize, char *history[], int *histIndex, int *histTop, char *inputLine) {
    if (*histTop < histSize) {
        history[*histTop] = inputLine;
        (*histTop)++;
    } else {
        for (int i = 0; i < histSize - 1; i++) {
            history[i] = history[i + 1];
        }
        history[histSize - 1] = inputLine;
    }
    *histIndex = *histTop;
}