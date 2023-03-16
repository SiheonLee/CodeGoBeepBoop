#include "processing.h"

void printRedirect(Redirect redirect) {
    printf("Input: %s\n", redirect.inputFile);
    printf("Output: %s\n", redirect.outputFile);
    printf("Input flag: %d\n", redirect.input);
    printf("Output flag: %d\n", redirect.output);
}

void initRedirect(Redirect *redirect) {
    redirect->inputFile = NULL;
    redirect->outputFile = NULL;
    redirect->input = false;
    redirect->output = false;
}

bool wrongDirect(Redirect redirect) {
    if(redirect.inputFile != NULL && redirect.outputFile != NULL && strcmp(redirect.inputFile, redirect.outputFile) == 0) {
        printf("Error: input and output files cannot be equal!\n");
        return true;
    }
    return false;
}

void redirectIO(Redirect redirect) {
    // Redirect input and output
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
 * @brief Executes a command using fork() and exec().
 * 
 * @param executable the executable of the command
 * @param execArgs the options of the command
 * @param skipFlag if true, the command will not be executed
 * @param filename the filename to redirect to
 * @param redirectFlag 0 = 
 * @return true
 */
bool executeCommand(char ***execArgs, char **executable, int skipFlag, Redirect redirect, int *exitCode, int numCommands) {
    fflush(0);

    // make a pid array
    pid_t pidArray[numCommands];

    int status;
    if (skipFlag || wrongDirect(redirect)) {
        return true;
    }

    // Make The pipes
    int pipefds[numCommands - 1][2];
    for (int i = 0; i < numCommands - 1; i++) {
        if (pipe(pipefds[i]) == -1) {
            printf("Error: pipe failed!\n");
            return true;
        }
    }

    for(int i = 0; i < numCommands; i++) {
        int pid = fork();
        pidArray[i] = pid;
        if (pid < 0) {
            printf("Error: fork failed!\n");
            return true;
        }

        if(pid == 0) {
            redirectIO(redirect);
            if (i == 0 && numCommands > 1) {
                // First command with multiple commands
                dup2(pipefds[i][1], STDOUT_FILENO);
                close(pipefds[i][0]);
                close(pipefds[i][1]);
            } else if (i == numCommands - 1 && numCommands > 1) {
                // Last command with multiple commands
                dup2(pipefds[i - 1][0], STDIN_FILENO);
                close(pipefds[i - 1][0]);
                close(pipefds[i - 1][1]);
            } else if (i > 0 && i < numCommands - 1 && numCommands > 1) {
                // Intermediate commands with multiple commands
                dup2(pipefds[i - 1][0], STDIN_FILENO);
                dup2(pipefds[i][1], STDOUT_FILENO);
                close(pipefds[i - 1][0]);
                close(pipefds[i - 1][1]);
                close(pipefds[i][0]);
                close(pipefds[i][1]);
            }
            *exitCode = execvp(executable[i], execArgs[i]);
            printCommandNotFound();
        }
    }

    for(int i = 0; i < numCommands - 1; i++) {
        close(pipefds[i][0]);
        close(pipefds[i][1]);
    }

    for(int i = 0; i < numCommands; i++) {
        waitpid(pidArray[i], &status, 0);
    }
    if (WIFEXITED(status)) {
        *exitCode = WEXITSTATUS(status);
    }
    return true;
}

