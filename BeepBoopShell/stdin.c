#include <stdio.h>
#include <stdlib.h>



/**
 * @return a string containing the inputline.
 */
 int main(int argc, char *argv[]) {
    char *inputLine = malloc(100*sizeof(char));
    scanf("%s", inputLine);
    printf("This was read: %s\n", inputLine);
 }
