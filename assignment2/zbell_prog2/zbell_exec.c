#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "zbell_exec.h"
#include "zbell_argtok.h"

//
//	EE 4374 Assignment # 2 Command Executer
//	Author: Zachary J Bell 
//


int execBackground(char **args)
{
    int i = 0;
    // check for the ampersand at the end of the set of tokens
    while(args[i] != 0) i++;
    
    // traverse to the end of the tokens
    if(args[i-1][0] == '&') // check the last token
    {
        free(args[i-1]);
        args[i-1] = NULL;  // remove the ampersand

        return 1;
    }
    else return 0; 
}

/**
 * Gets the index of the next special char or returns -1 if not found.
 * Special chars are specified as a string and are intended to be reserved 
 * characters for the shell to use. Examples include pipes (|) and 
 * redirects (>).
 *
 * Args:
 *      args - the tokens
 *
 * Return:
 *      The index of the first special char or -1
 */ 

int next_special(char **args) {
    int i;
    char *p;
    for(i=0 ; args[i]; i++) {
        for (p=SPECIAL; *p; p++) {
            if (*p == *(args[i])) return i;
        }
    }
    return -1;
}


/*
 * This function handles redirects for the shell. Within the function 
 * the child process handles execution whereas the parent handles setting up 
 * and tearing down the file descripters.
 *
 * Args:
 *      args - arguments for the child to execute
 *      file - the file to redir output to
 *      append - a flag indicating whether to append to the end of the file
 */

void redir_to(char **args, char *file, bool append) {
    
    // set up file descriptor
    int fd;
    if (append) fd = open(file, O_WRONLY | O_APPEND | O_CREAT, 0666);
    else fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0666);
    int tmp_stdout = dup(1); // save reference to stdout
    dup2(fd, 1); // redir sdtout to file 

    int status = 0;
    int pid = fork();

    if (pid < 0) exit(1);
    if (pid == 0) {
        execvp(*args, args);
    } else {
        waitpid(pid, &status, 0);
        dup2(tmp_stdout, 1); // restore stdout
        close(fd);
        close(tmp_stdout);
    }
    // child terminated regularly?
    if (WIFEXITED(status)) exit(0);
    else exit(1);
}


/*
 * This function is intended to help parse through the input and deligate 
 * the next task to the appropriate function. For instance, if the arg 
 * array contained a pipe as its next special char, then this function would 
 * identify the existance of the pipe char, split the tokens accordingly, then 
 * call the appropriate function (which would set up the pipe).
 *
 * Args:
 *      args - list of token commands
 *
 * Returns:
 *      0 if successful and -1 if an error was detected
 */

int execute(char **args) {

    bool append;
    int idx = next_special(args);

    if (idx == -1) {
        execvp(*args, args); 
    }

    switch (args[idx][0]) {
        // my half hearted attempt at future proofing
        case '|' :
            printf("Not Implemented: Pipe");
            break;
        case '>' :
            // append? (i.e. >>)           
            append = (args[idx][1] == '>');

            // args before > token
            char** l_args = sub_cpy(args, 0, idx);

            redir_to(l_args, args[idx+1], append);
            
            free_tokens(l_args);
            return 0;
        case '<' :
            printf("Not Implemented: Redirect from file");
            break;
    }    
    return -1;
}


/*
 * This function creates a child process to execute the array of token 
 * args and then waits (or doesn't if & exists at end of input) for the 
 * child process to complete.
 *
 * Args:
 *      args - list of token commands
 *
 * Returns:
 *      0 if successful and -1 if an error was detected
 */

int executeCmd(char **args) {
    
    int status = 0;
    int background = execBackground(args);
    int pid = fork();

    if (pid < 0) return -1; 
    // child
    else if (pid == 0) execute(args);
    // parent
    else {
        if (!background) waitpid(pid, &status, 0);
    }

    if (WIFEXITED(status)) return 0;
    else return -1;
}

