#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_MATRIX_SIZE 500
#define TOKEN_DELIM " \t\r\n\a"

float a[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
float b[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
float c[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
int a_dimensions[2];  // [0] --> x dimension, [1] --> y dimension
int b_dimensions[2];
int c_dimensions[2];
int num_of_threads_in_element_calculation = 0;
int num_of_threads_in_row_calculation = 0;

void run();
void read_matrix_from_file(char *file_name, int mat_num);
void write_matrix_to_file(char *file_name);
char **get_files_list();
char *read_line();
char* connect_strings(char *s1, char *s2);
char **parse_line(char *line);
void calculate_element_by_element();
void calculate_row_by_row();
void *elements_calculation_thread(void *cell);
void *rows_calculation_thread(void *row);
void print_statistics();

struct cell
{
    int i; // row
    int j; // column
};

int main()
{
    run();
    return EXIT_SUCCESS;
}


void run()
{
    char **files = get_files_list();
    read_matrix_from_file(files[1], 1);
    read_matrix_from_file(files[2], 2);
    // check if the matrices is valid

    calculate_element_by_element();
    calculate_row_by_row();
    print_statistics();
    write_matrix_to_file(files[3]);
}

/**
  Reads a matrix from file
*/
void read_matrix_from_file(char *file_name, int mat_num)
{
    FILE *file = fopen(file_name, "r");
    int length, width, i, j;
    fscanf(file, "%d", &length);
    fscanf(file, "%d", &width);

    if(mat_num == 1)
    {
        printf("Reading first matrix...\n");
        a_dimensions[0] = length;
        a_dimensions[1] = width;
    }
    else
    {
        printf("Reading second matrix...\n");
        b_dimensions[0] = length;
        b_dimensions[1] = width;
    }

    for(i = 0; i < length; i++)
    {
        for(j = 0; j< width; j++)
        {
            float in;
            fscanf(file, "%f", &in);
            mat_num == 1 ? (a[i][j] = in) : (b[i][j] = in);
        }
    }
    fclose(file);
}

/**
  Writes a matrix to file
*/
void write_matrix_to_file(char *file_name)
{
    printf("Writing matrix to the output file...\n");
    int i, j;
    FILE *file = fopen(file_name, "w");
    fprintf(file,"%d %d\n",c_dimensions[0], c_dimensions[1]);

    for(i = 0; i < c_dimensions[0]; i++)
    {
        for(j = 0; j< c_dimensions[1]; j++)
        {
            fprintf(file, "%f ", c[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

/**
  Reads file names from user, and return all in and out file names
*/
char **get_files_list()
{
    char *line = read_line();
    char **files = parse_line(line);

    if (strcmp(files[0], "") == 0)
    {
        files[1] = "a.txt";
        files[2] = "b.txt";
        files[3] = "c.out";
    }
    else if (strcmp(files[1], "") == 0)
    {
        files[1] = connect_strings(files[0], "a.txt");
        files[2] = connect_strings(files[0], "b.txt");
        files[3] = connect_strings(files[0], "c.out");
    }
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

void calculate_element_by_element()
{
    printf("Calculating matrix element by element...\n");
    int i,j;

    for(i = 0; i < c_dimensions[0]; i++)   // rows
    {
        for(j = 0; j < c_dimensions[1]; j++)   // columns
        {
            struct cell *index = (struct cell *) malloc(sizeof(struct cell));
            index->i = i;
            index->j = j;

            pthread_t tid;       //Thread ID
            pthread_attr_t attr; //thread attributes

            pthread_attr_init(&attr);
            pthread_create(&tid, &attr, elements_calculation_thread, index);

            //parent should wait for all thread to complete
            pthread_join(tid, NULL);
            num_of_threads_in_element_calculation++;
        }
    }
}

void calculate_row_by_row()
{
    printf("Calculating matrix row by row...\n");
    int i;

    for(i = 0; i < c_dimensions[0]; i++)   // rows
    {
        pthread_t tid;       //Thread ID
        pthread_attr_t attr; //thread attributes

        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, elements_calculation_thread, (void *)i);

        //parent should wait for all thread to complete
        pthread_join(tid, NULL);
        num_of_threads_in_row_calculation++;
    }
}

void *elements_calculation_thread(void *cell)
{
    struct cell *current_cell = cell;
    int k;
    float sum = 0.0;

    //multiply row by column
    for(k = 0; k< a_dimensions[1]; k++)
    {
        sum += a[current_cell->i][k] * b[k][current_cell->j] * 1.0;
    }

    // set the value to it's index
    c[current_cell->i][current_cell->j] = sum;

    pthread_exit(0);
}

void *rows_calculation_thread(void *row)
{
    int i, j, k;
    float sum = 0.0;

    //multiply row by column
    for(i = 0; i < c_dimensions[1]; i++)
    {
        for(j = 0; j< a_dimensions[1]; j++)
        {
            sum += a[(int)row][j] * b[j][i] * 1.0;
        }
        // set the value to it's index
        c[(int)row][i] = sum;
    }

    pthread_exit(0);
}

void print_statistics()
{

}
