#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <unistd.h>
#include <limits.h>
#include "scanner.h"
#include "shell.h"
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

//    #if BONUS
    char cwd[200];
    int histSize = 100;
    char *history[histSize];
    int histIndex = 0;
    int histTop = 0;

//    #endif

    // Shell loop
    int printFlag = 1;
    while (true) {
        #if BONUS
        int color = 0;
        if (getcwd(cwd, sizeof(cwd)) != NULL && printFlag) {
            for(int i = 0; i < strlen(cwd); i++){
                if(cwd[i] == '/'){
                    printf("%s", getRainbowColor(color++ % 6));
                }
                printf("%c", cwd[i]);
            }
            printf("%s>%s ", BHWHT, RESET);
        }
        #endif
        printFlag = 1;
        inputLine = readInputLine(&exitFlag, &printFlag, history, histIndex, histSize, histTop);// exitFlag will be set to 1 for EOF
        // #if BONUS
        if (!printFlag) {
            continue;
        }
        printFlag = 1;
        if (histTop < histSize) {
            history[histTop] = inputLine;
            histTop++;
        } else {
            for (int i = 0; i < histSize - 1; i++) {
                history[i] = history[i + 1];
            }
            history[histSize - 1] = inputLine;
        }
        histIndex = histTop;


        // #endif
        tokenList = getTokenList(inputLine);

        List startTokenList = tokenList;

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
        if(exitFlag){
            break;
        }
//         free history
    }
    for (int i = 0; i < histTop; i++) {
        free(history[i]);
    }

    return 0;
}
