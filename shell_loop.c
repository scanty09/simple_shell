#include "shell.h"

/**
 * shellLoop - main shell loop
 * @shellInfo: the parameter & return info struct
 * @argumentVector: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int shellLoop(ShellInfo_t *shellInfo, char **argumentVector)
{
	ssize_t readResult = 0;
	int builtinReturn = 0;

	while (readResult != -1 && builtinReturn != -2)
	{
		clearShellInfo(shellInfo);
		if (interactiveMode(shellInfo))
			printString("$ ");
		errorPutchar(BUFFER_FLUSH);
		readResult = getInput(shellInfo);
		if (readResult != -1)
		{
			setShellInfo(shellInfo, argumentVector);
			builtinReturn = findBuiltin(shellInfo);
			if (builtinReturn == -1)
				findCommand(shellInfo);
		}
		else if (interactiveMode(shellInfo))
			putcharString('\n');
		freeShellInfo(shellInfo, 0);
	}
	writeHistory(shellInfo);
	freeShellInfo(shellInfo, 1);
	if (!interactiveMode(shellInfo) && shellInfo->status)
		exit(shellInfo->status);
	if (builtinReturn == -2)
	{
		if (shellInfo->errNum == -1)
			exit(shellInfo->status);
		exit(shellInfo->errNum);
	}
	return (builtinReturn);
}

/**
 * findBuiltin - finds a builtin command
 * @shellInfo: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int findBuiltin(ShellInfo_t *shellInfo)
{
	int i, builtinReturn = -1;
	BuiltinCommand_t builtinTable[] = {
		{"exit", exitShell},
		{"env", shellEnvironment},
		{"help", shellHelp},
		{"history", shellHistory},
		{"setenv", setShellEnvironment},
		{"unsetenv", unsetShellEnvironment},
		{"cd", changeDirectory},
		{"alias", shellAlias},
		{NULL, NULL}
	};

	for (i = 0; builtinTable[i].type; i++)
		if (stringCompare(shellInfo->argv[0], builtinTable[i].type) == 0)
		{
			shellInfo->lineCount++;
			builtinReturn = builtinTable[i].func(shellInfo);
			break;
		}
	return (builtinReturn);
}

/**
 * findCommand - finds a command in PATH
 * @shellInfo: the parameter & return info struct
 *
 * Return: void
 */
void findCommand(ShellInfo_t *shellInfo)
{
	char *path = NULL;
	int i, k;

	shellInfo->path = shellInfo->argv[0];
	if (shellInfo->lineCountFlag == 1)
	{
		shellInfo->lineCount++;
		shellInfo->lineCountFlag = 0;
	}
	for (i = 0, k = 0; shellInfo->arg[i]; i++)
		if (!isDelimiter(shellInfo->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = findPath(shellInfo, getEnvironmentVariable(shellInfo, "PATH="), shellInfo->argv[0]);
	if (path)
	{
		shellInfo->path = path;
		forkCommand(shellInfo);
	}
	else
	{
		if ((interactiveMode(shellInfo) || getEnvironmentVariable(shellInfo, "PATH=")
			|| shellInfo->argv[0][0] == '/') && isCommand(shellInfo, shellInfo->argv[0]))
			forkCommand(shellInfo);
		else if (*(shellInfo->arg) != '\n')
		{
			shellInfo->status = 127;
			printErrorMessage(shellInfo, "not found\n");
		}
	}
}

/**
 * forkCommand - forks an exec thread to run cmd
 * @shellInfo: the parameter & return info struct
 *
 * Return: void
 */
void forkCommand(ShellInfo_t *shellInfo)
{
	pid_t childPid;

	childPid = fork();
	if (childPid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (childPid == 0)
	{
		if (execve(shellInfo->path, shellInfo->argv, getShellEnvironment(shellInfo)) == -1)
		{
			freeShellInfo(shellInfo, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(shellInfo->status));
		if (WIFEXITED(shellInfo->status))
		{
			shellInfo->status = WEXITSTATUS(shellInfo->status);
			if (shellInfo->status == 126)
				printErrorMessage(shellInfo, "Permission denied\n");
		}
	}
}

