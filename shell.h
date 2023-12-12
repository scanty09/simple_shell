#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

/* for command chaining */
#define CMD_NORMAL	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HISTORY_FILE	".simple_shell_history"
#define HISTORY_MAX	4096

extern char **environ;

/**
 * struct StringList - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct StringList
{
	int num;
	char *str;
	struct StringList *next;
} StringList_t;

/**
 *struct ShellInfo - contains pseudo-arguments to pass into a function,
 *		allowing a uniform prototype for a function pointer struct
 *@arg: a string generated from getline containing arguments
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@lineCount: the error count
 *@errNum: the error code for exit()s
 *@lineCountFlag: if on, count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@envChanged: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmdBuffer: address of a pointer to cmdBuffer, on if chaining
 *@cmdBufferType: CMD_type ||, &&, ;
 *@readFd: the fd from which to read line input
 *@histCount: the history line number count
 */
typedef struct ShellInfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int lineCount;
	int errNum;
	int lineCountFlag;
	char *fname;
	StringList_t *env;
	StringList_t *history;
	StringList_t *alias;
	char **environ;
	int envChanged;
	int status;

	char **cmdBuffer; /* pointer to cmd ; chain buffer, for memory management */
	int cmdBufferType; /* CMD_type ||, &&, ; */
	int readFd;
	int histCount;
} ShellInfo_t;

#define SHELL_INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct BuiltinCommand - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct BuiltinCommand
{
	char *type;
	int (*func)(ShellInfo_t *);
} BuiltinCommand_t;

/* toem_shloop.c */
int shellLoop(ShellInfo_t *, char **);
int findBuiltin(ShellInfo_t *);
void findCommand(ShellInfo_t *);
void forkCommand(ShellInfo_t *);

/* toem_parser.c */
int isCommand(ShellInfo_t *, char *);
char *duplicateChars(char *, int, int);
char *findPath(ShellInfo_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void errorPuts(char *);
int errorPutchar(char);
int putFd(char c, int fd);
int putsFd(char *str, int fd);

/* toem_string.c */
int stringLength(char *);
int stringCompare(char *, char *);
char *startsWith(const char *, const char *);
char *stringConcatenate(char *, char *);

/* toem_string1.c */
char *stringCopy(char *, char *);
char *stringDuplicate(const char *);
void printString(char *);
int putcharString(char);

/* toem_exits.c */
char *stringNCopy(char *, char *, int);
char *stringNCat(char *, char *, int);
char *stringChar(char *, char);

/* toem_tokenizer.c */
char **tokenizeString(char *, char *);
char **tokenizeString2(char *, char);

/* toem_realloc.c */
char *memorySet(char *, char, unsigned int);
void freeStrings(char **);
void *reallocateMemory(void *, unsigned int, unsigned int);

/* toem_memory.c */
int freeMemory(void **);

/* toem_atoi.c */
int interactiveMode(ShellInfo_t *);
int isDelimiter(char, char *);
int isAlphabetic(int);
int convertAtoi(char *);

/* toem_errors1.c */
int errorAtoi(char *);
void printErrorMessage(ShellInfo_t *, char *);
int printDecimal(int, int);
char *convertNumber(long int, int, int);
void removeComments(char *);

/* toem_builtin.c */
int exitShell(ShellInfo_t *);
int changeDirectory(ShellInfo_t *);
int shellHelp(ShellInfo_t *);

/* toem_builtin1.c */
int shellHistory(ShellInfo_t *);
int shellAlias(ShellInfo_t *);

/*toem_getline.c */
ssize_t getInput(ShellInfo_t *);
int getLine(ShellInfo_t *, char **, size_t *);
void signalIntHandler(int);

/* toem_getinfo.c */
void clearShellInfo(ShellInfo_t *);
void setShellInfo(ShellInfo_t *, char **);
void freeShellInfo(ShellInfo_t *, int);

/* toem_environ.c */
char *getEnvironmentVariable(ShellInfo_t *, const char *);
int shellEnvironment(ShellInfo_t *);
int setShellEnvironment(ShellInfo_t *);
int unsetShellEnvironment(ShellInfo_t *);
int populateEnvironmentList(ShellInfo_t *);

/* toem_getenv.c */
char **getShellEnvironment(ShellInfo_t *);
int unsetShellEnvironmentVariable(ShellInfo_t *, char *);
int setShellEnvironmentVariable(ShellInfo_t *, char *, char *);

/* toem_history.c */
char *getHistoryFile(ShellInfo_t *shellInfo);
int writeHistory(ShellInfo_t *shellInfo);
int readHistory(ShellInfo_t *shellInfo);
int buildHistoryList(ShellInfo_t *shellInfo, char *buf, int lineCount);
int renumberHistory(ShellInfo_t *shellInfo);

/* toem_lists.c */
StringList_t *addNode(StringList_t **, const char *, int);
StringList_t *addNodeEnd(StringList_t **, const char *, int);
size_t printListString(const StringList_t *);
int deleteNodeAtIndex(StringList_t **, unsigned int);
void freeStringList(StringList_t **);

/* toem_lists1.c */
size_t listLength(const StringList_t *);
char **listToStrings(StringList_t *);
size_t printList(const StringList_t *);
StringList_t *nodeStartsWith(StringList_t *, char *, char);
ssize_t getNodeIndex(StringList_t *, StringList_t *);

/* toem_vars.c */
int isCommandChain(ShellInfo_t *, char *, size_t *);
void checkCommandChain(ShellInfo_t *, char *, size_t *, size_t, size_t);
int replaceAlias(ShellInfo_t *);
int replaceVariables(ShellInfo_t *);
int replaceString(char **, char *);

#endif

