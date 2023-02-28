/**
 * @file MyList.c
 * @brief MyList.c file adapted to manage integers from the scanner of the previous assignment.
 * @version 0.1
 * @date 2023-02-28
 * 
 */

#include "MyList.h"

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
    printf("\"%d\"", li->t);
    li = li->next;
    while (li != NULL) {
        printf(", \"%d\"", li->t);
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
    freeTokenList(li->next);
    free(li);
}

/**
 * @brief Create a list node object with an integer attached as a token.
 * 
 * @param n the integer.
 * @return The list node.
 */
List newNode(int n) {
    List node = malloc(sizeof(*node));
    node->next = NULL;
    node->t = n;
    return node;
}