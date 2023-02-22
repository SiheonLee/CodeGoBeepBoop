#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "scanner.h"

    // The executable to be executed in the last parsed command
char *executable;
    // Exit code of the last executed command
int exitCode = 0;

/**
 * The function acceptToken checks whether the current token matches a target identifier,
 * and goes to the next token if this is the case.
 * @param lp List pointer to the start of the tokenlist.
 * @param ident target identifier
 * @return a bool denoting whether the current token matches the target identifier.
 */
bool acceptToken(List *lp, char *ident) {
    if (*lp != NULL && strcmp(((*lp)->t), ident) == 0) {
        *lp = (*lp)->next;
        return true;
    }
    return false;
}

/**
 * The function parseExecutable parses an executable.
 * The executable is saved in a global variable and is accessed by the parseOptions function.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the executable was parsed successfully.
 */
bool parseExecutable(List *lp) {
    executable = (*lp)->t;
    return true;
}

/**
 * Checks whether the input string \param s is an operator.
 * @param s input string.
 * @return a bool denoting whether the current string is an operator.
 */
bool isOperator(char *s) {
    // NULL-terminated array makes it easy to expand this array later
    // without changing the code at other places.
    char *operators[] = {
            "&",
            "&&",
            "||",
            ";",
            "<",
            ">",
            "|",
            NULL
    };

    for (int i = 0; operators[i] != NULL; i++) {
        if (strcmp(s, operators[i]) == 0) return true;
    }
    return false;
}

/**
 * @brief Executes a command using fork() and exec().
 * 
 * @param execArgs the options of the command
 * @param skipFlag if true, the command will not be executed
 * @return true
 */
bool executeCommand(char **execArgs, int skipFlag) {
    int status;
    if (skipFlag) {
        free(execArgs);
        return true;
    }

    if (fork() != 0) {
        // Parent process
        waitpid(-1, &status, 0);
        if (WIFEXITED(status)) {
            exitCode = WEXITSTATUS(status);
        }
    } else {
        // Child process
        status = execvp(executable, execArgs);
        printf("Error: command not found!\n");
        exit(127);
    }

    free(execArgs);
    return true;
}

/**
 * The function parseOptions parses options. 
 * It also calls the method executing the command.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the options were parsed successfully.
 */
bool parseOptions(List *lp, int skipFlag) {
    
    // Store all the options in an arguments array
    int size = 10;
    char **execArgs = malloc(size * sizeof(*execArgs));
    int cnt = 0;
    while (*lp != NULL && !isOperator((*lp)->t)) {
        // Increase size of array if needed
        if (cnt == (size - 2)) {
            size *= 2;
            execArgs = realloc(execArgs, size * sizeof(char*));
        }

        // Store argument in array
        execArgs[cnt] = (*lp)->t;
        cnt++;
        (*lp) = (*lp)->next;
    }
    execArgs[cnt] = NULL;

    return executeCommand(execArgs, skipFlag);
}

/**
 * The function parseRedirections parses a command according to the grammar:
 *
 * <command>        ::= <executable> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the command was parsed successfully.
 */
bool parseCommand(List *lp, int skipFlag) {
    return parseExecutable(lp) && parseOptions(lp, skipFlag);
}

/**
 * The function parsePipeline parses a pipeline according to the grammar:
 *
 * <pipeline>           ::= <command> "|" <pipeline>
 *                       | <command>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the pipeline was parsed successfully.
 */
bool parsePipeline(List *lp, int skipFlag) {
    if (!parseCommand(lp, skipFlag)) {
        return false;
    }

    if (acceptToken(lp, "|")) {
        return parsePipeline(lp, skipFlag);
    }

    return true;
}

/**
 * The function parseFileName parses a filename.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the filename was parsed successfully.
 */
bool parseFileName(List *lp) {
    //TODO: Process the file name appropriately (expected as a required later assignment)
    char *fileName = (*lp)->t;
    return true;
}

/**
 * The function parseRedirections parses redirections according to the grammar:
 *
 * <redirections>       ::= <pipeline> <redirections>
 *                       |  <builtin> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the redirections were parsed successfully.
 */
bool parseRedirections(List *lp) {
    if (isEmpty(*lp)) {
        return true;
    }

    if (acceptToken(lp, "<")) {
        if (!parseFileName(lp)) return false;
        if (acceptToken(lp, ">")) return parseFileName(lp);
        else return true;
    } else if (acceptToken(lp, ">")) {
        if (!parseFileName(lp)) return false;
        if (acceptToken(lp, "<")) return parseFileName(lp);
        else return true;
    }

    return true;
}

/**
 * The function parseBuiltIn parses a builtin.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the builtin was parsed successfully.
 */
bool parseBuiltIn(List *lp, int *exitFlag, int skipFlag) {
    // NULL-terminated array makes it easy to expand this array later
    // without changing the code at other places.
    if (skipFlag) {
        return false;
    }
    char *builtIns[] = {
            "exit",
            "status",   
            NULL
    };

    for (int i = 0; builtIns[i] != NULL; i++) {
        if (acceptToken(lp, builtIns[i])) {
            switch (i) {
            case 0:
                *exitFlag = 1;
                break;
            case 1:
                printf("The most recent exit code is: %d\n", exitCode);
                fflush(stdout);
                break;
            default:
                break;
            }
            return true;
        }
    }

    return false;
}

/**
 * The function parseChain parses a chain according to the grammar:
 *
 * <chain>              ::= <pipeline> <redirections>
 *                       |  <builtin> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the chain was parsed successfully.
 */
bool parseChain(List *lp, int *exitFlag, int skipFlag) {
    if (parseBuiltIn(lp, exitFlag, skipFlag)) {
        if (exitFlag) {
            return true;
        }
        return parseOptions(lp, skipFlag);
    }
    if (parsePipeline(lp, skipFlag)) {
        return parseRedirections(lp);
    }
    return false;
}

/**
 * The function parseInputLine parses an inputline according to the grammar:
 *
 * <inputline>      ::= <chain> & <inputline>
 *                   | <chain> && <inputline>
 *                   | <chain> || <inputline>
 *                   | <chain> ; <inputline>
 *                   | <chain>
 *                   | <empty>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the inputline was parsed successfully.
 */
bool parseInputLine(List *lp, int *exitFlag, int skipFlag) {
    if (isEmpty(*lp)) {
        return true;
    }

    if (!parseChain(lp, exitFlag, skipFlag)) {
        return false;
    }

    // A skipFlag is used in order to decide whether to skip the next command or not.
    // This depends on the previous command's exit code and the operator used.
    skipFlag = 0;
    if (acceptToken(lp, "&") || acceptToken(lp, "&&")) {
        if (exitCode != 0) {
            skipFlag = 1;
        }
        return parseInputLine(lp, exitFlag, skipFlag);
    } else if (acceptToken(lp, "||")) {
        if (exitCode == 0) {
            skipFlag = 1;
        }
        return parseInputLine(lp, exitFlag, skipFlag);
    } else if (acceptToken(lp, ";")) {
        return parseInputLine(lp, exitFlag, skipFlag);
    }

    return true;
}
