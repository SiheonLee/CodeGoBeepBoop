#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <unistd.h>
#include <limits.h>
#include "scanner.h"
#include "shell.h"
#include "bonus.h"
#include "history.h"

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
    char cwd[200];
    History hist;
    hist.index = 0;
    hist.top = 0;
    #endif

    // Shell loop
    while (true) {
        #if BONUS
        int color = 0;
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            for(int i = 0; i < strlen(cwd); i++){
                if(cwd[i] == '/'){
                    printf("%s", getRainbowColor(color++ % 6));
                }
                printf("%c", cwd[i]);
            }
            printf("%s>%s ", BHWHT, RESET);
        }
        #endif
        inputLine = readInputLine(&exitFlag); // exitFlag will be set to 1 for EOF
        #if BONUS
        addInputToHistory(&hist, inputLine);

#endif
        tokenList = getTokenList(inputLine);
        List startTokenList = tokenList;

        bool parsedSuccessfully = parseInputLine(&tokenList, &exitFlag, 0, hist); // exitFlag will be set to 1 for exit command
        if (!(tokenList == NULL && parsedSuccessfully)) {
            #if BONUS
                printf("%s", HGRN);
            #endif

            printf("Error: invalid syntax!\n");

            #if BONUS
                printf("%s", RESET);
            #endif
        }

        #if !BONUS
        free(inputLine);
        #endif
        freeTokenList(startTokenList);
        if(exitFlag){
            break;
        }
    }
    // free history
    #if BONUS
    for(int i = 0; i < hist.top; i++){
        free(hist.arr[i]);
    }
    #endif
    
    return 0;
}
