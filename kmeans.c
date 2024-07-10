#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void sum_clusters_reset(double **sum_clusters, int num_clusters, int vector_length)
{
    for (int i = 0; i < num_clusters; i++)
    {
        for (int j = 0; j < vector_length; j++)
        {
            sum_clusters[i][j] = 0;
        }
    }
};

void counters_reset(int *counters, int num_clusters)
{
    for (int i = 0; i < num_clusters; i++)
    {
        counters[i] = 0;
    }
}

int have_centroids_changed(double *delta_centroids, double epsilon, int num_clusters)
{
    for (int i = 0; i < num_clusters; i++)
    {
        if (delta_centroids[i] >= epsilon)
        {
            return 1;
        }
    }

    return 0;
};

double euclidean_distance(double *cord1, double *cord2, int vector_length)
{
    int sum = 0;
    for (int i = 0; i < vector_length; i++)
    {
        // calculate euclidean distance between two vectors
        double distance = cord1[i] - cord2[i];
        double squared_cord_distance = pow(distance, 2);
        sum += squared_cord_distance;
    }
    // calculate square root of sum
    double sqrt_sum = sqrt(sum);
    return sqrt_sum;
}
int find_closest_centroid(struct cord *curr_cord, double **centroids, int num_clusters, int vector_length)
{
    // malloc a curr_cord_array to hold the current vector
    double *curr_cord_array = malloc(vector_length * sizeof(double));
    int cord_index = 0;
    while (curr_cord != NULL)
    {
        curr_cord_array[cord_index] = curr_cord->value;
        curr_cord = curr_cord->next;
        cord_index++;
    }
    // convert curr_cord to array for easier distance calculation
    double min_distance = euclidean_distance(curr_cord_array, centroids[0], vector_length);
    double min_index = 0;
    // returns index of the closest centroid
    for (int i = 1; i < num_clusters; i++)
    {
        double current_distance = euclidean_distance(curr_cord_array, centroids[i], vector_length);
        if (current_distance < min_distance)
        {
            min_distance = current_distance;
            min_index = i;
        }
    }
    free(curr_cord_array); // free the allocated memory for curr_cord_array
    return min_index;
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
    int in_first_vector = 1;

    while (scanf("%lf%c", &n, &c) == 2)
    {
        if (in_first_vector == 1)
        {
            vector_length++;
        }
        if (c == '\n')
        {
            in_first_vector = 0;
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

    // printf("THIS IS VECTOR LENGTH:%d\n", vector_length);

    // create a 2d array of vectors of length vector_length * num_clusters

    double **centroids = malloc(num_clusters * sizeof(double *));
    for (int i = 0; i < num_clusters; i++)
    {
        centroids[i] = malloc(vector_length * sizeof(double));
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
            centroids[vector_index][cord_index] = curr_cord->value;
            curr_cord = curr_cord->next;
            cord_index++;
        }
        curr_vec = curr_vec->next;
        vector_index++;
    }

    // iterate over the array
    // for (int i = 0; i < num_clusters; i++)
    // {
    //     for (int j = 0; j < vector_length; j++)
    //     {
    //         printf("%f ", centroids[i][j]);
    //     }
    //     printf("\n");
    // }

    // create a 2d array of length of num_clusters

    double **sum_clusters = malloc(num_clusters * sizeof(double *));
    int *counters = malloc(num_clusters * sizeof(int));

    // initialize sum_clusters with zeros, and size of vector_length
    for (int i = 0; i < num_clusters; i++)
    {
        sum_clusters[i] = malloc(vector_length * sizeof(double));
        for (int j = 0; j < vector_length; j++)
        {
            sum_clusters[i][j] = 0;
        }
    }

    int curr_iter = 0;

    double *delta_centroids = malloc(num_clusters * sizeof(double)); // NEED TO IMPLEMENT

    // initialize delta_centroids with inifinity
    for (int i = 0; i < num_clusters; i++)
    {
        delta_centroids[i] = INFINITY;
    }

    while (curr_iter < num_iterations && have_centroids_changed(delta_centroids, epsilon, num_clusters))
    {
        // reset counters
        sum_clusters_reset(sum_clusters, num_clusters, vector_length);
        counters_reset(counters, num_clusters);

        // print sum_clusters and counters after reset
        for (int i = 0; i < num_clusters; i++)
        {
            printf("Sum cluster %d: ", i);
            for (int j = 0; j < vector_length; j++)
            {
                printf("%f ", sum_clusters[i][j]);
            }
            printf("\n");
        }
        printf("Counters: ");
        for (int i = 0; i < num_clusters; i++)
        {
            printf("%d ", counters[i]);
        }

        // printf("Iteration %d:\n", curr_iter);

        curr_vec = head_vec;
        while (curr_vec != NULL && curr_vec->cords != NULL)
        {
            struct cord *curr_cord = curr_vec->cords;

            int closest_centroid = find_closest_centroid(curr_cord, centroids, num_clusters, vector_length); // IMPLEMENT FIND_CLOSEST_CENTROID

            for (int j = 0; j < vector_length; j++)
            {
                sum_clusters[closest_centroid][j] += curr_cord->value;
                curr_cord = curr_cord->next;
            }
            counters[closest_centroid]++;
            curr_vec = curr_vec->next;
        }
        // recalculate centroids
        for (int i = 0; i < num_clusters; i++)
        {
            // copy centroids[i] to a temp vector
            double *temp_centroid = malloc(vector_length * sizeof(double));
            for (int j = 0; j < vector_length; j++)
            {
                temp_centroid[j] = centroids[i][j];
            }

            for (int j = 0; j < vector_length; j++)
            {
                centroids[i][j] = sum_clusters[i][j] / counters[i];
            }
            // calculate euclidean distance between temp_centroid and centroids[i]
            double delta = euclidean_distance(temp_centroid, centroids[i], vector_length);
            delta_centroids[i] = delta;
            free(temp_centroid);
        }

        curr_iter++;
    }
    for (int i = 0; i < num_clusters; i++)
    {
        for (int j = 0; j < vector_length; j++)
        {
            printf("%.4f ", centroids[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // free memory
    for (int i = 0; i < num_clusters; i++)
    {
        free(centroids[i]);
    }
    free(centroids);

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

    for (int i = 0; i < num_clusters; i++)
    {
        free(sum_clusters[i]);
    }
    free(sum_clusters);
    free(counters);
    free(delta_centroids);

    return 0;
};