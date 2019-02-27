//
//	EE 4374 Assignment # 2 Command Executer
//	Author: Zachary J Bell
//

#ifndef ZBELL_EXEC_H
#define ZBELL_EXEC_H


#define SPECIAL "|><"

int next_special(char **args);

int execBackground(char **args);

int executeCmd(char **args);

#endif
