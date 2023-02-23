#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <unistd.h>
#include <limits.h>
#include "scanner.h"
#include "string.h"

int main() {
    char *inputLine = malloc(100);
    strcpy(inputLine, "hello world");
    printf("%s", inputLine);
    int c = getch();
    while (c != '\n') {
        if (c == 127) {
            if (strlen(inputLine) > 0) {
                inputLine[strlen(inputLine) - 1] = '\0';
                printf("\b \b");
            }
//            printf("\b \b");
        } else {
            printf("%c", c);
//            inputLine[strlen(inputLine)] = c;
        }
        c = getch();
    }
    printf("\n");
    printf("%s\n", inputLine);
//    fflush(stdout); // flush the output buffer to ensure the text is printed
//    sleep(1); // wait for 1 second
//    printf("\b \b"); // move the cursor back and overwrite the last character with a space
//    printf("\b \b");
//    fflush(stdout); // flush the output buffer to ensure the space is printed
}