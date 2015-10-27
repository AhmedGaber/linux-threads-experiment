#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MATRIX_SIZE 500
#define TOKEN_DELIM " \t\r\n\a"

int a[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
int b[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
int c[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
int a_dimensions[2];  // [0] --> x dimension, [1] --> y dimension
int b_dimensions[2];
int c_dimensions[2];

void run();
int read_matrix_from_file(char *file_name);
int write_matrix_to_file(char *file_name);
char *read_line();
char* concat(char *s1, char *s2);
char **parse_line(char *line);

int main()
{
    run();
    return EXIT_SUCCESS;
}


void run()
{
    char *line = read_line();

}

int read_matrix_from_file(char *file_name)
{

}

int write_matrix_to_file(char *file_name)
{

}

char *read_line()
{

}

char *concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char **parse_line(char *line)
{
    int current = 0;
    char **tokens = malloc(200 * sizeof(char*));
    char *token;

    token = strtok(line, TOKEN_DELIM);
    while(token != NULL)
    {
        tokens[current] = token;
        current ++;
        token = strtok(NULL, TOKEN_DELIM);
    }

    tokens[current] = NULL;
    return tokens;
}

