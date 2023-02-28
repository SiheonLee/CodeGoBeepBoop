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
