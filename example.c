#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_CITIES 10

// Function to calculate the total weight of a path
int calculatePathWeight(int path[], int weights[][MAX_CITIES], int numCities) {
    int weight = 0;
    for (int i = 0; i < numCities - 1; i++) {
        weight += weights[path[i]][path[i + 1]];
    }
    weight += weights[path[numCities - 1]][path[0]]; // Return to the starting city
    return weight;
}

// Function to swap two elements in an array
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to generate all possible paths
void generateAllPaths(int path[], int weights[][MAX_CITIES], int numCities, int currentIndex) {
    if (currentIndex == numCities - 1) {
        // Calculate the weight of the current path
        int currentWeight = calculatePathWeight(path, weights, numCities);

        // Print the current path and its weight
        printf("Path: ");
        for (int i = 0; i < numCities; i++) {
            printf("%d ", path[i]);
        }
        printf("-> Total Weight: %d\n", currentWeight);

        return;
    }

    for (int i = currentIndex; i < numCities; i++) {
        // Swap the current element with itself and all the subsequent elements
        swap(&path[currentIndex], &path[i]);

        // Recursively generate paths
        generateAllPaths(path, weights, numCities, currentIndex + 1);

        // Backtrack
        swap(&path[currentIndex], &path[i]);
    }
}

int main() {
    int weights[MAX_CITIES][MAX_CITIES] = {
        {0, 2, 9, 10},
        {1, 0, 6, 4},
        {15, 7, 0, 8},
        {6, 3, 12, 0}
    };

    int numCities = 4;
    int path[MAX_CITIES];

    // Initialize the path array with city indices
    for (int i = 0; i < numCities; i++) {
        path[i] = i;
    }

    printf("All possible paths:\n");
    generateAllPaths(path, weights, numCities, 0);

    return 0;
}
