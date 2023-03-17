/**
 * @file shell.c
 * @brief The main class that executes the grammar of our shell.
 * Each InputLine is parsed in here.
 * 
 * The grammar is defined as follows:
 * 
 *  <command>      ::= <executable> <options>
 * 
 *  <pipeline>     ::= <command> | <pipeline>
 *                   | <command>
 *  
 *  <redirections> ::= < <filename> > <filename>
 *                   | > <filename> < <filename>
 *                   | < <filename>
 *                   | > <filename>
 *                   | <empty>
 * 
 *  <chain>        ::= <pipeline> <redirections>
 *                   | <builtin> <options>
 * 
 * <inputline>     ::= <chain> & <inputline>
 *                   | <chain> && <inputline>
 *                   | <chain> || <inputline>
 *                   | <chain> ; <inputline>
 *                   | <chain> 
 *                   | <empty>
 * 
 * @version 1.1
 * @date 2023-03-17
 * 
 */

#include "processing.h"

#define MAX_COMMANDS 100

// Exit code of the last executed command
int exitCode = 0;

/**
 * Frees the memory allocated for the list of arguments of each command.
 * 
 * @param execArgs the list of arguments of each command.
 * @param numCommands the number of commands.
 */
void freeArgsOptions(char ***execArgs, int *numCommands) {
    if(*numCommands == 0) {
        free(execArgs[0]);
    }
    for (int i = 0; i < *numCommands; i++) {
        free(execArgs[i]);
    }
}

/**
 * The function acceptToken checks whether the current token matches a target identifier,
 * and goes to the next token if this is the case.
 * 
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
 * The function parseExecutable parses an executable
 * and stores it in an array of executables.
 * 
 * @param lp List pointer to the start of the tokenlist.
 * @param executable an array storing the executables.
 * @param numCommands the number of commands used for storing at the correct index.
 * @return a bool denoting whether the executable was parsed successfully.
 */
bool parseExecutable(List *lp, char **executable, int *numCommands) {
    executable[*numCommands] = (*lp)->t;
    return true;
}

/**
 * Checks whether the input string \param s is an operator.
 * 
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
 * Parses the options of a command and stores them in the array at the position
 * associated with the specific command.
 * 
 * @param lp List pointer to the start of the tokenlist.
 * @param myArgs an array storing at each position the arguments of a command.
 * @param numCommands the number of commands used for storing at the correct index.
 * @return a bool denoting whether the options were parsed successfully.
 */
bool parseOptions(List *lp, char ***myArgs, int *numCommands) {
    // Store all the options in an arguments array
    int size = 10;
    myArgs[*numCommands] = malloc(size * sizeof(char*));
    int cnt = 0;
    while (*lp != NULL && !isOperator((*lp)->t)) {
        // Increase size of array if needed
        if (cnt == (size - 2)) {
            size *= 4;
            myArgs[*numCommands] = realloc(*myArgs, size * sizeof(char*));
        }

        // Store argument in array
        myArgs[*numCommands][cnt] = (*lp)->t;
        cnt++;
        (*lp) = (*lp)->next;
    }
    myArgs[*numCommands][cnt] = NULL;
    return true;
}

/**
 * Parses a command according to the grammar:
 *
 * <command>        ::= <executable> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @param skipFlag a flag saying whether the command should be skipped.
 * @param executable an array storing the executables.
 * @param execArgs an array storing at each position the arguments of a command.
 * @param numCommands the number of commands stored.
 * @return a bool denoting whether the command was parsed successfully.
 */
bool parseCommand(List *lp, int skipFlag, char **executable, char ***execArgs, int *numCommands) {
    if(!parseExecutable(lp, executable, numCommands)) {
        return false;
    }

    if(!parseOptions(lp, execArgs, numCommands)) {
        return false;
    }

    (*numCommands)++;
    return true;
}

/**
 * The function parsePipeline parses a pipeline according to the grammar:
 *
 * <pipeline>           ::= <command> "|" <pipeline>
 *                       | <command>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @param skipFlag a flag saying whether the command should be skipped.
 * @param executable an array storing the executables.
 * @param execArgs an array storing at each position the arguments of a command.
 * @param numCommands the number of commands stored.
 * @return a bool denoting whether the pipeline was parsed successfully.
 */
bool parsePipeline(List *lp, int skipFlag, char **executable, char ***execArgs, int *numCommands) {
    // Check if list is empty
    if (isEmpty(*lp)) {
        return false;
    }

    if (!parseCommand(lp, skipFlag,  executable, execArgs, numCommands)) {
        return false;
    }

    if (acceptToken(lp, "|")) {
        return parsePipeline(lp, skipFlag, executable, execArgs, numCommands);
    }

    return true;
}

