#include "myshell.h"

/**
 * main - entry point
 * @argc: arguments count
 * @argv: arguments vector
 * Return: 0 on success
 */

int main(int argc, char *argv[])
{
	int status, i;
	char **words;
	char *comm;

	(void)argc;
	status = -1;
	while (status == -1)
	{
		if (isatty(STDIN_FILENO) == 1)
		{
			printf("$ ");
		}
		comm = comm_read();
		words = split(comm);
		status = exec_comm(words, argv[0]);
		free(comm);
		for (i = 0; words[i]; i++)
			free(words[i]);
		free(words);
		if (status >= 0)
			exit(status);
	}
	return (0);
}

/**
 * comm_read - reads the command from standard input using getline
 * Return: the command string obtained
 */

char *comm_read(void)
{
	size_t len = 0;
	char *buff = NULL;
	ssize_t rd = 0;

	rd = getline(&buff, &len, stdin);
	if (rd == -1)
	{
		if (feof(stdin))
		{
			free(buff);
			exit(EXIT_SUCCESS);
		}
		free(buff);
		perror("Error reading from stdin\n");
		exit(EXIT_FAILURE);
	}
	buff[strcspn(buff, "\n")] = '\0';
	return (buff);
}

/**
 * exec_comm - searches if command is a built in function
 * @tokens: the array of strings obtained from stdin
 * @name: name of the program
 * Return: -1 or the result from new_fork function
 */

int exec_comm(char **tokens, char *name)
{
	int i;
	char *own_fun_list[] = {
		"exit",
		"env",
		"cd"
	};
	int (*own_fun[])(char **, char *) = {
		&exit_fun,
		&env_fun,
		&cd_fun
	};

	if (!tokens[0])
		return (-1);
	for (i = 0; i < 3; i++)
	{
		if (strcmp(tokens[0], own_fun_list[i]) == 0)
			return ((*own_fun[i])(tokens, name));
	}
	return (new_fork(tokens, name));
}

/**
 * new_fork - if the command is found in the path it forks and executes it
 * @words: the array of strings obtained from the stdin
 * @prog: the programs name
 * Return: -1 on success
 */

int new_fork(char **words, char *prog)
{
	int status;
	pid_t kid;
	char *command = NULL;

	if (access(words[0], X_OK) == 0)
		command = strdup(words[0]);
	else
		command = path_check(words, prog);
	if (command)
	{
		kid = fork();
		if (kid == 0)
		{

			execve(command, words, NULL);
			perror("error in executing\n");
			exit(EXIT_FAILURE);
		}
		if (kid == -1)
		{
			perror("Error: forking");
			exit(EXIT_FAILURE);
		}
		else
		{
			waitpid(kid, &status, 0);
			free(command);
		}
	}
	return (-1);
}

/**
 * path_check - searches for the executable file in the path
 * @words: the array of strings obtained from stdin
 * @prog: the program's name
 * Return: the full path of the file
 */

char *path_check(char **words, char *prog)
{
	char *direct, *pcopy, *p = NULL;
	char **e;
	char fp[PATH_MAX];

	for (e = environ; *e; e++)
	{
		if (strncmp(*e, "PATH=", 5) == 0)
		{
			p = *e + 5;
			break;
		}
	}
	if (p)
	{
		pcopy = strdup(p);
		if (!pcopy)
		{
			perror("Error:\n");
			return (NULL);
		}
		direct = strtok(pcopy, ":");
		while (direct)
		{
			snprintf(fp, sizeof(fp), "%s/%s", direct, words[0]);
			if (access(fp, X_OK) == 0)
			{
				free(pcopy);
				return (strdup(fp));
			}
			direct = strtok(NULL, ":");
		}
		fprintf(stderr, "%s: No such file or directory\n", prog);
		free(pcopy);
		return (NULL);
	}
	else
		fprintf(stderr, "Error: PATH enviroment variable not set\n");
	return (NULL);
}
