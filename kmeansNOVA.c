#include <math.h>
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

void sum_clusters_reset(double **sum_clusters, int num_clusters, int vector_length)
{
    int i;
    int j;
    for (i = 0; i < num_clusters; i++)
    {
        for (j = 0; j < vector_length; j++)
        {
            sum_clusters[i][j] = 0;
        }
    }
}

void counters_reset(int *counters, int num_clusters)
{
    int i;
    for (i = 0; i < num_clusters; i++)
    {
        counters[i] = 0;
    }
}

int have_centroids_changed(double *delta_centroids, double epsilon, int num_clusters)
{
    int i;
    for (i = 0; i < num_clusters; i++)
    {
        if (delta_centroids[i] >= epsilon)
        {
            return 1;
        }
    }

    return 0;
}

double euclidean_distance(double *cord1, double *cord2, int vector_length)
{
    double sum = 0;
    int i;
    double sqrt_sum;
    double distance;
    double squared_cord_distance;
    for (i = 0; i < vector_length; i++)
    {
        distance = cord1[i] - cord2[i];
        squared_cord_distance = pow(distance, 2);
        sum += squared_cord_distance;
    }
    sqrt_sum = sqrt(sum);
    return sqrt_sum;
}
int find_closest_centroid(struct cord *curr_cord, double **centroids, int num_clusters, int vector_length)
{
    double *curr_cord_array = malloc(vector_length * sizeof(double));
    int cord_index = 0;
    double min_index;
    double min_distance;
    int i;

    while (curr_cord != NULL)
    {
        curr_cord_array[cord_index] = curr_cord->value;
        curr_cord = curr_cord->next;
        cord_index++;
    }
    min_distance = euclidean_distance(curr_cord_array, centroids[0], vector_length);
    min_index = 0;
    for (i = 1; i < num_clusters; i++)
    {
        double current_distance = euclidean_distance(curr_cord_array, centroids[i], vector_length);
        if (current_distance < min_distance)
        {
            min_distance = current_distance;
            min_index = i;
        }
    }
    free(curr_cord_array);
    return min_index;
}

int main(int argc, char **argv)
{
    int i;
    int j;
    double epsilon;
    int num_iterations = 200;
    double **sum_clusters;
    int *counters;
    double **centroids;
    double delta;
    int num_clusters;
    int vector_length;
    int in_first_vector;
    int total_vectors;
    int curr_iter;
    int vector_index = 0;
    struct vector *head_vec, *curr_vec, *next_vec;
    struct cord *head_cord, *curr_cord, *next_cord;
    double n;
    char c;
    double *delta_centroids;

    if (argc < 2 || argc > 3)
    {
        printf("An Error Has Occurred\n");
        return 1;
    }

    if (argc == 3)
    {
        num_iterations = atoi(argv[2]);
    }

    epsilon = 0.001;

    if (atoi(argv[1]) == 0)
    {
        printf("Invalid number of clusters!");
        return 1;
    }
    if (atoi(argv[2]) == 0)
    {
        printf("Invalid maximum iterations!");
        return 1;
    }
    num_clusters = atoi(argv[1]);
    vector_length = 0;

    if (num_iterations <= 1 || num_clusters >= 1000)
    {
        printf("Invalid maximum iterations!");
    }

    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;
    in_first_vector = 1;

    total_vectors = 0;

    while (scanf("%lf%c", &n, &c) == 2)
    {
        if (in_first_vector == 1)
        {
            vector_length++;
        }
        if (c == '\n')
        {
            total_vectors++;

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

    if (num_clusters >= total_vectors || num_clusters <= 1)
    {
        printf("Invalid number of clusters!");
        return 1;
    }

    centroids = malloc(num_clusters * sizeof(double *));
    for (i = 0; i < num_clusters; i++)
    {
        centroids[i] = malloc(vector_length * sizeof(double));
    }

    curr_vec = head_vec;
    vector_index = 0;

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

    /* iterate over the array
     for (int i = 0; i < num_clusters; i++)
     {
        for (int j = 0; j < vector_length; j++)
        {
            printf("%f ", centroids[i][j]);
        }
         printf("\n");
    } */

    /* create a 2d array of length of num_clusters */

    sum_clusters = malloc(num_clusters * sizeof(double *));
    counters = malloc(num_clusters * sizeof(int));

    /*initialize sum_clusters with zeros, and size of vector_length*/
    for (i = 0; i < num_clusters; i++)
    {
        sum_clusters[i] = malloc(vector_length * sizeof(double));
        for (j = 0; j < vector_length; j++)
        {
            sum_clusters[i][j] = 0;
        }
    }

    curr_iter = 0;

    delta_centroids = malloc(num_clusters * sizeof(double));

    /* initialize delta_centroids with inifinity*/
    for (i = 0; i < num_clusters; i++)
    {
        delta_centroids[i] = 1000000;
    }

    while (curr_iter < num_iterations && have_centroids_changed(delta_centroids, epsilon, num_clusters))
    {
        /* reset counters */
        sum_clusters_reset(sum_clusters, num_clusters, vector_length);
        counters_reset(counters, num_clusters);

        curr_vec = head_vec;
        while (curr_vec != NULL && curr_vec->cords != NULL)
        {
            struct cord *curr_cord = curr_vec->cords;

            int closest_centroid = find_closest_centroid(curr_cord, centroids, num_clusters, vector_length);

            for (j = 0; j < vector_length; j++)
            {
                sum_clusters[closest_centroid][j] += curr_cord->value;
                curr_cord = curr_cord->next;
            }
            counters[closest_centroid]++;
            curr_vec = curr_vec->next;
        }

        for (i = 0; i < num_clusters; i++)
        {
            double *temp_centroid = malloc(vector_length * sizeof(double));
            for (j = 0; j < vector_length; j++)
            {
                temp_centroid[j] = centroids[i][j];
            }

            for (j = 0; j < vector_length; j++)
            {
                centroids[i][j] = sum_clusters[i][j] / counters[i];
            }
            delta = euclidean_distance(temp_centroid, centroids[i], vector_length);
            delta_centroids[i] = delta;
            free(temp_centroid);
        }

        curr_iter++;
    }
    for (i = 0; i < num_clusters; i++)
    {
        for (j = 0; j < vector_length - 1; j++)
        {
            printf("%.4f,", centroids[i][j]);
        }
        printf("%.4f\n", centroids[i][vector_length - 1]);
    }
    printf("\n");

    for (i = 0; i < num_clusters; i++)
    {
        free(centroids[i]);
    }
    free(centroids);

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

    for (i = 0; i < num_clusters; i++)
    {
        free(sum_clusters[i]);
    }
    free(sum_clusters);
    free(counters);
    free(delta_centroids);

    return 0;
}