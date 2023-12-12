#include "shell.h"

void exec_comm(char *c, char **words)
{
	pid_t kid;
	int status;

	kid = fork();
	if (kid == -1)
	{
		perror("Error:");
		return;
	}
	if (kid == 0)
	{
		execve(c, words, NULL);
        perror("Error");
		fprintf(stderr, "Error: unable to execute command\n");
		_exit(EXIT_FAILURE);
	}
	else
	{
		wait(&status);
	}

}
void path_check(char ** words)
{
	char *pcopy, *direct, *p = NULL;
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
			perror("Error:");
			return;
		}
		direct = strtok(pcopy, ":");
		while (direct)
		{
			snprintf(fp, sizeof(fp), "%s/%s", direct, words[0]);
			if(access(fp, X_OK) == 0)
			{
				exec_comm(fp, words);
				free(pcopy);
				return;
			}
			direct = strtok(NULL, ":");
		}
		fprintf(stderr, "Error: %s command not found\n", words[0]);
		free(pcopy);
	}
	else
		fprintf(stderr, "Error: PATH enviroment variable not set\n");
}
void split(char *line)
{
	char *scopy, *s2copy, *token;
	char **words;
	size_t i, count;

	if (line == NULL)
		return;
	scopy = strdup(line);
	if (scopy == NULL)
		return;
	count = 0;
	token = strtok(scopy, " ");
	while (token)
	{  
		count++;
		token = strtok(NULL, " ");
	}
	words = (char **)malloc((count + 1) * sizeof(char *));
	if (words == NULL)
		exit(-1);
	free(scopy);
	s2copy = strdup(line);
	if (s2copy == NULL)
		exit(-1);
	token = strtok(s2copy, " ");
	for (i = 0; i < count; i++)
	{
		words[i] = strdup(token);
		if (words[i] == NULL)
			exit(-1);
		token = strtok(NULL, " ");
	}
	words[count] = NULL;
	free(s2copy);
	path_check(words);
	free(line);
	for (i = 0; words[i]; i++)
        free(words[i]);
    free(words);
}

void _read()
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while (1)
	{
		if (isatty(0))
		{   
			printf("$ ");
		}
		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			perror("getline");
			free(line);
			exit(EXIT_FAILURE);
		}
        line[strcspn(line, "\n")] = '\0';
		split(line);
	}
}

int main ()
{
	_read();

	return (0);
}
