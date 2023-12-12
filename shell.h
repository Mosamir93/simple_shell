#ifndef SHELL_H
#define SHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/types.h>
#include <linux/limits.h>

extern char **environ;
void exec_comm(char *c, char **words);
void path_check(char ** words);
void split(char *line);
void _read();

#endif