/**
 * The function parseFileName parses a filename.
 * 
 * @param lp List pointer to the start of the tokenlist.
 * @param redirect a struct storing the input and output files.
 * @param inputFlag a flag saying whether the filename is an input file.
 * @return a bool denoting whether the filename was parsed successfully.
 */
bool parseFileName(List *lp, Redirect *redirect, int inputFlag) {
    if (isEmpty(*lp)) {
        return false;
    }

    if(inputFlag) {
        (*redirect).inputFile = (*lp)->t;
        (*redirect).input = true;
    } else {
        (*redirect).outputFile = (*lp)->t;
        (*redirect).output = true;
    }
    
    (*lp) = (*lp)->next;
    return true;
}

/**
 * The function parseRedirections parses redirections according to the grammar:
 *
 * <redirections>       ::= <pipeline> <redirections>
 *                       |  <builtin> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @param redirect a struct storing the input and output files.
 * @return a bool denoting whether the redirections were parsed successfully.
 */
bool parseRedirections(List *lp, Redirect *redirect) {
    if (isEmpty(*lp)) {
        return true;
    }

    if (acceptToken(lp, "<")) {
        if (!parseFileName(lp, redirect, 1)) return false;

        if (acceptToken(lp, ">")) return parseFileName(lp, redirect, 0);
        else return true;
    } else if (acceptToken(lp, ">")) {
        if (!parseFileName(lp, redirect, 0)) return false;
        if (acceptToken(lp, "<")) return parseFileName(lp, redirect, 1);
        else return true;
    }

    return true;
}

/**
 * The function parseBuiltIn parses a builtin.
 * 
 * @param lp List pointer to the start of the tokenlist.
 * @param builtin a string storing the builtin.
 * @param skipFlag a flag saying whether the command should be skipped.
 * @return a bool denoting whether the builtin was parsed successfully.
 */
bool parseBuiltIn(List *lp, char **builtin, int skipFlag) {
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
 * @param executable an array storing the executables.
 * @param execArgs an array storing at each position the arguments of a command.
 * @param exitFlag a flag saying whether the shell should exit.
 * @param skipFlag a flag saying whether the command should be skipped.
 * @param hist a struct storing the history.
 * @param numCommands the number of commands stored.
 * @return a bool denoting whether the chain was parsed successfully.
 */
bool parseChain(List *lp, char ***execArgs, char **executable, int *exitFlag, int skipFlag, History hist, int *numCommands) {

    // Built-in
    char *builtin;
    if (parseBuiltIn(lp, &builtin, skipFlag)) {
        if(!parseOptions(lp, execArgs, numCommands)) {
            freeArgsOptions(execArgs, numCommands);
            return false;
        }

        // Execute Built-in
        int builtInExitCode = executeBuiltIn(builtin, execArgs, exitFlag, exitCode, hist);
        if (builtInExitCode == -1) {
            exitCode = 2;
        } else if (builtInExitCode == 3) {
            exitCode = 0;
        }

        freeArgsOptions(execArgs, numCommands);
        return true;
    }

    // Pipeline
    if (parsePipeline(lp, skipFlag, executable, execArgs, numCommands)) {
        Redirect redirect;
        initRedirect(&redirect);

        if (!parseRedirections(lp, &redirect)) {
            freeArgsOptions(execArgs, numCommands);
            return false;
        }

        // Execute Command
        if(!executeCommand(execArgs, executable, skipFlag, redirect, &exitCode, *numCommands)) {
            freeArgsOptions(execArgs, numCommands);
            return false;
        }
        freeArgsOptions(execArgs, numCommands);  
        return true;
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
 * @param exitFlag a flag saying whether the shell should exit.
 * @param skipFlag a flag saying whether the command should be skipped.
 * @param hist a struct storing the history.
 * @return a bool denoting whether the inputline was parsed successfully.
 */
bool parseInputLine(List *lp, int *exitFlag, int skipFlag, History hist) {
    if (isEmpty(*lp)) {
        return true;
    }

    char ***execArgs = malloc(MAX_COMMANDS*sizeof(char**));
    char **executable = malloc(MAX_COMMANDS*sizeof(char*));
    int numCommands = 0;

    if (!parseChain(lp, execArgs, executable, exitFlag, skipFlag, hist, &numCommands)) {
        free(execArgs);
        free(executable);
        return false;
    }
    free(execArgs);
    free(executable);

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
