#include "shell.h"

/**
 * splitString - splits a string into words. Repeat delimiters are ignored
 * @inputString: the input string
 * @delimiterString: the delimeter string
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **splitString(char *inputString, char *delimiterString)
{
	int i, j, k, m, numWords = 0;
	char **resultArray;

	if (inputString == NULL || inputString[0] == 0)
		return (NULL);
	if (!delimiterString)
		delimiterString = " ";
	for (i = 0; inputString[i] != '\0'; i++)
		if (!isDelimiter(inputString[i], delimiterString) &&
		    (isDelimiter(inputString[i + 1], delimiterString) || !inputString[i + 1]))
			numWords++;

	if (numWords == 0)
		return (NULL);
	resultArray = malloc((1 + numWords) * sizeof(char *));
	if (!resultArray)
		return (NULL);
	for (i = 0, j = 0; j < numWords; j++)
	{
		while (isDelimiter(inputString[i], delimiterString))
			i++;
		k = 0;
		while (!isDelimiter(inputString[i + k], delimiterString) && inputString[i + k])
			k++;
		resultArray[j] = malloc((k + 1) * sizeof(char));
		if (!resultArray[j])
		{
			for (k = 0; k < j; k++)
				free(resultArray[k]);
			free(resultArray);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			resultArray[j][m] = inputString[i++];
		resultArray[j][m] = 0;
	}
	resultArray[j] = NULL;
	return (resultArray);
}

/**
 * splitString2 - splits a string into words
 * @inputString: the input string
 * @delimiter: the delimeter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **splitString2(char *inputString, char delimiter)
{
	int i, j, k, m, numWords = 0;
	char **resultArray;

	if (inputString == NULL || inputString[0] == 0)
		return (NULL);
	for (i = 0; inputString[i] != '\0'; i++)
		if ((inputString[i] != delimiter && inputString[i + 1] == delimiter) ||
		    (inputString[i] != delimiter && !inputString[i + 1]) || inputString[i + 1] == delimiter)
			numWords++;
	if (numWords == 0)
		return (NULL);
	resultArray = malloc((1 + numWords) * sizeof(char *));
	if (!resultArray)
		return (NULL);
	for (i = 0, j = 0; j < numWords; j++)
	{
		while (inputString[i] == delimiter && inputString[i] != delimiter)
			i++;
		k = 0;
		while (inputString[i + k] != delimiter && inputString[i + k] && inputString[i + k] != delimiter)
			k++;
		resultArray[j] = malloc((k + 1) * sizeof(char));
		if (!resultArray[j])
		{
			for (k = 0; k < j; k++)
				free(resultArray[k]);
			free(resultArray);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			resultArray[j][m] = inputString[i++];
		resultArray[j][m] = 0;
	}
	resultArray[j] = NULL;
	return (resultArray);
}

