#include "shell.h"

/**
 * printErrorMessage - prints an error message string to stderr
 * @message: the error message to be printed
 *
 * Return: Nothing
 */
void printErrorMessage(char *message)
{
    int i = 0;

    if (!message)
        return;
    while (message[i] != '\0')
    {
        writeErrorMessageChar(message[i]);
        i++;
    }
}

/**
 * writeErrorMessageChar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int writeErrorMessageChar(char c)
{
    static int i;
    static char buf[WRITE_BUF_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        write(2, buf, i);
        i = 0;
    }
    if (c != BUF_FLUSH)
        buf[i++] = c;
    return (1);
}

/**
 * writeCharToFileDescriptor - writes the character c to given file descriptor
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int writeCharToFileDescriptor(char c, int fd)
{
    static int i;
    static char buf[WRITE_BUF_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        write(fd, buf, i);
        i = 0;
    }
    if (c != BUF_FLUSH)
        buf[i++] = c;
    return (1);
}

/**
 * writeStringToFileDescriptor - writes an input string to given file descriptor
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 *
 * Return: the number of characters written
 */
int writeStringToFileDescriptor(char *str, int fd)
{
    int i = 0;

    if (!str)
        return (0);
    while (*str)
    {
        i += writeCharToFileDescriptor(*str++, fd);
    }
    return (i);
}

