//
//	EE 4374 Assignment # 2 Command Executer
//	Author: Zachary J Bell
//

#ifndef ZBELL_EXEC_H
#define ZBELL_EXEC_H


#define SPECIAL "|><"

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

int executeCmd(char **args);

#endif
