#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <unistd.h>
#include <limits.h>
#include "scanner.h"
#include "shell.h"
#include "history.h"
#include "bonus.h"

/**
 * @brief Main function of the shell. Runs the shell loop.
 * 
 * @param argc argument paremeter count
 * @param argv argument parameter vector
 */
int main(int argc, char *argv[]) {
    char *inputLine;
    List tokenList;
    int exitFlag = 0;
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    #if BONUS
    int histSize = 100;
    char *history[histSize];
    int histIndex = 0;
    int histTop = 0;
    #endif

    // Shell loop
    int printFlag = 1;
    while (true) {
        // Print the current directory
        #if BONUS
        printCurrentDirectory(printFlag);
        #endif

        printFlag = 1;

        // Read the input line
        #if BONUS
        inputLine = readInputLineBonus(&exitFlag, &printFlag, history, histIndex, histSize, histTop); // exitFlag will be set to 1 for EOF
        #else
        inputLine = readInputLine(&exitFlag); // exitFlag will be set to 1 for EOF
        #endif

        // Add to history
        #if BONUS
        if (!printFlag) {
            continue;
        }
        printFlag = 1;
        addInputToHistory(histSize, history, &histIndex, &histTop, inputLine);
        #endif

        // Get the token list
        tokenList = getTokenList(inputLine);
        List startTokenList = tokenList;

        // Parse the input line
        bool parsedSuccessfully = parseInputLine(&tokenList, &exitFlag, 0); // exitFlag will be set to 1 for exit command
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

        #if !BONUS
        free(inputLine);
        #endif

        if(exitFlag){
            break;
        }
    }

    // Free history
    #if BONUS
    for (int i = 0; i < histTop; i++) {
        free(history[i]);
    }
    #endif

    return 0;
}
