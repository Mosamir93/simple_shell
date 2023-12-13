#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <linux/limits.h>

extern char **environ;
#define DEL " \t\n\r\a\""

int new_fork(char **words, char *prog);
char *comm_read(void);
int exec_comm(char **tokens, char *name);
char **split(char *s);
char *path_check(char **words, char *prog);
int exit_fun(char **words, char *name);
int env_fun(char **words, char *name);
int cd_fun(char **words, char *name);

#endif
