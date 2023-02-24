#include "bonus.h"

char *getRainbowColor(int i) {
    switch (i % 6) {
        case 0:
            return RED;
        case 1:
            return YEL;
        case 2:
            return GRN;
        case 3:
            return CYN;
        case 4:
            return BLU;
        case 5:
            return MAG;
        default:
            return WHT;
    }
}

void printCurrentDirectory(int printFlag) {
    char cwd[200];
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
}
