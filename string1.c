#include "shell.h"

/**
 * copyString - copies a string
 * @destination: the destination
 * @source: the source
 *
 * Return: pointer to destination
 */
char *copyString(char *destination, char *source)
{
	int index = 0;

	if (destination == source || source == NULL)
		return (destination);
	while (source[index])
	{
		destination[index] = source[index];
		index++;
	}
	destination[index] = '\0';
	return (destination);
}

/**
 * stringDuplicate - duplicates a string
 * @str: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *stringDuplicate(const char *str)
{
	int length = 0;
	char *result;

	if (str == NULL)
		return (NULL);
	while (*str++)
		length++;
	result = malloc(sizeof(char) * (length + 1));
	if (!result)
		return (NULL);
	for (length++; length--;)
		result[length] = *--str;
	return (result);
}

/**
 * printString - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void printString(char *str)
{
	int index = 0;

	if (!str)
		return;
	while (str[index] != '\0')
	{
		putCharacter(str[index]);
		index++;
	}
}

/**
 * putCharacter - writes the character c to stdout
 * @character: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int putCharacter(char character)
{
	static int bufferIndex;
	static char buffer[WRITE_BUF_SIZE];

	if (character == BUF_FLUSH || bufferIndex >= WRITE_BUF_SIZE)
	{
		write(1, buffer, bufferIndex);
		bufferIndex = 0;
	}
	if (character != BUF_FLUSH)
		buffer[bufferIndex++] = character;
	return (1);
}

