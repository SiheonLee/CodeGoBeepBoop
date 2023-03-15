/**
 * @file builtin.c
 * @brief Contains all methods related to the execution of built-in commands.
    *     It also contains a method to check if a command is a built-in command.
 * @version 0.1
 * @date 2023-02-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "builtin.h"
#include "bonus.h"
#include "history.h"

#define BUILTIN_NOT_FOUND 126
#define BUILTIN_EXECUTED 0
#define BUILTIN_FAILED -1

/**
 * @brief Checks if a command is a built-in command.
 * 
 * @param s The string that is being checked.
 * @return if it is a built-in command
 */
bool isBuiltIn(char *s) {
    // NULL-terminated array makes it easy to expand this array later
    // without changing the code at other places.
    char *builtIns[] = {
            "exit",
            "status",   
            "cd",
            "hist",
            NULL
    };

    for (int i = 0; builtIns[i] != NULL; i++) {
        if (strcmp(s, builtIns[i]) == 0) return true;
    }
    return false;
}

/**
 * @brief Executes the status command.
 * 
 * @param exitCode the status code of the previous command.
 * @return true if the command was executed successfully.
 */
bool executeStatus(int exitCode) {
    printf("The most recent exit code is: %d\n", exitCode);
    fflush(stdout);
    return true;
}


/**
 * @brief Executes the cd command to change directory.
 * 
 * @param args contains the path to the directory to be changed to from the current directory.
 * @return true for successfull change of directory.
 * @return false for failed change of directory.
 */
bool executeCD(char *args[]) {
    if (args[0] == NULL) {
        #if EXT_PROMPT
            printf("%s", BHRED);
        #endif

        printf("Error: cd requires folder to navigate to!\n");

        #if EXT_PROMPT
            printf("%s", RESET);
        #endif
        return false;
    }

    if (args[1] != NULL) {
        #if EXT_PROMPT
            printf("%s", BHRED);
        #endif

        printf("cd: too many arguments\n");

        #if EXT_PROMPT
            printf("%s", RESET);
        #endif
        return false;
    }

    if (chdir(args[0]) == -1) {
        #if EXT_PROMPT
            printf("%s", BHRED);
        #endif

        printf("Error: cd directory not found!\n");

        #if EXT_PROMPT
            printf("%s", RESET);
        #endif
        return false;
    }

    return true;
}

/**
 * @brief Executes a built-in command.
 * 
 * @param builtin the built-in command to be executed.
 * @param args the options of the built-in command as an array of strings.
 * @param exitFlag it is set to 1 if the command is exit.
 * @param exitCode the status code of the previous command.
 * @return the status code of the execution of the built-in command.
 */
int executeBuiltIn(char *builtin, char *args[], int *exitFlag, int exitCode, History hist) {
    if (strcmp(builtin, "exit") == 0) {
        *exitFlag = 1;
        return BUILTIN_EXECUTED;
    }
    
    if (strcmp(builtin, "status") == 0) {
        if (executeStatus(exitCode)) return BUILTIN_EXECUTED;
        return BUILTIN_FAILED;
    }

    if (strcmp(builtin, "cd") == 0) {
        if (executeCD(args)) return BUILTIN_EXECUTED;
        return BUILTIN_FAILED;
    }

    if (strcmp(builtin, "hist") == 0) {
        if (printHistory(hist, args)) return BUILTIN_EXECUTED;
        return BUILTIN_FAILED;
    }

    return BUILTIN_NOT_FOUND;
}