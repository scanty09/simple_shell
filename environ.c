#include "shell.h"

/**
 * displayEnvironment - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int displayEnvironment(info_t *info)
{
    print_list_str(info->env);
    return (0);
}

/**
 * getEnvironmentVariable - gets the value of an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: environment variable name
 *
 * Return: the value
 */
char *getEnvironmentVariable(info_t *info, const char *name)
{
    list_t *node = info->env;
    char *p;

    while (node)
    {
        p = starts_with(node->str, name);
        if (p && *p)
            return (p);
        node = node->next;
    }
    return (NULL);
}

/**
 * setEnvironmentVariable - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int setEnvironmentVariable(info_t *info)
{
    if (info->argc != 3)
    {
        _eputs("Incorrect number of arguments\n");
        return (1);
    }
    if (_setenv(info, info->argv[1], info->argv[2]))
        return (0);
    return (1);
}

/**
 * unsetEnvironmentVariable - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int unsetEnvironmentVariable(info_t *info)
{
    int i;

    if (info->argc == 1)
    {
        _eputs("Too few arguments.\n");
        return (1);
    }
    for (i = 1; i <= info->argc; i++)
        _unsetenv(info, info->argv[i]);

    return (0);
}

/**
 * populateEnvironmentList - populates environment linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int populateEnvironmentList(info_t *info)
{
    list_t *node = NULL;
    size_t i;

    for (i = 0; environ[i]; i++)
        add_node_end(&node, environ[i], 0);
    info->env = node;
    return (0);
}

