#include "myshell.h"

/**
 * exit_fun - exits the shell with success
 * @tokens: array of strings obtained from stdin
 * @name: name of the program
 * Return: nothing because it exits
 */

int exit_fun(char **tokens, char *name)
{
	(void)tokens;
	(void)name;
	return (0);
}

/**
 * env_fun - prints the local enviroment
 * @tokens: array of strings obtained from stdin
 * @name: name of the program
 * Return: -1 on success
 */

int env_fun(char **tokens, char *name)
{
	char **enviro;

	(void)tokens;
	(void)name;
	enviro = environ;

	while (*enviro)
	{
		printf("%s\n", *enviro);
		enviro++;
	}
	return (-1);
}

/**
 * split - splits the string obtained from stdin into array of strings
 * @s: the string obtained from stdin
 * Return: an array of strings
 */

char **split(char *s)
{
	size_t count, i;
	char **words;
	char *token, *scopy;

	scopy = strdup(s);
	token = strtok(scopy, DEL);
	for (count = 1; token != NULL; count++)
		token = strtok(NULL, DEL);
	words = malloc(count * sizeof(char *));
	if (words == NULL)
	{
		fprintf(stderr, "malloc error in split: words\n");
		exit(EXIT_FAILURE);
	}
	free(scopy);
	scopy = strdup(s);
	token = strtok(scopy, DEL);
	for (i = 0; i < (count - 1); i++)
	{
		if (token[0] == '#')
			break;
		words[i] = strdup(token);
		token = strtok(NULL, DEL);
	}
	words[i] = NULL;
	free(scopy);
	return (words);
}

/**
 * cd_fun - changes directory and gets the full path of the directory
 * @tokens: array of strings obtained from stdin
 * @name: name of the program
 * Return: -1
 */

int cd_fun(char **tokens, char *name)
{
	char *h_d = NULL, c_d[PATH_MAX], f_p[2 + PATH_MAX];
	char **enviro = environ;

	if (tokens[1] == NULL)
	{
		for (; *enviro; enviro++)
		{
			if (strncmp(*enviro, "HOME=", 5) == 0)
			{
				h_d = *enviro + 5;
				break;
			}
		}
		if (h_d)
		{
			if (chdir(h_d))
			{
				perror("chdir");
				return (-1);
			}
			else
			{
				fprintf(stderr, "%s: No such file or directory\n", name);
				return (-1);
			}
		}
	}
	else
	{
		if (!getcwd(c_d, sizeof(c_d)))
		{
			perror("getcwd");
			return (-1);
		}
		snprintf(f_p, sizeof(f_p), "%s/%s", c_d, tokens[1]);
		chdir(f_p);
	}
	return (-1);
}
