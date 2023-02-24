#ifndef BONUS_H
#define BONUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define HGRN "\x1B[0;92m"

#define BHRED "\x1B[1;91m"
#define BHWHT "\x1B[1;97m"

char* getRainbowColor(int i);

#endif