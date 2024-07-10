"""
Given a set of N datapoints x1,x2,...,xN ∈ Rd, 
the goal is to group the data into K ∈ N clusters, 
each datapoint is assigned to exactly one cluster 
and the number of clusters K is such that 1 < K < N . 
Each cluster k is represented by its centroid, 
which is the mean μk ∈ Rd of the cluster’s members.
"""

import math

def k_means(K, input_data, iter = 200):

    # Define the stopping criteria
    epsilon = 0.001

    # Open the file in read mode
    with open(input_data, 'r') as file:
        # Read the entire content of the file
        data = file.read()

    # Use the split method to split the content by whitespace (default)

    # Step 1: Split the data into individual lines
    lines = data.strip().split('\n')

    # Step 2: Process each line and convert it to a tuple of floats
    data_points = []
    for line in lines:
        # Step 3: Split the line by comma and strip any whitespace
        parts = line.split(',')
        parts = [float(part.strip()) for part in parts]
        
        # Step 4: Convert to a tuple and add to the list of data points
        data_points.append(tuple(parts))

    centroids = [data_points[i] for i in range(K)]
    delta_centroids = [float("inf") for i in range(K)]

    points_per_cluster = [[] for i in range(K)]
    
    curr_iter = 0
    # Check if all delta of centroids have changed at least to a size bigger than epsilon
    while curr_iter < iter and not have_centroids_changed(delta_centroids, epsilon):

        # Reset the points per cluster, as we will calculate the uk's again
        points_per_cluster = [[] for i in range(K)]

        # Assign each data point to the closest cluster
        for i in range(len(data_points)):
            index_closest_centroid = find_closest_cluster(data_points[i], centroids)
            points_per_cluster[index_closest_centroid].append(data_points[i])
        
        # Calculate the new centroids
        for i in range(len(points_per_cluster)):
            updated_centroid = avg_of_data_points(points_per_cluster[i])
            delta_centroids[i] = euclidean_distance(updated_centroid, centroids[i])

            centroids[i] = updated_centroid

        curr_iter += 1

    # Formatting the elements of each tuple in the list to four decimal places
    centroids = [tuple(f"{x:.4f}" for x in centroid) for centroid in centroids]

    print(centroids)
    print("this is curr_iter: " + str(curr_iter))
    return centroids


def have_centroids_changed(delta_centroids, epsilon):
    for delta_centroid in delta_centroids:
        if delta_centroid >= epsilon:
            return False
        
    return True

# Find closest index of cluster, which data_point is closest to
def find_closest_cluster(data_point, centroids):
    min_dist = euclidean_distance(data_point, centroids[0])
    min_index = 0
    for i in range(len(centroids)):
        dist_from_centroid = euclidean_distance(data_point, centroids[i])
        if dist_from_centroid < min_dist:
            min_dist = dist_from_centroid
            min_index = i

    return min_index


def euclidean_distance(point1, point2):
    # Ensure the points have the same number of dimensions
    if len(point1) != len(point2):
        raise ValueError("Points must have the same number of dimensions")

    # Calculate the sum of squared differences
    sum_squared_diff = sum((p1 - p2) ** 2 for p1, p2 in zip(point1, point2))

    # Take the square root of the sum to get the Euclidean distance
    distance = math.sqrt(sum_squared_diff)
    return distance

def avg_of_data_points(list_of_tuples):
    dim = len(list_of_tuples[0])
    sum_of_coordinates = [0 for i in range(dim)]
    for data_tuple in list_of_tuples:
        for coordinate in range(dim):
            sum_of_coordinates[coordinate] += data_tuple[coordinate]

    return tuple([sum_of_coordinate / len(list_of_tuples) for sum_of_coordinate in sum_of_coordinates])
        

k_means(7, 'input_2.txt')

    

    
