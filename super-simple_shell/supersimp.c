#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <linux/limits.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdarg.h>

/**
 * struct path_s - singly linked list for PATH environ variable
 * @directory: string of directory
 * @next: next location directory
 *
 * Description: singly linked list node structure
 */
typedef struct path_s
{
	char *directory;
	struct path_s *next;
} path_t;

extern char **environ;
char *_getenv(const char *name);
void printPATH(void);
path_t *buildListPath(void);
int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);

int main(int argc, char *argv[], char **env)
{
	for (size_t i = 0; environ[i] != NULL; i++)
	{
		printf("environ1: %s\n", environ[i]);
		/* printf("env    : %s\n", env[i]); */
	}
	if (argc > 1)
		printf("\n%s\n", _getenv((const char *)argv[1]));
	printPATH();
	/* path_t *pathList = buildListPath(); */
	printf("Setting new env var: Hello \n");
	printf("_setenv return value is %d\n", _setenv("Hello", "World", 1));
	printf("Setting HOSTTYPE to x86\n");
	printf("_setenv return value is %d\n", _setenv("HOSTTYPE", "x86", 1));
	printf("\n%s\n", _getenv("Hello"));
	printf("\n%s\n", _getenv("HOSTTYPE"));
	printf("Unsetting Hello\n");
	printf("_unsetenv return value is %d\n", _unsetenv("OLDPWD"));
	printf("\n%s\n", _getenv("HOSTTYPE"));
	for (size_t i = 0; environ[i] != NULL; i++)
	{
		printf("environ2: %s\n", environ[i]);
		/* printf("env    : %s\n", env[i]); */
	}
	return (0);
}
/**
 * _getenv - gets an environmental value from a name-value pair in environ
 * @name: name in name-value pair
 *
 * Return: value if found, NULL if not
 */
char *_getenv(const char *name) /* gets an environmental variable */
{
	char **current;
	char *token;
	char *temp_line;
	for (current = environ; *current; current++)
	{
		temp_line = strdup(*current);
		token = strtok(temp_line, "=");
		if (strcmp(token, name) == 0)
			return (strtok(NULL, "="));
		free(temp_line);
	}
	return (NULL);
}
/**
 * printPATH - prints the PATH
 */
void printPATH(void)
{
	char *path = _getenv("PATH");
	char *token = strtok(path, ":");

	while (token != NULL)
	{
		printf("%s\n", token);
		token = strtok(NULL, ":");
	}
}
/**
 * buildListPath - builds a singly linked list off the environ variable PATH
 *
 * Returns: singly linked list of PATH variables, NULL if failed
 */
path_t *buildListPath(void)
{
	path_t *new_node = NULL;
	path_t *head = NULL;
	char *path = getenv("PATH");
	char *token = strtok(path, ":");

	while (token != NULL)
	{
		new_node = (path_t *)malloc(sizeof(path_t));
		if (new_node == NULL)
			return (NULL);
		new_node->directory = token;
		new_node->next = head;
		head = new_node;
		token = strtok(NULL, ":");
	}
	return (head);
}

/**
 * _setenv - sets an environmental variable to a new value,
 * or appends a new value if not found
 * @name: name of environmental variable to be set
 * @value: value to set the environmental variable to
 * @overwrite: nonzero to actually change the value, zero to not change
 *
 * Return: 0 on success, -1 on failure
 */
int _setenv(const char *name, const char *value, int overwrite)
{
	int i;
	char *temp;
	char *new_line;
	char *temp_line;
	char found = -1; /* flag for finding name, 0 if found */
	int size_environ = 0;
	char **new_environ;

	if (name == NULL)
		return (-1);
	else if (strlen(name) == 0) /* || strchr(name, '=') == NULL) */
		return (-1);

	new_line = malloc(strlen(name) + strlen(value) + 2); /* line replacement */
	if (new_line == NULL)
		return (-1);
	strcpy(new_line, name);
	strcat(new_line, "=");
	strcat(new_line, value);

	for (i = 0; environ[i] != NULL && overwrite != 0; i++) /* looks for name */
	{
		temp_line = strdup(environ[i]);
		temp = strtok(temp_line, "=");
		size_environ++;

		if (strcmp(temp, name) == 0) /* name found in environ */
		{
			environ[i] = strdup(new_line);
			found = 0;
			break;
		}
	}
	if (found != 0 && overwrite != 0 && environ[i] == NULL) /* if adding at end */
	{
		new_environ = malloc(sizeof(char *) * size_environ + 2);
		if (new_environ == NULL)
		{
			free(new_line);
			return (-1);
		}
		for (i = 0; i < size_environ; i++)
		{
			new_environ[i] = strdup(environ[i]);
		}
		new_environ[i] = new_line;
		new_environ[i + 1] = NULL;
	}
	environ = new_environ;
	return (0);
}

/**
 * _unsetenv - unsets an environmental variable
 * @name: name of environmental variable to remove
 *
 * Return: 0 if success, -1 on failure
 */
int _unsetenv(const char *name)
{
	/* check name, check environ, if found new_environ without name */
	int size_environ = 0;
	char **new_environ;
	int location = -1, i, new_environ_index = 0;

	if (name == NULL)
		return (-1);
	else if (strlen(name) == 0)
		return (-1);
	/* find size of array and location of possible match */
	for (i = 0; environ[i] != NULL; i++)
	{
		size_environ++;
		if (strncmp(environ[i], name, strlen(name)) == 0)
			location = i;
	}
	/* if match found rebuild environ without the found element */
	if (location < i && location != -1)
	{
		new_environ = malloc(sizeof(char *) * size_environ);
		for (i = 0; i < size_environ; i++)
		{
			if (i != location)
			{
				new_environ[new_environ_index] = strdup(environ[i]);
				new_environ_index++;
			}
		}
		new_environ[new_environ_index] = NULL;
		environ = new_environ;
	}
	return (0);
}
