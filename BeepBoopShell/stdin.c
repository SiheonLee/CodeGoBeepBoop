#include <stdio.h>
#include <stdlib.h>



/**
 * @return a string containing the inputline.
 */
 int main(int argc, char *argv[]) {
    printf("echo ");
    for (int i = 0; i < 30000; i++) {
        printf("%d", i);
    }
    printf("\nexit\n");
 }
