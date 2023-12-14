#include "myshell.h"

/**
 * unsetenv_fun - unsets enviroment variable
 * @token: arrays of string from stdin
 * @name: name of the program
 * Return: -1 on success
*/

int unsetenv_fun(char **tokens, char *name)
{
    char **enviro, ***new;
    int i, num;

    if (!tokens[1] || tokens[2])
    {
        fprintf(stderr,"%s: VARIABLE\n", name);
        return (-1);
    }
    enviro = environ;
    for (num = 0; *enviro; num++, enviro++)
    ;
    new = malloc((num + 1) * sizeof(char*));
    if (new == NULL)
    {
        perror("Error in memory allocation");
        exit(EXIT_FAILURE);
    }
    enviro = environ;
    for (i = 0; i < num; i++, enviro++)
    {
        if (strncmp(*enviro, tokens[1], strlen(tokens[1])))
        {
            new[i] = *enviro;
            i++;
        }
    }
    new[i] = NULL;
    environ = new;
    free(new);
    return (-1);
}

/**
 * setenv_fun - sets enviroment variable
 * @token: array of strings from stdin
 * @name: name of the program
 * Return: -1 on success
*/

int setenv_fun(char **tokens, char *name)
{
    char **enviro, ***new;
    int i, num, len1, len2;

    if (!tokens[1] || !tokens[2] || tokens[3])
    {
        fprintf(stderr,"%s: VARIABLE VALUE\n", name);
        return (-1);
    }
    enviro = environ;
    for (num = 0; *enviro; num++, enviro++)
    ;
    new = malloc((num + 2) * sizeof(char *));
    if (new == NULL)
    {
        perror("Error in memory allocation");
        exit(EXIT_FAILURE);
    }
    enviro = environ;
    for (i = 0; i < num; i++, enviro++)
    {
        new[i] = *enviro;
    }
    len1 = strlen(tokens[1]);
    len2 = strlen(tokens[2]);
    new[i] = malloc(len1 + len2 + 2);
    snprintf(new[i], len1, len2 + 2, "%s=%s", tokens[1], tokens[2]);
    new[++i] = NULL;
    environ = *new;
    free(new);
    return (-1);
}
