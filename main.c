#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "parser.h"

#define MAX_MATRIX_SIZE 500

float a[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
float b[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
float c[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
int a_dimensions[2];  // [0] --> x dimension, [1] --> y dimension
int b_dimensions[2];
int c_dimensions[2];
int num_of_threads_in_element_calculation = 0;
int num_of_threads_in_row_calculation = 0;
unsigned long without_threads_calculation_time = 0;
unsigned long elements_calculation_time = 0;
unsigned long rows_calculation_time = 0;
int error_flag = 0;

void run();
void read_matrix_from_file(char *file_name, int mat_num);
void write_matrix_to_file(char *file_name);
void post_read();
void calculate_without_threads();
void calculate_element_by_element();
void calculate_row_by_row();
void *elements_calculation_thread(void *cell);
void *rows_calculation_thread(void *row);
void print_statistics();

/**
    A struct to hold row and column params of an index. This struct is being
    called by the thread creation function
*/
struct cell
{
    int i; // row
    int j; // column
};

int main()
{
    run();

    if(error_flag == 1)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

/**
  Works like the main function of the program, it calls almost all other functions
*/
void run()
{
    char **files = get_files_list();

    read_matrix_from_file(files[1], 1);
    if (error_flag == 1)
        return;

    read_matrix_from_file(files[2], 2);
    if (error_flag == 1)
        return;

    post_read();
    if (error_flag == 1)
        return;

    calculate_without_threads();
    calculate_element_by_element();
    calculate_row_by_row();
    write_matrix_to_file(files[3]);
    print_statistics();
}

/**
  Reads a matrix from file
*/
void read_matrix_from_file(char *file_name, int mat_num)
{
    FILE *file = fopen(file_name, "r");
    if(!file)
    {
        error_flag = 1;
        perror("Can't find the input file. Terminating...\n");
        return;
    }

    int length, width, i, j;
    float in;
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
        for(j = 0; j < width; j++)
        {
            fscanf(file, "%f", &in);
            if(in == EOF){
              error_flag = 1;
              perror("Can't find all the matrix element, please check the input file again.\n");
              return;
            }
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
  Check if the two matrices multiplication is valid, and assign the dimensions to c matrix
*/
void post_read()
{
    if(a_dimensions[1] != b_dimensions[0])
    {
        error_flag = 1;
        perror("Number of columns of first matrix is not equle number of rows of the second one, can't multiply those two matrices.\n");
        return;
    }
    c_dimensions[0] = a_dimensions[0];
    c_dimensions[1] = b_dimensions[1];
}

/**
   Multiply the two matrices in one thread.
*/
void calculate_without_threads()
{
    printf("Calculating matrix without threads...\n");

    struct timeval stop, start;
    gettimeofday(&start, NULL); //start checking time

    int i, j, k;
    float sum = 0.0;

    for(i = 0; i < c_dimensions[0]; i++)   // rows
    {
        for(j = 0; j < c_dimensions[1]; j++)   // columns
        {
            sum = 0.0;
            for(k = 0; k < a_dimensions[1]; k++) //multiply row by column
            {
                sum += a[i][k] * b[k][j] * 1.0;
            }
            // set the value to it's index
            c[i][j] = sum;
        }
    }

    gettimeofday(&stop, NULL); //end checking time
    without_threads_calculation_time =  stop.tv_usec - start.tv_usec;
}

/**
   Multiply the two matrices by calculating each element in a separate thread.
*/
void calculate_element_by_element()
{
    printf("Calculating matrix element by element...\n");

    struct timeval stop, start;
    gettimeofday(&start, NULL); //start checking time

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
            //pthread_join(tid, NULL);
            num_of_threads_in_element_calculation++;
        }
    }

    gettimeofday(&stop, NULL); //end checking time
    elements_calculation_time =  stop.tv_usec - start.tv_usec;
}

/**
   Multiply the two matrices by calculating each row in a separate thread.
*/
void calculate_row_by_row()
{
    printf("Calculating matrix row by row...\n");

    struct timeval stop, start;
    gettimeofday(&start, NULL); //start checking time

    int i;

    for(i = 0; i < c_dimensions[0]; i++)   // rows
    {
        pthread_t tid;       //Thread ID
        pthread_attr_t attr; //thread attributes

        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, rows_calculation_thread, (void *)i);

        //parent should wait for all thread to complete
        //pthread_join(tid, NULL);
        num_of_threads_in_row_calculation++;
    }

    gettimeofday(&stop, NULL); //end checking time
    rows_calculation_time =  stop.tv_usec - start.tv_usec;
}

/**
  This function is being called by the created thread while calculating the multiplication
  element by element.
*/
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

/**
  This function is being called by the created thread while calculating the multiplication
  row by row.
*/
void *rows_calculation_thread(void *row)
{
    int i = (int)row, j, k;
    float sum = 0.0;

    //multiply row by column
    for(j = 0; j < c_dimensions[1]; j++)
    {
        sum = 0;
        for(k = 0; k < a_dimensions[1]; k++)
        {
            sum += a[i][k] * b[k][j] * 1.0;
        }
        // set the value to it's index
        c[i][j] = sum;
    }

    pthread_exit(0);
}


/**
  Prints the final statistics to the stdout.
*/
void print_statistics()
{
    printf("\nFinal Statestics...\n\n");

    printf("Calculation without threads took: %lu microseconds.\n", without_threads_calculation_time);
    printf("Calculating element by element took: %lu microseconds.\n", elements_calculation_time);
    printf("Calculating row by row took: %lu microseconds.\n\n", rows_calculation_time);

    printf("Number of threads created during element by element calculation: %d\n", num_of_threads_in_element_calculation);
    printf("Number of threads created during row by row calculation: %d\n", num_of_threads_in_row_calculation);
}
