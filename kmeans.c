#include <stdio.h>
#include <stdlib.h>

struct cord
{
    double value;
    struct cord *next;
};

struct vector
{
    struct vector *next;
    struct cord *cords;
};

int main(int argc, char **argv)
{
    // if (argc != 3)
    // {
    //     fprintf(stderr, "Usage: %s <num_clusters> <num_iterations>\n", argv[0]);
    //     return 1;
    // }
    double epsilon = 0.001;
    int num_clusters = atoi(argv[1]);
    int num_iterations = atoi(argv[2]);
    int vector_length = 0;

    struct vector *head_vec, *curr_vec, *next_vec;
    struct cord *head_cord, *curr_cord, *next_cord;
    double n;
    char c;

    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;

    while (scanf("%lf%c", &n, &c) == 2)
    {
        if (c == '\n')
        {
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;

            head_cord = malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
        }
        else
        {
            curr_cord->value = n;
            curr_cord->next = malloc(sizeof(struct cord));
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
        }
    }

    // count length of one of the vectors

    struct cord *head = head_cord;

    while (head != NULL)
    {
        vector_length++;
        head = head->next;
    }

    printf("%d\n", vector_length);

    // create a 2d array of vectors of length vector_length * num_clusters

    double **array = malloc(num_clusters * sizeof(double *));
    for (int i = 0; i < num_clusters; i++)
    {
        array[i] = malloc(vector_length * sizeof(double));
    }

    // Traverse the linked list of vectors
    curr_vec = head_vec;
    int vector_index = 0;

    while (curr_vec != NULL && curr_vec->cords != NULL && vector_index < num_clusters)
    {
        struct cord *curr_cord = curr_vec->cords;
        int cord_index = 0;

        while (curr_cord != NULL)
        {

            array[vector_index][cord_index] = curr_cord->value;
            curr_cord = curr_cord->next;
            cord_index++;
        }
        curr_vec = curr_vec->next;
        vector_index++;
    }

    // iterate over the array
    for (int i = 0; i < num_clusters; i++)
    {
        for (int j = 0; j < vector_length; j++)
        {
            printf("%f ", array[i][j]);
        }
        printf("\n");
    }

    // free memory
    for (int i = 0; i < num_clusters; i++)
    {
        free(array[i]);
    }
    free(array);

    // free linked lists
    curr_cord = head_cord;
    while (curr_cord != NULL)
    {
        next_cord = curr_cord->next;
        free(curr_cord);
        curr_cord = next_cord;
    }

    curr_vec = head_vec;
    while (curr_vec != NULL)
    {
        next_vec = curr_vec->next;
        free(curr_vec);
        curr_vec = next_vec;
    }

    // printf

    // curr_vec = head_vec;
    // while (curr_vec != NULL && curr_vec->cords != NULL)
    // {
    //     struct cord *curr_cord = curr_vec->cords;
    //     printf("Vector: ");
    //     while (curr_cord != NULL)
    //     {
    //         printf("%f ", curr_cord->value);
    //         curr_cord = curr_cord->next;
    //     }
    //     printf("\n");
    //     curr_vec = curr_vec->next;
    // }

    return 0;
}