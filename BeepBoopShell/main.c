/**
 * @file main.c
 * @author Andrei Girjoaba, Oscar de Francesca
 * @brief Our main file containing the loop of the shell.
 * Inside the loop we read input, tokenize it and parse it until the user exits.
 * 
 * @version 1.1
 * @date 2023-03-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

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

    #if EXT_PROMPT
    char cwd[200];
    #endif

    History hist;
    hist.index = 0;
    hist.top = 0;

    // Shell loop
    while (true) {
        #if EXT_PROMPT
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
        #if EXT_PROMPT
        addInputToHistory(&hist, inputLine);

#endif
        tokenList = getTokenList(inputLine);
        List startTokenList = tokenList;

        bool parsedSuccessfully = parseInputLine(&tokenList, &exitFlag, 0, hist); // exitFlag will be set to 1 for exit command
        if (!(tokenList == NULL && parsedSuccessfully)) {
            #if EXT_PROMPT
                printf("%s", HGRN);
            #endif

            printf("Error: invalid syntax!\n");

            #if EXT_PROMPT
                printf("%s", RESET);
            #endif
        }

        #if !EXT_PROMPT
        free(inputLine);
        #endif
        freeTokenList(startTokenList);
        if(exitFlag){
            break;
        }
    }
    // free history
    #if EXT_PROMPT
    for(int i = 0; i < hist.top; i++){
        free(hist.arr[i]);
    }
    #endif
    
    return 0;
}
