#include "history.h"
#include "shell.h"
#include "string.h"
#include <ctype.h>

/**
 * @brief Adds the input line to the history array.
 * 
 * @param histSize the size of the history array.
 * @param history the history of previously executed commands.
 * @param histIndex the current index in the history array.
 * @param histTop the maximum index in the history array.
 * @param inputLine the current input line.
 */
void addInputToHistory(History *hist, char *inputLine) {
    if (hist->top < HIST_SIZE) {
        hist->arr[hist->top] = inputLine;
        (hist->top)++;
    } else {
        for (int i = 0; i < HIST_SIZE - 1; i++) {
            hist->arr[i] = hist->arr[i + 1];
        }
        hist->arr[HIST_SIZE - 1] = inputLine;
    }
    hist->index = hist->top;
}

int isNumeric(const char *str) {
    int i;
    int len = strlen(str);
    for (i = 0; i < len; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

/**
* @brief Prints the history of previously executed commands.
*/
bool printHistory(History hist, char *args[]) {
    if (args[0] == NULL) {
        if (hist.top == 0) {
            printf("No commands in history.\n");
            return false;
        } else {
            for (int i = 0; i < hist.top; i++) {
                printf(" [%d] %s\n", i, hist.arr[i]);
            }
            return true;
        }
    } else if (isNumeric(args[0]) && args[1] == NULL && atoi(args[0]) >= 0 && atoi(args[0]) < hist.top) {
        List tokenList = getTokenList(hist.arr[atoi(args[0])]);
        List startTokenList = tokenList;
        bool parsedSuccessfully = parseInputLine(&tokenList, 0, 0, hist); // exitFlag will be set to 1 for exit command
        if (!(tokenList == NULL && parsedSuccessfully)) {
        #if BONUS
            printf("%s", HGRN);
        #endif
        printf("Error: invalid syntax!\n");

        #if BONUS
            printf("%s", RESET);
        #endif
        }

        freeTokenList(startTokenList);
        return true;
    } else {
        printf("Error: invalid history index!\n");
        return false;
    }
}
