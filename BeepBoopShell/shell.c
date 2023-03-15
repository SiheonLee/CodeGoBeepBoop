#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "scanner.h"
#include "bonus.h"
#include "builtin.h"

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
bool parseExecutable(List *lp, char **executable) {
    *executable = (*lp)->t;
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
bool executeCommand(char **execArgs, char *executable, int skipFlag) {
    int status;
    if (skipFlag) {
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
        #if EXT_PROMPT
            printf("%s", BHRED);
        #endif

        printf("Error: command not found!\n");

        #if EXT_PROMPT
            printf("%s", RESET);
        #endif
        exit(127);
    }

    return true;
}

/**
 * The function parseOptions parses options. 
 * It also calls the method executing the command.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the options were parsed successfully.
 */
bool parseOptions(List *lp, char ***myArgs, int skipFlag) {
    // Store all the options in an arguments array
    int size = 10;
    *myArgs = malloc(size * sizeof(char*));
    int cnt = 0;
    while (*lp != NULL && !isOperator((*lp)->t)) {
        // Increase size of array if needed
        if (cnt == (size - 2)) {
            size *= 4;
            *myArgs = realloc(*myArgs, size * sizeof(char*));
        }

        // Store argument in array
        (*myArgs)[cnt] = (*lp)->t;
        cnt++;
        (*lp) = (*lp)->next;
    }
    (*myArgs)[cnt] = NULL;
    return true;
}

/**
 * The function parseRedirections parses a command according to the grammar:
 *
 * <command>        ::= <executable> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the command was parsed successfully.
 */
bool parseCommand(List *lp, int skipFlag, char **executable, char ***execArgs) {
    // Parse executable
    if(!parseExecutable(lp, executable)) {
        return false;
    }

    // Parse options
    if(!parseOptions(lp, execArgs, skipFlag)) {
//        free(execArgs);
        return false;
    }

//    free(execArgs);
    return true;
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
bool parsePipeline(List *lp, int skipFlag, char **executable, char ***execArgs) {
    if (!parseCommand(lp, skipFlag,  executable, execArgs)) {
        return false;
    }

    if (acceptToken(lp, "|")) {
        return parsePipeline(lp, skipFlag, executable, execArgs);
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
bool parseBuiltIn(List *lp, char **builtin, int *exitFlag, int skipFlag) {
    if (skipFlag) {
        return false;
    }
    if(isBuiltIn((*lp)->t)) {
        *builtin = (*lp)->t;
        (*lp) = (*lp)->next;
        return true;
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
bool parseChain(List *lp, char ***execArgs, char **executable, int *exitFlag, int skipFlag, History hist) {

    // Built-in
    char *builtin;
    if (parseBuiltIn(lp, &builtin, exitFlag, skipFlag)) {
        char **execArgs;
        if(!parseOptions(lp, &execArgs, skipFlag)) {
//            free(execArgs);
            return false;
        }

        if (executeBuiltIn(builtin, execArgs, exitFlag, exitCode, hist) == -1) {
            exitCode = 2;
        }
        if (exitFlag) {
//            free(execArgs);
            return true;
        }

//        free(execArgs);
        return true;
    }

    // Pipeline
    if (parsePipeline(lp, skipFlag, executable, execArgs)) {
        // Execute command
        if(!executeCommand(*execArgs, *executable, skipFlag)) {
            return false;
        }
        free(*execArgs);
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
bool parseInputLine(List *lp, int *exitFlag, int skipFlag, History hist) {
    if (isEmpty(*lp)) {
        return true;
    }

    char **execArgs;
    char *executable;

    if (!parseChain(lp, &execArgs, &executable, exitFlag, skipFlag, hist)) {
        return false;
    }

    // A skipFlag is used in order to decide whether to skip the next command or not.
    // This depends on the previous command's exit code and the operator used.
    skipFlag = 0;
    if (acceptToken(lp, "&") || acceptToken(lp, "&&")) {
        if (exitCode != 0) {
            skipFlag = 1;
        }
        return parseInputLine(lp, exitFlag, skipFlag, hist);
    } else if (acceptToken(lp, "||")) {
        if (exitCode == 0) {
            skipFlag = 1;
        }
        return parseInputLine(lp, exitFlag, skipFlag, hist);
    } else if (acceptToken(lp, ";")) {
        return parseInputLine(lp, exitFlag, skipFlag, hist);
    }

    return true;
}
