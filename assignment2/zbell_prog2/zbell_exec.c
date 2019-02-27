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

void print_args(char** args) {
    printf("Debug : ");
    for ( ; *args; args++) printf("%s ", *args);
    printf("\n");
}

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

int redir_from(char **args, char *file) {
    return -1;
}

int redir_to(char **args, char *file, bool append) {
    int fd;

    if (append) fd = open(file, O_WRONLY | O_APPEND);
    else fd = open(file, O_WRONLY);

    printf("Debug1");

    int tmp_stdout = dup(1); // save reference to stdout
    dup2(fd, 1); // redir sdtout to file 

    int pid = fork();

    if (pid < 0) return -1;
    if (pid == 0) {
        execvp(*args, args);
        fflush(stdout);
    } else {
        wait(NULL);
        dup2(tmp_stdout, 1); // restore stdout
        close(fd);
        close(tmp_stdout);
    }

    return 0;
}

int execute(char **args) {

    bool append;
    int idx = next_special(args);

    if (idx == -1) {
        execvp(*args, args); 
    }

    switch (args[idx][0]) {
        case '|' :
            printf("Not Implemented: Pipe");
            break;
        case '>' :
            // append? (i.e. >>)           
            append = (args[idx][1] == '>');

            char** l_args = sub_cpy(args, 0, idx);
            int rcode = redir_to(l_args, args[idx+1], append);
            
            free_tokens(l_args);

            return rcode;
        case '<' :
            printf("Not Implemented: Redirect from file");
            break;
        default :
            return -1;
    }
}

int executeCmd(char **args) {
    
    int rcode;

    if (execBackground(args)) {
        // child process executes and parent doesn't wait
        if (!fork()) rcode = execute(args);
        else rcode = 0;
    } else {
        if (fork()) {
            // FIXME: make sure that wait actually works
            int pid = wait(NULL);
            if (pid < 0) rcode = -1;
            else rcode = 0;
        }
        else rcode = execute(args);
    }

    return rcode;
}
