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

    int status;
    if (skipFlag || wrongDirect(redirect)) {
        return true;
    }

    int pfd[2];
    if(pipe(pfd) == -1) {
        printf("Error: pipe failed!\n");
        return true;
    }

    int pid1 = fork();
    if (pid1 < 0) {
        printf("Error: fork failed!\n");
        return true;
    }

    if(pid1 == 0) {
        redirectIO(redirect);
        if (numCommands > 1) {
            dup2(pfd[1], STDOUT_FILENO);
            close(pfd[0]);
            close(pfd[1]);
        }
        *exitCode = execvp(executable[0], execArgs[0]);
        printCommandNotFound();
    }

    int pid2;
    for(int i = 1; i < numCommands; i++) {
        pid2 = fork();
        if (pid2 < 0) {
            printf("Error: fork failed!\n");
            return true;
        }

        if(pid2 == 0) {
            dup2(pfd[0], STDIN_FILENO);
            close(pfd[0]);
            close(pfd[1]);
            *exitCode = execvp(executable[i], execArgs[i]);
            printCommandNotFound();
        }
    }

    close(pfd[0]);
    close(pfd[1]);

    waitpid(pid1, &status, 0);
    if(numCommands > 1) waitpid(pid2, &status, 0);
    if (WIFEXITED(status)) {
        *exitCode = WEXITSTATUS(status);
    }

    return true;
}