#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include "scanner.h"
#include "termios.h"
#include <unistd.h>

int getch() {
    struct termios oldtc;
    struct termios newtc;
    int ch;
    tcgetattr(STDIN_FILENO, &oldtc);
    newtc = oldtc;
    newtc.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
    ch=getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
    return ch;
}


char *upArrowPressed(char *history[], int *printFlag, int histIndex, int histSize, int histTop, char* s) {
    if (histIndex > 0) {
        histIndex--;
        s = history[histIndex];
        printf("%s", s);
        int c = getch();
        // if up arrow is pressed again or down arrow is pressed
        if (c == 27) {
            // clear the line
            for (int i = 0; i < strlen(s); i++) {
                printf("\b \b");
            }
            c = getch();
            if (c == 91) {
                c = getch();
                if (c == 65) {
                    if (histIndex == 0) {
                        *printFlag = 0;
                        return s;
                    }
                    return upArrowPressed(history, printFlag, histIndex, histSize, histTop, s);
                } else if (c == 66) {
                    if (histIndex == (histTop - 1)) {
                        *printFlag = 0;
                        return s;
                    }
                    return downArrowPressed(history, printFlag, histIndex, histSize, histTop, s);
                } else{
                    printf("%s", s);
                    int c = getch();
                }
            }
        }
        while (c != '\n') {
            if (c == 127) { // backspace pressed
                if (strlen(s) > 0) {
                    s[strlen(s) - 1] = '\0';
                    printf("\b \b");
                } else {
                    s[strlen(s)] = c;
                    s[strlen(s) + 1] = '\0';
                    printf("%c", c);
                }
                c = getch();
            } else {
                printf("%c", c);
                s[strlen(s)] = c;
                s[strlen(s) + 1] = '\0';
                c = getch();
            }
        }
        printf("\n");
    }
    return s;
}

char *downArrowPressed(char *history[], int *printFlag, int histIndex, int histSize, int histTop, char* s) {
    if (histIndex < histTop - 1) {
//        printf("histIndex: %d | histTop: %d\n", histIndex, histTop);
        histIndex++;
        s = history[histIndex];
        printf("%s", s);
        int c = getch();
        // if down arrow is pressed again
        if (c == 27) {
            // clear the line
            for (int i = 0; i < strlen(s); i++) {
                printf("\b \b");
            }
            c = getch();
            if (c == 91) {
                c = getch();
                if (c == 65) {
                    return upArrowPressed(history, printFlag, histIndex, histSize, histTop, s);
                } else if (c == 66) {
//                    printf("idx: %d | top: %d", histIndex, histTop);
                    if (histIndex == (histTop-1)) {
                        *printFlag = 0;
                        return s;
                    }
                    return downArrowPressed(history, printFlag, histIndex, histSize, histTop, s);
                }  else{
                    printf("%s", s);
                    c = getch();
                }
            }
        }
        while (c != '\n') {
            if (c == 127) { // backspace pressed
                if (strlen(s) > 0) {
                    s[strlen(s) - 1] = '\0';
                    printf("\b \b");
                } else {
                    s[strlen(s)] = c;
                    s[strlen(s) + 1] = '\0';
                    printf("%c", c);
                }
                c = getch();
            } else {
                printf("%c", c);
                s[strlen(s)] = c;
                s[strlen(s) + 1] = '\0';
                c = getch();
            }
        }
        printf("\n");
    } else {
        s = "\0";
    }
    return s;
}

/**
 * Reads an inputline from stdin.
 * @return a string containing the inputline.
 */
char *readInputLine(int *exitFlag, int *printFlag, char *history[], int histIndex, int histSize, int histTop) {
    int strLen = INITIAL_STRING_SIZE;
    int c = getch();
    int i = 0;
    char *s = malloc((strLen + 1) * sizeof(*s));
    assert(s != NULL);

    if (c == EOF) {
        *exitFlag = 1;
        s[0] = '\0';
        return s;
    } else if (c == 27) { // TODO: wrap in bonus
        c = getch();
        if (c == 91) {
            c = getch();
            if (c == 65) { // up arrow
                if (histIndex == 0) {
                    *printFlag = 0;
                    return s;
                }
                s = upArrowPressed(history, printFlag, histIndex, histSize, histTop, s);
            } else if (c == 66) { // down arrow
                if (histIndex == histTop) {
                    *printFlag = 0;
                    return s;
                }
                s = downArrowPressed(history, printFlag, histIndex, histSize, histTop, s);
            }
        } else {
            putchar(c);
        }
        return s;
    }

    bool quoteStarted = false;
    while (c != '\n' || quoteStarted) {// Ensure that newlines in strings are accepted
        if (c == '\"') {
            quoteStarted = !quoteStarted;
        }
        if (c == 127) {
            if (i == 0) {
                c = getch();
                continue;
            }
            if (i > 0) {
                s[i - 1] = '\0';
                i--;
                printf("\b \b");
            } else {
                s[i] = c;
                s[i + 1] = '\0';
                printf("%c", c);
            }
            c = getch();
            continue;
        } else {
//            i++;
//            s[strlen(s)] = c;
            s[i++] = c;
//            printf("\n%s\n",
//            s);
            putchar(c);
        }

        if (i >= strLen) { // Resize the string if necessary
            strLen = 2 * strLen;
            s = realloc(s, (strLen + 1) * sizeof(*s));
            assert(s != NULL);
        }
        c = getch();
    }
    putchar('\n');
    s[i] = '\0';
    return s;
}

