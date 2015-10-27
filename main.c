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
int read_matrix_from_file(char *file_name, int mat_num);
int write_matrix_to_file(char *file_name);
char **get_files_list();
char *read_line();
char* connect_strings(char *s1, char *s2);
char **parse_line(char *line);

int main()
{
    run();
    return EXIT_SUCCESS;
}


void run()
{
  char **files = get_files_list();
}

/**
  Reads a matrix from file
*/
int read_matrix_from_file(char *file_name, int mat_num)
{

}

/**
  Writes a matrix to file
*/
int write_matrix_to_file(char *file_name)
{

}

/**
  Reads file names from user, and return all in and out file names
*/
char **get_files_list(){
  char *line = read_line();
  char **files = parse_line(line);

  if (strcmp(files[0], "") == 0){
    files[1] = "a.txt";
    files[2] = "b.txt";
    files[3] = "c.out";
  }
  else if (strcmp(files[1], "") == 0){
    files[1] = connect_strings(files[0], "a.txt");
    files[2] = connect_strings(files[0], "b.txt");
    files[3] = connect_strings(files[0], "c.out");
  }
  else{
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
}

/**
  connects two strings
*/
char *connect_strings(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
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
        tokens[current] = token;
        current ++;
        token = strtok(NULL, TOKEN_DELIM);
    }

    tokens[current] = NULL;
    return tokens;
}

