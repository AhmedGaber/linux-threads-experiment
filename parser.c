#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define TOKEN_DELIM " \t\r\n\a"

/**
  Reads file names from user, and return all in and out file names
*/
char **get_files_list()
{
    char *line = read_line();
    char **files = parse_line(line);

    if (files[0] == NULL)
    {
        files[1] = "a.txt";
        files[2] = "b.txt";
        files[3] = "c.out";
    }
    /*    else if (files[1] == NULL)
        {
            printf("lknj");
            files[1] = connect_strings(files[0], "a.txt");
            files[2] = connect_strings(files[0], "b.txt");
            files[3] = connect_strings(files[0], "c.out");
        }
    */
    else
    {
        files[1] = connect_strings(files[0], files[1]);
        files[2] = connect_strings(files[0], files[2]);
        files[3] = connect_strings(files[0], files[3]);
    }
    return files;
}

/**
  Reads a line from terminal
*/
char *read_line()
{
    char *line = malloc(sizeof(char) * 200);
    int current = 0;
    char c;
    c = getc(stdin);
    while (c != '\n')
    {
        line[current++] = c;
        c = getc(stdin);
    }
    line[current] = '\0';
    return line;
}

/**
  connects two strings
*/
char *connect_strings(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+2);
    char *s3 = "/";
    strcpy(result, s1);
    strcat(result, s3);
    strcat(result, s2);
    return result;
}

/**
  parses a line and returns the parsed values in a 2D array
*/
char **parse_line(char *line)
{
    int current = 0;
    char **tokens = malloc(200 * sizeof(char*));
    char *token;

    token = strtok(line, TOKEN_DELIM);
    while(token != NULL)
    {
        tokens[current++] = token;
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[current] = NULL;
    return tokens;
}