/**
 * The function isOperatorCharacter checks whether the input paramater \param c is an operator.
 * @param c input character.
 * @return a bool denoting whether \param c is an operator.
 */
bool isOperatorCharacter(char c) {
    return c == '&' || c == '|' || c == ';' || c == '<' || c == '>';
}

/**
 * Reads an identifier in string \param s starting at index \param start.
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a pointer to the start of the identifier string
 */
char *matchIdentifier(char *s, int *start) {
    int strLen = INITIAL_STRING_SIZE;
    int pos = 0, offset = 0;

    char *ident = malloc((strLen + 1) * sizeof(*ident));
    assert(ident != NULL);

    bool quoteStarted = false;
    int length = strlen(s);
    while ((!isspace(s[*start + offset]) && !isOperatorCharacter(s[*start + offset])) || quoteStarted) { // Ensure that whitespace in strings is accepted
        if (s[*start + offset] == '\"') { // Strip the quotes from the input before storing in the identifier
            quoteStarted = !quoteStarted;
            offset++;
            continue;
        }
        ident[pos++] = s[*start + offset++];
        if (*start + offset > length) { // Identifiers of size 1
            break;
        }
        if (pos >= strLen) { // Resize the string if necessary
            strLen = 2 * strLen;
            ident = realloc(ident, (strLen + 1) * sizeof(*ident));
            assert(ident != NULL);
        }
    }
    ident[pos] = '\0';
    *start = *start + offset;
    return ident;
}

/**
 * The function newNode makes a new node for the token list and fills it with the token that
 * has been read. Precondition: !isspace(a[*ip]).
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a list node that contains the current token.
 */
List newNode(char *s, int *start) {
    List node = malloc(sizeof(*node));
    assert(node != NULL);
    node->next = NULL;
    node->t = matchIdentifier(s, start);
    return node;
}

/**
 * Reads an operator in string \param s starting at index \param start.
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a pointer to the start of the operator string.
 */
char *matchOperator(char *s, int *start) {
    int strLen = 2; // the operator consists of *at most* 2 characters
    int pos = 0, offset = 0;

    char *op = malloc((strLen + 1) * sizeof(*op));
    assert(op != NULL);

    while (isOperatorCharacter(s[*start + offset])) {
        op[pos++] = s[*start + offset++];
    }
    op[pos] = '\0';
    *start = *start + offset;
    return op;
}

/**
 * The function newOperatorNode makes a new operator node for the token list and fills it with the token that
 * has been read. Precondition: !isspace(a[*ip]).
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a list node that contains the current token.
 */
List newOperatorNode(char *s, int *start) {
    List node = malloc(sizeof(*node));
    assert(node != NULL);
    node->next = NULL;
    node->t = matchOperator(s, start);
    return node;
}

/**
 * The function tokenList reads an array and puts the tokens that are read in a list.
 * @param s input string.
 * @return a pointer to the beginning of the list.
 */
List getTokenList(char *s) {
    List lastNode = NULL;
    List node = NULL;
    List tl = NULL;
    int i = 0;
    int length = strlen(s);
    while (i < length) {
        if (isspace(s[i])) { // spaces are skipped
            i++;
        }else {
            node = isOperatorCharacter(s[i]) ? newOperatorNode(s, &i) : newNode(s, &i);
            if (lastNode == NULL) { // there is no list yet
                tl = node;
            } else { // a list already exists; add current node at the end
                (lastNode)->next = node;
            }
            lastNode = node;
        }
    }
    return tl;
}

/**
 * Checks whether list \param l is empty.
 * @param l input list.
 * @return a bool denoting whether \param l is empty.
 */
bool isEmpty(List l) {
    return l == NULL;
}

/**
 * The function printList prints the tokens in a token list, separated by commas.
 * @param li the input list to be printed.
 */
void printList(List li) {
    if (li == NULL) return;
    printf("\"%s\"", li->t);
    li = li->next;
    while (li != NULL) {
        printf(", \"%s\"", li->t);
        li = li->next;
    }
    printf("\n");
}

/**
 * The function freeTokenlist frees the memory of the nodes of the list, and of the strings
 * in the nodes.
 * @param li the starting node of a list.
 */
void freeTokenList(List li) {
    if (li == NULL) {
        return;
    }
    free(li->t);
    freeTokenList(li->next);
    free(li);
}
