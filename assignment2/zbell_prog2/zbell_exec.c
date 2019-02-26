#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "zbell_exec.h"

//
//	EE 4374 Assignment # 2 Command Executer
//	Author: Zachary J Bell 
//

int execBackground(char **args)
{
    int i = 0;
    // check for the ampersand at the end of the set of tokens
    while(args[i]) i++;
    
    // traverse to the end of the tokens
    if(args[i-1][0] == '&') // check the last token
    {
        free(args[i]);
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

int execute(char **args) {

    int idx = next_special(args);

    if (idx == -1) execvp(*args, args); 

    switch(args[idx]) {
        case '|':
            printf("Not Implemented: Pipe");
        case '>':
            printf("Not Implemented: Redirect to file");
        case '<':
            printf("Not Implemented: Redirect from file");

    }
}

int executeCmd(char **args) {
    if (execBackground(args)) {
        // child process executes and parent doesn't wait
        if (!fork()) return execute(args);
    } else {
        // child process executes and parent waits
        if (fork()) wait(NULL);
        else return execute(args);
    }
}
