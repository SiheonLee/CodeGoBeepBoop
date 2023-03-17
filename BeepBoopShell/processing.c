/**
 * @file processing.c
 * @author Andrei Girjoaba, Oscar de Francesca
 * @brief Helper class for shell.c
 * This class contains functions only related to the execution of a command.
 * 
 * @version 1.0
 * @date 2023-03-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "processing.h"

/**
 * @brief Prints the redirect struct.
 */
void printRedirect(Redirect redirect) {
    printf("Input: %s\n", redirect.inputFile);
    printf("Output: %s\n", redirect.outputFile);
    printf("Input flag: %d\n", redirect.input);
    printf("Output flag: %d\n", redirect.output);
}

/**
 * @brief Initializes the redirect struct.
 */
void initRedirect(Redirect *redirect) {
    redirect->inputFile = NULL;
    redirect->outputFile = NULL;
    redirect->input = false;
    redirect->output = false;
}

/**
 * @brief Checks if the output file is the same as the input file.
 * If it is, it prints an error message and returns true.
 * 
 * @param redirect the redirect to be checked.
 * @return true if there is an error, false otherwise.
 */
bool wrongDirect(Redirect redirect) {
    if(redirect.inputFile != NULL && redirect.outputFile != NULL && strcmp(redirect.inputFile, redirect.outputFile) == 0) {
        printf("Error: input and output files cannot be equal!\n");
        return true;
    }
    return false;
}

/**
 * @brief Redirects the standard input and output to the files specified in the redirect struct.
 */
void redirectIO(Redirect redirect) {
    if (redirect.input) {
        int fd0 = open(redirect.inputFile, O_RDONLY);
        dup2(fd0, STDIN_FILENO);
        close(fd0);
    }
    if (redirect.output) {
        int fd1 = open(redirect.outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd1, STDOUT_FILENO);
        close(fd1);
    }
}

/**
 * @brief Prints the error message for a command not found.
 */
void printCommandNotFound() {
    #if EXT_PROMPT
        printf("%s", BHRED);
    #endif
    printf("Error: command not found!\n");
    #if EXT_PROMPT
        printf("%s", RESET);
    #endif
    exit(127);
}

/**
 * @brief Makes all the pipes.
 * 
 * @param pipefds the array of pipes which are created.
 * @param numCommands the number of commands which is equal to the number of pipes + 1
 */
void makePipes(int pipefds[][2], int numCommands) {
    for (int i = 0; i < numCommands - 1; i++) {
        if (pipe(pipefds[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * @brief Closes all the pipes.
 * 
 * @param pipefds the array of pipes
 * @param numCommands the number of commands which is equal to the number of pipes + 1
 */
void closePipes(int pipefds[][2], int numCommands) {
    for (int i = 0; i < numCommands - 1; i++) {
        close(pipefds[i][0]);
        close(pipefds[i][1]);
    }
}

/**
 * @brief Parent process waits for all the children to finish.
 * 
 * @param pidArray the array of process ids of children.
 * @param numCommands the number of commands.
 * @param exitCode the exit code of the last command to be updated.
 */
void waitAllChildren(pid_t *pidArray ,int numCommands, int *exitCode) {
    int status;
    for(int i = 0; i < numCommands; i++) {
        waitpid(pidArray[i], &status, 0);
    }
    if (WIFEXITED(status)) {
        *exitCode = WEXITSTATUS(status);
    }
}

/**
 * @brief Executes all the commands in the command array.
 * Multiple commands are executed only in the case when pipes are used.
 * 
 * @param execArgs an array storing all the options for each command.
 * @param executable an array storing the executables for each command.
 * @param skipFlag a flag which is set to true if the command is to be skipped.
 * @param redirect the redirect struct describing output or input files.
 * @param exitCode will be updated depending on the success of the command.
 * @param numCommands the number of commands to be executed.
 */
bool executeCommand(char ***execArgs, char **executable, int skipFlag, Redirect redirect, int *exitCode, int numCommands) {
    fflush(0);

    // Array containing the process ids of the child processes
    pid_t pidArray[numCommands];

    if (skipFlag || wrongDirect(redirect)) {
        return true;
    }

    // Make The pipes
    int pipefds[numCommands - 1][2];
    makePipes(pipefds, numCommands);

    for(int i = 0; i < numCommands; i++) {
        int pid = fork();
        pidArray[i] = pid;
        if (pid < 0) {
            printf("Error: fork failed!\n");
            return true;
        }

        if(pid == 0) {
            // Child process
            redirectIO(redirect);

            if (i == 0 && numCommands > 1) {
                // First command with multiple commands
                dup2(pipefds[i][1], STDOUT_FILENO);
                closePipes(pipefds, numCommands);

            } else if (i == numCommands - 1 && numCommands > 1) {
                // Last command with multiple commands
                dup2(pipefds[i - 1][0], STDIN_FILENO);
                closePipes(pipefds, numCommands);

            } else if (i > 0 && i < numCommands - 1 && numCommands > 1) {
                // Intermediate commands with multiple commands
                dup2(pipefds[i - 1][0], STDIN_FILENO);
                dup2(pipefds[i][1], STDOUT_FILENO);
                closePipes(pipefds, numCommands);
            }

            *exitCode = execvp(executable[i], execArgs[i]);
            printCommandNotFound();
        }
    }

    closePipes(pipefds, numCommands);
    waitAllChildren(pidArray, numCommands, exitCode);
    
    return true;
}

